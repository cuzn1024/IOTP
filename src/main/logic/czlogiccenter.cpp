#include <logic/czlogiccenter.h>

#include <bitset>

#include <boost/bind.hpp>
#include <boost/date_time.hpp>

#include <system/czlog.h>
#include <czsingletonmanager.h>
#include <czconfiguration.h>
#include <thread/czmessagepool.h>
#include <logic/czdatahelper.h>
#include <network/cznodemanager.h>
#include <network/czsessionmanager.h>
#include <utils/czmiscs.h>
#include <network/cznode.h>

using namespace boost;
using namespace cz_defines;
using namespace cz_types;

CZLogicCenter::CZLogicCenter()
{
    responseBuffer = new CZUChar[receive_buffer_size];
    timerTaskBuffer = new CZUChar[receive_buffer_size];
    task = (TimerTask*)timerTaskBuffer;
    bufferLen = 0;
}

CZLogicCenter::~CZLogicCenter()
{
    delete[] responseBuffer;
    responseBuffer = 0;

    delete[] timerTaskBuffer;
    timerTaskBuffer = 0;
}

void CZLogicCenter::init()
{
    for (int i = 0; i < 255; i++)
    {
        registerEntry((eDataId)i, NULL);

        registerRetryEntry((eDataId)i, NULL);
    }
    //定义逻辑处理函数
    registerEntry(EdReqJoinNwk, ENTRY_TYPE_BIND(&CZLogicCenter::edReqJoinNetwork));
    registerEntry(ApReqJoinNwk, ENTRY_TYPE_BIND(&CZLogicCenter::apReqJoinNetwork));
    registerEntry(GetSysTime, ENTRY_TYPE_BIND(&CZLogicCenter::getSysTime));
    registerEntry(EdActive, ENTRY_TYPE_BIND(&CZLogicCenter::edActive));
    registerEntry(Disconnect, ENTRY_TYPE_BIND(&CZLogicCenter::disconnectAck));
    registerEntry(Fault, ENTRY_TYPE_BIND(&CZLogicCenter::faultReport));
    registerEntry(GetDeviceInfo, ENTRY_TYPE_BIND(&CZLogicCenter::getDeviceInfo));

    //定义重传逻辑处理函数
    registerRetryEntry(SetApWorkTab, ENTRY_RETRY_TYPE_BIND(&CZLogicCenter::setApWorkTab));
    registerRetryEntry(NwkResource, ENTRY_RETRY_TYPE_BIND(&CZLogicCenter::nwkResource));
    registerRetryEntry(GetWlQty, ENTRY_RETRY_TYPE_BIND(&CZLogicCenter::getWlQty));
    registerRetryEntry(ModifyEdNwkConfig, ENTRY_RETRY_TYPE_BIND(&CZLogicCenter::setModifyEdNwkConfig));
    registerRetryEntry(SetEdNwkConfig, ENTRY_RETRY_TYPE_BIND(&CZLogicCenter::setEdNwkConfig));
    registerRetryEntry(SetApNwkConfig, ENTRY_RETRY_TYPE_BIND(&CZLogicCenter::setApNwkConfig));
    registerRetryEntry(ReqAddOrDelEd, ENTRY_RETRY_TYPE_BIND(&CZLogicCenter::reqAddOrDelEd));
    registerRetryEntry(ReqAddOrDelGroup, ENTRY_RETRY_TYPE_BIND(&CZLogicCenter::reqAddOrDelGroup));
    registerRetryEntry(Disconnect, ENTRY_RETRY_TYPE_BIND(&CZLogicCenter::disconnect));
    registerRetryEntry(HopFreq, ENTRY_RETRY_TYPE_BIND(&CZLogicCenter::hopFreq));
    registerRetryEntry(Test, ENTRY_RETRY_TYPE_BIND(&CZLogicCenter::test));
}

int CZLogicCenter::processMessage(cz_types::RequestMessage* requestMessage)
{
    if (NULL == requestMessage)
    {
        return 1;
    }

    bufferLen = 0;

    //判断入网请求和data_id 的对应是否正确

    //添加入网网络地址和dataid校验
    if (requestMessage->src_addr == JOIN_ED_NETWORK_ADDR && requestMessage->data_id != EdReqJoinNwk)
    {
        DebugLog << "终端申请入网地址不可以操作其他业务,data_id:" << ByteToHexStr(requestMessage->data_id);
        return 1;
    }

    if (requestMessage->src_addr == JOIN_SITE_NETWORK_ADDR && requestMessage->data_id != ApReqJoinNwk)
    {
        DebugLog << "基站申请入网地址不可以操作其他业务,data_id:" << ByteToHexStr(requestMessage->data_id);
        return 1;
    }

    if (requestMessage->src_addr != JOIN_ED_NETWORK_ADDR && requestMessage->data_id == EdReqJoinNwk)
    {
        DebugLog << "终端申请入网数据ID和网络地址不匹配";
        return 1;
    }

    if (requestMessage->src_addr != JOIN_SITE_NETWORK_ADDR && requestMessage->data_id == ApReqJoinNwk)
    {
        DebugLog << "基站申请入网数据ID和网络地址不匹配";
        return 1;
    }

    if (bRetryLogic(requestMessage->data_id))
    {
        if (!retryLogicTable[requestMessage->data_id])
        {
            DebugLog << "Can not find retry logic function to match data_id:" << ByteToHexStr(requestMessage->data_id);
            return 1;
        }

        if (requestMessage->msg_type == E_Msg_Type_Client_Request)
        {
            if (!CZSingletonManagerInstance.getNodeManager().checkIsHaveCase(requestMessage->src_addr, requestMessage->data_id))
            {
                DebugLog << "服务器并未主动向 " << ShortToHexStr(requestMessage->src_addr) << " 请求 数据ID:$i:" << ByteToHexStr(requestMessage->data_id) << "$ 数据内容：" <<
                    CharsToHexStr(requestMessage->data, requestMessage->data_len) << " 判断为非法数据 拒绝响应";
                return 1;
            }

            //删除重传任务
            CZSingletonManagerInstance.getMessagePool().delTimerTask(requestMessage->src_addr, requestMessage->data_id);
            return retryLogicTable[requestMessage->data_id](requestMessage, false, false);
        }
        else if (requestMessage->msg_type == E_Msg_Type_TimerTask)
        {
            return retryLogicTable[requestMessage->data_id](requestMessage, true, true);
        }
        else
        {
            return retryLogicTable[requestMessage->data_id](requestMessage, true, false);
        }
    }
    else
    {
        if (CZSingletonManagerInstance.getSessionManager().getSiteAddr(requestMessage->session_id) == 0)
        {
            if (requestMessage->data_id != GetDeviceInfo && requestMessage->data_id != ApReqJoinNwk)
            {
                DebugLog << "获取设备信息 " << requestMessage->session_id;
                bufferLen = 0;
                sendMessage(requestMessage->session_id, 0xff, GetDeviceInfo, responseBuffer, bufferLen);
                return 0;
            }
        }

        if (!logicTable[requestMessage->data_id])
        {
            DebugLog << "Can not find logic function to match data_id:" << ByteToHexStr(requestMessage->data_id);
            return 1;
        }

        return logicTable[requestMessage->data_id](requestMessage);
    }
}

bool CZLogicCenter::bRetryLogic(CZUChar data_id)
{
    if (retryLogicTable[data_id])
    {
        return true;
    }
    else
    {
        return false;
    }
}

int CZLogicCenter::getDeviceInfo(cz_types::RequestMessage *requestMessage)
{
    DebugLog << "设备信息响应帧 " << requestMessage->src_addr;
    if (requestMessage->data_len != 13)
    {
        DebugLog << "长度不等于13";
        return -1;
    }

    if (*(CZUShort*)(requestMessage->data + 8) == 0)
    {
        DebugLog << "网络地址不能为零";
        return -1;
    }

    CZSingletonManagerInstance.getSessionManager().updateSiteInfo(requestMessage->session_id, *(CZUShort*)(requestMessage->data + 8));

    return 0;
}

int CZLogicCenter::setApWorkTab(cz_types::RequestMessage* requestMessage, bool bServerCall, bool bNeedReply)
{
    if (bServerCall)
    {
        DebugLog << "发起工作任务表请求";

        bufferLen = 0;

        eNodeType nodeType;

        CZInt iRet = CZSingletonManagerInstance.getNodeManager().getNodeType(requestMessage->src_addr, nodeType);

        if (iRet)
        {
            DebugLog << "未找到对应的节点类型";
            return 1;
        }

        if (nodeType == M_AP)
        {
            std::vector<CZUShort> edList;

            CZSingletonManagerInstance.getNodeManager().getEdManagerList(requestMessage->src_addr, edList);

            responseBuffer[0] = M_AP;
            bufferLen = 1;

            std::map<eNodeType, std::vector<CZUShort>> deviceTypeMap;
            std::map<eNodeType, std::vector<CZUShort>>::iterator itFind;
            std::vector<CZUShort> tmpVec;
            eNodeType findNodeType;

            for (std::vector<CZUShort>::iterator it = edList.begin(); it != edList.end(); it++)
            {
                tmpVec.clear();
                if (CZSingletonManagerInstance.getNodeManager().getNodeType(*it, findNodeType))
                {
                    continue;
                }

                itFind = deviceTypeMap.find(findNodeType);

                if (itFind != deviceTypeMap.end())
                {
                    tmpVec = itFind->second;
                }

                tmpVec.push_back(*it);

                deviceTypeMap[findNodeType] = tmpVec;
            }

            CZUShort network_addr = 0;

            for (itFind = deviceTypeMap.begin(); itFind != deviceTypeMap.end(); itFind++)
            {
                tmpVec.clear();

                tmpVec = itFind->second;

                if (tmpVec.size() <= 0)
                {
                    continue;
                }

                //DeviceType
                responseBuffer[bufferLen] = itFind->first;
                bufferLen += 1;

                //EDCnt
                CZUShort size = tmpVec.size();

                memcpy(responseBuffer + bufferLen, &size, sizeof(size));
                bufferLen += sizeof(size);

                for (std::vector<CZUShort>::iterator it = tmpVec.begin(); it != tmpVec.end(); it++)
                {
                    network_addr = *it;
                    memcpy(responseBuffer + bufferLen, &network_addr, sizeof(network_addr));
                    bufferLen += sizeof(network_addr);
                }
            }

            sendMessage(requestMessage, responseBuffer, bufferLen);
        }
        else if (nodeType == R_AP)
        {
            responseBuffer[0] = R_AP;
            bufferLen = 1;
            CZUShort max_group = CZSingletonManagerInstance.getNodeManager().getRsiteCurrentGroupSize(requestMessage->src_addr);

            CZUChar max_member_size = R_AP_MAX_GROUP_MEMBER_SIZE;

            memcpy(responseBuffer + bufferLen, &max_group, sizeof(max_group));
            bufferLen += sizeof(max_group);

            responseBuffer[bufferLen] = max_member_size;
            bufferLen += 1;

            sendMessage(requestMessage, responseBuffer, bufferLen);
        }

        DebugLog << "发送工作任务表成功";
    }
    else
    {
        DebugLog << "收到工作任务表响应";

        //成功则发送广播网络资源表
        DebugLog << "设置基站网络配置表成功 广播网络资源表";
        nwkResource(requestMessage, true, false);
    }
    return 0;
}

int CZLogicCenter::edReqJoinNetwork(cz_types::RequestMessage* requestMessage)
{
    bufferLen = 0;

    //终端申请入网
    if (requestMessage->data_len != 9)
    {
        DebugLog << CharsToHexStr(requestMessage->data, requestMessage->data_len) << " 申请入网参数不为9个字节";
        return 1;
    }

    //设备类型

    CZUChar cDeviceType = requestMessage->data[8];

    //终端物理地址
    CZPhysicalAddr phyAddr(requestMessage->data);

    DebugLog << "收到终端" << phyAddr << " 入网请求 设备类型:" << ByteToHexStr(cDeviceType);

    //存在该终端的配置 放行

    //状态
    CZUChar status = 0;

    if (phyAddr.empty())
    {
        DebugLog << "终端未初始化，请联系技术支持";

        status = 2;

        //填充物理地址
        memcpy(responseBuffer, requestMessage->data, requestMessage->data_len);
        bufferLen = requestMessage->data_len;

        //填充状态码
        memcpy(responseBuffer + bufferLen, &status, sizeof(status));
        bufferLen += sizeof(status);

        //返回给终端
        sendMessage(requestMessage, responseBuffer, bufferLen);

        return 0;
    }

    //查看之前是否有网络地址
    CZUShort network_addr = 0;

    bool bSucc = true;

    //判断是否为支持的设备类型
    if (!CZMiscs::isDevice(cDeviceType))
    {
        status = 0x04;
        network_addr = 0;
        bSucc = false;
        DebugLog << phyAddr << " 入网请求受理失败，设备类型错误";
    }

    //检查终端物理地址是否为模拟的基站地址

    eNodeType siteType;
    bool bSiteConfig = CZSingletonManagerInstance.getNodeManager().getSiteIdentity(phyAddr, siteType);

    if (bSiteConfig)
    {
        status = 0x02;
        DebugLog << "终端物理地址被检测为基站地址,以防恶意黑入，禁止入网";
        bSucc = false;
    }

    if (bSucc)
    {
        if (CZSingletonManagerInstance.getConfiguration().isNetworkOpen())
        {
            network_addr = CZSingletonManagerInstance.getNodeManager().getNetworkAddrByPhyAddr(phyAddr);

            if (network_addr > 0)
            {
                DebugLog << phyAddr << " 入网请求受理成功,找到之前分配过的网络地址:" << ShortToHexStr(network_addr);
            }
            else
            {
                network_addr = CZSingletonManagerInstance.getNodeManager().createNode(phyAddr, (eNodeType)cDeviceType, 0);
                DebugLog << phyAddr << " 入网请求受理成功,分配网络地址:" << ShortToHexStr(network_addr);
            }

            bSucc = true;
        }
        else
        {
            bSucc = false;
            //网络已关闭
            status = 0x01;

            DebugLog << phyAddr << " 入网请求受理失败,网络已关闭 不受理入网请求";
        }
    }

    CZSingletonManagerInstance.getNodeManager().setNodeStatus(network_addr, E_Node_Status_Ready);
    CZSingletonManagerInstance.getNodeManager().setFaultSite(network_addr, 0);

    if (bSucc)
    {
        //填充物理地址
        memcpy(responseBuffer, requestMessage->data, requestMessage->data_len - 1);
        bufferLen = requestMessage->data_len - 1;

        //填充状态码
        memcpy(responseBuffer + bufferLen, &status, sizeof(status));
        bufferLen += sizeof(status);

        //填充网络地址
        memcpy(responseBuffer + bufferLen, &network_addr, sizeof(network_addr));
        bufferLen += sizeof(network_addr);

        //返回给终端
        sendMessage(requestMessage, responseBuffer, bufferLen);

        requestMessage->src_addr = network_addr;

        //终端申请入网都要执行质量表请求
        DebugLog << "执行请求网络质量表";
        //没有 则请求网络质量表
        getWlQty(requestMessage);
    }
    else
    {
        //填充物理地址
        memcpy(responseBuffer, requestMessage->data, requestMessage->data_len);
        bufferLen = requestMessage->data_len;

        //填充状态码
        memcpy(responseBuffer + bufferLen, &status, sizeof(status));
        bufferLen += sizeof(status);

        //返回给终端
        sendMessage(requestMessage, responseBuffer, bufferLen);
    }

    return 0;
}

int CZLogicCenter::apReqJoinNetwork(cz_types::RequestMessage* requestMessage)
{
    bufferLen = 0;

    //基站申请入网
    if (requestMessage->data_len != 8)
    {
        DebugLog << CharsToHexStr(requestMessage->data, requestMessage->data_len) << " 申请入网参数不为8位";
        return 1;
    }

    //基站物理地址
    CZPhysicalAddr phyAddr(requestMessage->data);

    DebugLog << "收到基站" << phyAddr << " 入网请求";

    //返回状态码
    CZUChar status = 0;

    //网络地址
    CZUShort network_addr = 0;

    bool bSucc = false;

    //基站身份
    eNodeType nodeType;

    bool bSiteConfig = CZSingletonManagerInstance.getNodeManager().getSiteIdentity(phyAddr, nodeType);

    if (bSiteConfig)
    {
        //存在该基站的配置 放行

        //查看之前是否有网络地址
        network_addr = CZSingletonManagerInstance.getNodeManager().getNetworkAddrByPhyAddr(phyAddr);

        if (network_addr > 0)
        {
            eNodeType oldNodeType;

            CZSingletonManagerInstance.getNodeManager().getNodeType(network_addr, oldNodeType);

            if (oldNodeType == nodeType)
            {
                DebugLog << phyAddr << " 入网请求受理成功,找到之前分配过的网络地址:" << ShortToHexStr(network_addr);
                bSucc = true;
            }
            else
            {
                DebugLog << phyAddr << " 入网请求受理失败,找到之前分配过的网络地址:" << ShortToHexStr(network_addr) << " 但是类型不匹配 请联系管理员排查解决";
                bSucc = false;
                status = 0x04;
            }
        }
        else
        {
            network_addr = CZSingletonManagerInstance.getNodeManager().createNode(phyAddr, nodeType, 0);
            DebugLog << phyAddr << " 入网请求受理成功,分配网络地址:" << ShortToHexStr(network_addr);
            bSucc = true;
        }
    }
    else
    {
        //基站未配置
        status = 0x02;
        DebugLog << phyAddr << " 入网请求受理失败,未进行基站配置";
    }

    if (bSucc)
    {
        //填充物理地址
        memcpy(responseBuffer, requestMessage->data, requestMessage->data_len);
        bufferLen = requestMessage->data_len;

        //填充状态码
        memcpy(responseBuffer + bufferLen, &status, sizeof(status));
        bufferLen += sizeof(status);

        //填充网络地址
        memcpy(responseBuffer + bufferLen, &network_addr, sizeof(network_addr));
        bufferLen += sizeof(network_addr);

        //返回给基站
        sendMessage(requestMessage, responseBuffer, bufferLen);

        //判断基站是否有频点信息
        CZUShort freId = 0;

        CZInt iRet = CZSingletonManagerInstance.getNodeManager().getSiteFreid(network_addr, freId);

        requestMessage->src_addr = network_addr;

        //更新链路与基站关系
        CZSingletonManagerInstance.getSessionManager().updateSiteInfo(requestMessage->session_id, network_addr);
    }
    else
    {
        //填充物理地址
        memcpy(responseBuffer, requestMessage->data, requestMessage->data_len);
        bufferLen = requestMessage->data_len;

        //填充状态码
        memcpy(responseBuffer + bufferLen, &status, sizeof(status));
        bufferLen += sizeof(status);

        //返回给基站
        sendMessage(requestMessage, responseBuffer, bufferLen);
    }

    return 0;
}

int CZLogicCenter::getSysTime(cz_types::RequestMessage* requestMessage)
{
    bufferLen = 0;

    boost::posix_time::ptime time_t_epoch(boost::gregorian::date(1970, 1, 1));
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::universal_time();
    boost::posix_time::time_duration diff = now - time_t_epoch + boost::posix_time::hours(8);
    long long time = diff.total_microseconds();

    //填充时间
    memcpy(responseBuffer, &time, sizeof(time));
    bufferLen = sizeof(time);

    sendMessage(requestMessage, responseBuffer, bufferLen);
    CZUShort freId = 0;

    CZUShort na = requestMessage->src_addr;
    CZInt iRet = CZSingletonManagerInstance.getNodeManager().getSiteFreid(na, freId);

    if (iRet)
    {
        DebugLog << "执行请求网络质量表";
        //没有 则请求网络质量表
        getWlQty(requestMessage);
    }
    else
    {
        DebugLog << "曾经工作在频点:" << freId << "上，下发基站网络配置表";
        //有 下发基站网络配置表
        setApNwkConfig(requestMessage);
    }
    return 0;
}

int CZLogicCenter::edActive(cz_types::RequestMessage* requestMessage)
{
    bufferLen = 0;

    //判断上报者身份

    eNodeType nodeType;

    if (CZSingletonManagerInstance.getNodeManager().getNodeType(requestMessage->src_addr, nodeType))
    {
        return 1;
    }

    if (!CZMiscs::isDevice((CZUChar)nodeType))
    {
        return 1;
    }

    //首次上线 需要通知对应的M基站添加终端

    std::vector<CZUShort> *msiteVec;

    CZSingletonManagerInstance.getNodeManager().getCanReachSites(requestMessage->src_addr, M_AP, msiteVec);

    if (msiteVec == 0 || msiteVec->size() < 1)
    {
        DebugLog << "数据异常 网络地址为" << ShortToHexStr(requestMessage->src_addr) << "的终端并未发现该有的M基站";
        return 1;
    }

    CZSingletonManagerInstance.getNodeManager().setNodeStatus(requestMessage->src_addr, E_Node_Status_Running);

    noticeMSiteAddDevice((*msiteVec)[0], requestMessage->src_addr, true);

    CZUShort sessionId = CZSingletonManagerInstance.getNodeManager().getNodeOverSessionRoad((*msiteVec)[0]);

    responseBuffer[0] = 0;
    bufferLen = 1;
    sendMessage(sessionId, requestMessage->src_addr, EdActive, responseBuffer, bufferLen, false);

    return 0;
}

CZInt CZLogicCenter::hopFreq(cz_types::RequestMessage* requestMessage, bool bServerCall, bool bNeedReply)
{
    if (bServerCall)
    {
        DebugLog << "发起跳频帧";

        std::vector<CZUShort> *msiteVec;

        CZUShort fredId;
        CZSingletonManagerInstance.getNodeManager().getCanReachSites(requestMessage->src_addr, M_AP, msiteVec);
        CZUShort sessionId = CZSingletonManagerInstance.getNodeManager().getNodeOverSessionRoad((*msiteVec)[0]);
        CZSingletonManagerInstance.getNodeManager().getSiteFreid((*msiteVec)[0], fredId);
        CZSingletonManagerInstance.getNodeManager().setFreidInfo(fredId + 1, (*msiteVec)[0]);

        responseBuffer[0] = 5;
        responseBuffer[1] = (*msiteVec)[0];
        *(CZUShort*)(responseBuffer + 2) = fredId;
        *(CZUShort*)(responseBuffer + 4) = fredId + 1;
        bufferLen = 6;

        return sendMessage(sessionId, (*msiteVec)[0], HopFreq, responseBuffer, bufferLen, bNeedReply);
    }
    else
    {
        DebugLog << "收到跳频帧响应";

        return nwkResource(requestMessage, true, false);
    }
}

int CZLogicCenter::disconnectAck(cz_types::RequestMessage* requestMessage)
{
    if (requestMessage->data_len < 2)
    {
        DebugLog << "收到通知基站" << ShortToHexStr(requestMessage->src_addr) << "断开链接返回";
    }

    return 0;
}

int CZLogicCenter::disconnect(cz_types::RequestMessage* requestMessage, bool bServerCall, bool bNeedReply)
{
    bufferLen = 0;

    if (bServerCall)
    {
        responseBuffer[0] = 2;
        responseBuffer[1] = 0;
        *((CZUShort*)(responseBuffer + 2)) = 0;
        bufferLen = 4;
        DebugLog << "向基站" << ShortToHexStr(requestMessage->src_addr) << "发起断开链接请求";
        sendMessage(requestMessage->session_id, requestMessage->src_addr, Disconnect, responseBuffer, bufferLen, bNeedReply);
    }
    else
    {
        if (requestMessage->data_len == 0)
        {
            DebugLog << "收到通知基站" << ShortToHexStr(requestMessage->src_addr) << "断开链接返回";
        }
        else if (requestMessage->data_len == 4)
        {
            bufferLen = 0;
            DebugLog << "收到通知基站" << ShortToHexStr(requestMessage->src_addr) << "断开链接请求";
            sendMessage(requestMessage->session_id, requestMessage->src_addr, Disconnect, responseBuffer, bufferLen, false);
        }
    }

    return 0;
}

int CZLogicCenter::faultReport(cz_types::RequestMessage* requestMessage)
{//只可能接收到无线故障帧
    if (requestMessage->data_len == 0)
    {
        DebugLog << "收到故障帧长度错误 " << ShortToHexStr(requestMessage->data_len);
        return 1;
    }

    bufferLen = 2;
    responseBuffer[0] = requestMessage->data[0];
    responseBuffer[1] = 0;
    sendMessage(requestMessage->session_id, requestMessage->src_addr, Fault, responseBuffer, bufferLen);

    switch (requestMessage->data[0] & 0x7f)
    {
    case AP_WChnDisturb:
        //信道干扰
        DebugLog << "收到基站信道干扰故障帧" << ShortToHexStr(requestMessage->src_addr);

        CZSingletonManagerInstance.getNodeManager().setNodeStatus(requestMessage->src_addr, E_Node_status_Disturb);

        DebugLog << "清除基站频点";

        DebugLog << "扫描网络质量信息";
        getWlQty(requestMessage);

        DebugLog << "广播信道干扰信息";
        DebugLog << "广播网络资源帧";
        break;
    case AP_WHardFault:
        //基站无线硬件故障
        DebugLog << "收到基站无线硬件故障帧" << ShortToHexStr(requestMessage->src_addr);
        DebugLog << "设置该基站为不可用（后续启用备用基站时，应考虑基站修复问题，服务器是否需要清掉相关信息）";

        CZSingletonManagerInstance.getNodeManager().setNodeStatus(requestMessage->src_addr, E_Node_status_Fault);

        DebugLog << "后续终端会重新上报无线链路故障帧，具体由后续帧对应函数处理";
        break;
    case AP_LLinkFault:
        //基站有线链路故障
        break;
    case AP_WLinkFault:
        //基站无线链路故障
        DebugLog << "收到基站无线链路故障帧";

        if ((requestMessage->data[0] & 0x7f) == requestMessage->data[0])
        {
            CZSingletonManagerInstance.getNodeManager().setNodeStatus(requestMessage->src_addr, E_Node_status_Fault);
            DebugLog << "设置基站" << ShortToHexStr(requestMessage->src_addr) << "不可用";
        }
        else
        {
            CZSingletonManagerInstance.getNodeManager().setNodeStatus(requestMessage->src_addr, E_Node_Status_Running);
            DebugLog << "恢复基站" << ShortToHexStr(requestMessage->src_addr) << "可用";
        }

        break;
    case ED_WLinkFault:
        //终端无线链路故障（基站引起）
        DebugLog << "收到无线链路故障帧";

        if (requestMessage->data[0] == 0)
        {
            DebugLog << "不应该出现这种情况，基站应上报终端丢失故障帧";
            return 1;
        }
        else
        {
            DebugLog << "基站" << ByteToHexStr(requestMessage->data[1]) << "引起故障帧";

            DebugLog << "向终端" << ShortToHexStr(requestMessage->src_addr) << "回复故障帧响应";
            requestMessage->session_id = CZSingletonManagerInstance.getNodeManager().getNodePool()[requestMessage->data[2]]->getSessionId();
            CZSingletonManagerInstance.getNodeManager().setNodeStatus(requestMessage->src_addr, E_Node_status_Fault);
            CZSingletonManagerInstance.getNodeManager().setFaultSite(requestMessage->src_addr, requestMessage->data[1]);

            DebugLog << "触发终端获取网络质量表";
            getWlQty(requestMessage);

            //std::vector<CZUShort> *msiteVec;
         //   CZSingletonManagerInstance.getNodeManager().getCanReachSites(requestMessage->src_addr, M_AP, msiteVec);
         //   msiteVec->front() = requestMessage->data[2];
        }

        break;
    case AP_EdLost:
        //终端丢失
        DebugLog << "收到终端丢失故障帧 " << ShortToHexStr(requestMessage->src_addr);

        std::shared_ptr<CZNode> node = CZSingletonManagerInstance.getNodeManager().getNodeByNwkAddr(*(CZUShort*)(requestMessage->data + 1));

        if (node->getStatus() == E_Node_Status_Ready)
            return 0;

        node->setStatus(E_Node_status_Fault);
        DebugLog << "标记丢失，考虑可能由于基站引起，后续会有终端无线链路故障帧上报";
        CZSingletonManagerInstance.getNodeManager().addEdToManagerList(requestMessage->src_addr, *(CZUShort*)(requestMessage->data + 1), false);
        CZSingletonManagerInstance.getDataHelper().addToMSiteList(requestMessage->src_addr, *(CZUShort*)(requestMessage->data + 1), false);
        CZSingletonManagerInstance.getDataHelper().setMSiteInservice(requestMessage->src_addr, *(CZUShort*)(requestMessage->data + 1), false);
        noticeMSiteAddDevice(requestMessage->src_addr, *(CZUShort*)(requestMessage->data + 1), false);
        break;
    }

    return 0;
}

/*********************主动触发***********************/

int CZLogicCenter::nwkResource(cz_types::RequestMessage *requestMessage, bool bServerCall, bool bNeedReply)
{
    bufferLen = 0;

    if (!bServerCall)
    {
        return 1;
    }

    bNeedReply = false;

    //网络ID
    CZUShort network_id = CZSingletonManagerInstance.getConfiguration().getNetworkId();
    bufferLen = 0;
    memcpy(responseBuffer + bufferLen, &network_id, sizeof(network_id));
    bufferLen += sizeof(network_id);

    //网络开放状态
    CZUChar network_status = CZSingletonManagerInstance.getConfiguration().isNetworkOpen() ? 1 : 0;
    memcpy(responseBuffer + bufferLen, &network_status, sizeof(network_status));
    bufferLen += sizeof(network_status);

    //基站数目
    CZUChar iSiteSize = 0;
    CZInt totalSiteCountPos = bufferLen;
    bufferLen += sizeof(iSiteSize);

    std::vector<CZUShort> vecSite;

    CZUShort freId = 0;

    for (int i = M_AP; i < Multiplex_AP; i++)
    {
        CZSingletonManagerInstance.getNodeManager().getApByType((eNodeType)i, vecSite);

        if (vecSite.empty())
            continue;

        responseBuffer[bufferLen++] = i;

        CZInt siteCount = 0;
        CZInt siteCountPos = bufferLen++;

        for (std::vector<CZUShort>::iterator it = vecSite.begin(); it != vecSite.end(); it++)
        {
            siteCount++;
            //基站网络地址short 协议要求改写为char
            responseBuffer[bufferLen++] = (CZUChar)(*it);

            CZSingletonManagerInstance.getNodeManager().getSiteFreid((*it), freId);
            memcpy(responseBuffer + bufferLen, &freId, sizeof(freId));
            bufferLen += sizeof(freId);
        }

        responseBuffer[siteCountPos] = siteCount;
        iSiteSize += siteCount;
    }

    responseBuffer[totalSiteCountPos] = iSiteSize;

    broadcastToAllSite(NwkResource, responseBuffer, bufferLen);

    return 0;
}
int CZLogicCenter::getWlQty(cz_types::RequestMessage* requestMessage, bool bServerCall, bool bNeedReply)
{
    bufferLen = 0;

    //判断是否为基站

    eNodeType nodeType;

    CZInt iRet = CZSingletonManagerInstance.getNodeManager().getNodeType(requestMessage->src_addr, nodeType);

    if (iRet || nodeType < M_AP)
    {
        return 1;
    }

    if (CZMiscs::isDevice((CZUChar)nodeType))
    {//终端
        if (bServerCall)
        {
            if (CZSingletonManagerInstance.getNodeManager().getNodeStatus(requestMessage->src_addr) == E_Node_status_Fault)
            {
                CZUShort faultSite;
                eNodeType type;
                CZSingletonManagerInstance.getNodeManager().getFaultSite(requestMessage->src_addr, faultSite);
                CZSingletonManagerInstance.getNodeManager().getNodeType(faultSite, type);

                std::vector<CZUShort> sites;
                CZSingletonManagerInstance.getNodeManager().getApByType(type, sites);
                if (sites.size() < 2)
                {
                    DebugLog << "没有备用基站，流程中止";

                    return 1;
                }

                responseBuffer[0] = type - M_AP + 1;
                bufferLen = 1;

                DebugLog << "触发终端扫描质量表 基站类型：" << ShortToHexStr(type);

                sendMessage(requestMessage->session_id, requestMessage->src_addr, GetWlQty, responseBuffer, bufferLen, bNeedReply);
            }
            else
            {
                //填充Range  全部基站的信道，StartFreId、EndFreId不填充
                responseBuffer[0] = 0x06;
                bufferLen = 1;

                DebugLog << "触发终端扫描质量表 频点 全部";

                sendMessage(requestMessage->session_id, requestMessage->src_addr, GetWlQty, responseBuffer, bufferLen, bNeedReply);
            }
        }
        else
        {
            CZUChar status = requestMessage->data[0];

            if (status != 0)
            {
                if (status == 1)
                {
                    DebugLog << "终端:" << ShortToHexStr(requestMessage->src_addr) << "获取质量表 数据未准备好，需要时间扫描 添加定时任务 再次申请";
                    addTimerTask(requestMessage->src_addr, requestMessage->session_id, requestMessage->data_id, NULL, 0, 5);
                }
                else if (status == 2)
                {
                    DebugLog << "终端:" << ShortToHexStr(requestMessage->src_addr) << "当前没有终端可使用的频率";
                }
                else if (status > 2)
                {
                    DebugLog << "终端:" << ShortToHexStr(requestMessage->src_addr) << "获取质量表 不支持的状态类型";
                }

                return 1;
            }

            if (requestMessage->data_len < 4)
            {
                DebugLog << CharsToHexStr(requestMessage->data, requestMessage->data_len) << " 终端网络质量表返回参数不足4位";
                return 1;
            }

            CZUChar range = requestMessage->data[1];

            //判断range是否为0x06
            if (CZSingletonManagerInstance.getNodeManager().getNodeStatus(requestMessage->src_addr) != E_Node_status_Fault && range != 0x06)
            {
                DebugLog << CharsToHexStr(requestMessage->data, requestMessage->data_len) << " 终端网络质量表返回Range不为0x06";
                return 1;
            }

            const CZUShort &count = *(CZUShort*)(requestMessage->data + 2);

            //判断长度是否正确

            if (requestMessage->data_len != (4 + 4 * count))
            {
                DebugLog << CharsToHexStr(requestMessage->data, requestMessage->data_len) << " 终端网络质量表返回参数长度不正确";
                return 1;
            }

            eNodeType serviceNodeType = No_Care;

            CZUShort serviceNetworkAddr = 0;

            //是否含有M基站
            bool bHaveMsite = false;

            //是否含有R基站
            bool bHaveRsite = false;

            QtyInfo* qtyInfo = (QtyInfo*)(requestMessage->data + 4);

            std::sort(qtyInfo, qtyInfo + count, [](QtyInfo &a, QtyInfo &b) {
                return a.rssi > b.rssi || (a.rssi == b.rssi && a.snr < b.snr);
            });

            std::vector<CZUShort> *sitesVec;

            ////排序得到每个基站类型的最优值

            eNodeType faultType;
            CZUShort faultSite = 0;
            if (CZSingletonManagerInstance.getNodeManager().getNodeStatus(requestMessage->src_addr) == E_Node_status_Fault)
            {
                CZSingletonManagerInstance.getNodeManager().getFaultSite(requestMessage->src_addr, faultSite);
                CZSingletonManagerInstance.getNodeManager().getNodeType(faultSite, faultType);
                CZSingletonManagerInstance.getNodeManager().delCanReachSites(requestMessage->src_addr, faultType);
            }
            else
            {
                CZSingletonManagerInstance.getNodeManager().delCanReachSites(requestMessage->src_addr, M_AP);
                CZSingletonManagerInstance.getNodeManager().delCanReachSites(requestMessage->src_addr, R_AP);
            }

            for (CZInt i = 0; i < count; i++)
            {
                serviceNetworkAddr = CZSingletonManagerInstance.getNodeManager().getNetworkAddrByFreid(qtyInfo[i].freId);

                if (qtyInfo[i].rssi == 0)
                    continue;

                if (serviceNetworkAddr == 0)
                {
                    DebugLog << "终端 扫描结果 频点:" << ShortToHexStr(qtyInfo[i].freId) << " Rssi:" << (int)qtyInfo[i].rssi << " Snr:" << (int)qtyInfo[i].snr << " 未发现对应的基站信息";
                    continue;
                }
                else
                    DebugLog << "终端 扫描结果 频点:" << ShortToHexStr(qtyInfo[i].freId) << " Rssi:" << (int)qtyInfo[i].rssi << " Snr:" << (int)qtyInfo[i].snr;

                CZSingletonManagerInstance.getNodeManager().getNodeType(serviceNetworkAddr, serviceNodeType);

                if (CZSingletonManagerInstance.getNodeManager().getNodeStatus(requestMessage->src_addr) == E_Node_status_Fault && faultSite == serviceNetworkAddr)
                    continue;

                CZSingletonManagerInstance.getNodeManager().getCanReachSites(requestMessage->src_addr, serviceNodeType, sitesVec);

                if (sitesVec->size() < 2)
                {
                    DebugLog << "终端 选取 类型:" << ByteToHexStr(serviceNodeType) << " 频点:" << ShortToHexStr(qtyInfo[i].freId) << " Rssi:" << (int)qtyInfo[i].rssi << " Snr:" << (int)qtyInfo[i].snr;

                    sitesVec->push_back(serviceNetworkAddr);

                    //CZSingletonManagerInstance.getNodeManager().setCanReachSites(requestMessage->src_addr, serviceNodeType, *sitesVec);

                    if (serviceNodeType == R_AP)
                    {
                        bHaveRsite = true;

                        CZUShort groupId = 0;

                        CZUChar pos = 0;

                        bool bNotifyRSite = false;

                        CZSingletonManagerInstance.getDataHelper().addToRSiteGroup(serviceNetworkAddr, requestMessage->src_addr);
                        CZSingletonManagerInstance.getDataHelper().setRSiteInservice(serviceNetworkAddr, requestMessage->src_addr, true);

                        if (CZSingletonManagerInstance.getNodeManager().addEdToGroup(serviceNetworkAddr, requestMessage->src_addr, groupId, pos, bNotifyRSite))
                        {
                            continue;
                        }

                        DebugLog << "终端：" << ShortToHexStr(requestMessage->src_addr) << " 被分配到R基站:" << ShortToHexStr(serviceNetworkAddr) << " 组号：" << (groupId + 1) << " 位置：" << (int)pos;

                        //if (bNotifyRSite)
                        //{
                        noticeRSiteAddGroup(serviceNetworkAddr, groupId + 1);
                        //                      continue;
                                          //}
                    }
                    else if (serviceNodeType == M_AP)
                    {
                        bHaveMsite = true;

                        CZSingletonManagerInstance.getNodeManager().addEdToManagerList(serviceNetworkAddr, requestMessage->src_addr, true);
                        CZSingletonManagerInstance.getDataHelper().addToMSiteList(serviceNetworkAddr, requestMessage->src_addr, true);
                        CZSingletonManagerInstance.getDataHelper().setMSiteInservice(serviceNetworkAddr, requestMessage->src_addr, true);
                    }
                }
                else
                    if (CZSingletonManagerInstance.getNodeManager().getNodeStatus(requestMessage->src_addr) == E_Node_status_Fault)
                        break;
            }

            //网络资源表配置
            if (CZSingletonManagerInstance.getNodeManager().getNodeStatus(requestMessage->src_addr) == E_Node_status_Fault)
            {
                CZUShort faultSite;
                eNodeType faultType;
                CZSingletonManagerInstance.getNodeManager().getFaultSite(requestMessage->src_addr, faultSite);
                CZSingletonManagerInstance.getNodeManager().getNodeType(faultSite, faultType);
                if (!(faultType == M_AP && bHaveMsite) && !(faultType == R_AP && bHaveRsite))
                    DebugLog << "未找到可用备用基站！！！";
                setModifyEdNwkConfig(requestMessage);
            }
            else
                setEdNwkConfig(requestMessage);
        }
    }
    else
    {
        //基站
        if (bServerCall)
        {
            //填充Range 指定信道范围，StartFreId、EndFreId有效
            responseBuffer[0] = 0xff;
            bufferLen = 1;

            //填充StartFreId、EndFreId

            CZUShort startFreId;

            CZUShort endFreId;

            if (CZSingletonManagerInstance.getNodeManager().getFreidRange(nodeType, startFreId, endFreId))
            {
                DebugLog << "获取基站类型:" << (int)nodeType << " 频点范围失败";
                return 1;
            }

            memcpy(responseBuffer + bufferLen, &startFreId, sizeof(startFreId));
            bufferLen += sizeof(startFreId);

            memcpy(responseBuffer + bufferLen, &endFreId, sizeof(endFreId));
            bufferLen += sizeof(endFreId);

            DebugLog << "触发基站扫描质量表 频点" << startFreId << " --> " << endFreId;

            //添加到缓存
            CZSingletonManagerInstance.getNodeManager().setCache(requestMessage->src_addr, "startFreId", ShortToHexStr(startFreId));
            CZSingletonManagerInstance.getNodeManager().setCache(requestMessage->src_addr, "endFreId", ShortToHexStr(endFreId));

            sendMessage(requestMessage->session_id, requestMessage->src_addr, GetWlQty, responseBuffer, bufferLen, bNeedReply);
        }
        else
        {
            CZUChar status = requestMessage->data[0];

            if (status != 0)
            {
                if (status == 1)
                {
                    DebugLog << "基站:" << ShortToHexStr(requestMessage->src_addr) << "获取质量表 数据未准备好，需要时间扫描 添加定时任务 再次请求";
                    addTimerTask(requestMessage->src_addr, CZSingletonManagerInstance.getNodeManager().getNodeOverSessionRoad(requestMessage->src_addr), requestMessage->data_id, NULL, 0, 5);
                    //disconnect(requestMessage);
                }
                else if (status == 2)
                {
                    DebugLog << "基站:" << ShortToHexStr(requestMessage->src_addr) << "获取质量表 超出芯片支持的频率范围";
                    disconnect(requestMessage);
                }
                else if (status > 2)
                {
                    DebugLog << "基站:" << ShortToHexStr(requestMessage->src_addr) << "获取质量表 不支持的状态类型";
                    disconnect(requestMessage);
                }

                return 1;
            }

            if (requestMessage->data_len < 8)
            {
                DebugLog << CharsToHexStr(requestMessage->data, requestMessage->data_len) << " 基站网络质量表返回参数不足8位";
                return 1;
            }

            CZUChar range = requestMessage->data[1];

            //判断range是否为0xff
            if (range != 0xff)
            {
                DebugLog << CharsToHexStr(requestMessage->data, requestMessage->data_len) << " 基站网络质量表返回Range不为0xff";
                return 1;
            }

            CZUShort count = *(CZUShort*)(requestMessage->data + 2);

            //判断长度是否正确

            if (requestMessage->data_len != (4 + 4 * count))
            {
                DebugLog << CharsToHexStr(requestMessage->data, requestMessage->data_len) << " 基站网络质量表返回参数长度不正确";
                return 1;
            }

            CZUShort cacheStartFreid = 0;

            CZUShort cacheEndFreid = 0;

            std::string tmpString = "";

            CZSingletonManagerInstance.getNodeManager().getCache(requestMessage->src_addr, "startFreId", tmpString);

            if (tmpString.length() > 0)
            {
                cacheStartFreid = HexStrToShort(tmpString);
            }

            CZSingletonManagerInstance.getNodeManager().getCache(requestMessage->src_addr, "endFreId", tmpString);

            if (tmpString.length() > 0)
            {
                cacheEndFreid = HexStrToShort(tmpString);
            }

            CZUChar* pData = requestMessage->data + 4;

            std::vector<QtyInfo> vecQtyInfo;

            CZUShort freidServiceNetworkAddr = 0;

            CZUShort oldFreqId;
            if (CZSingletonManagerInstance.getNodeManager().getNodeStatus(requestMessage->src_addr) == E_Node_status_Disturb)
            {//信道干扰 需要考虑一种情况：多次获取网络质量表
                CZSingletonManagerInstance.getNodeManager().getSiteFreid(requestMessage->src_addr, oldFreqId);
                //CZSingletonManagerInstance.getNodeManager().setSiteFreid(requestMessage->src_addr, 0);
            }

            for (int i = 0; i < count; i++)
            {
                QtyInfo* qtyInfo = (QtyInfo*)pData;

                pData += (sizeof(QtyInfo));

                if (qtyInfo->freId < cacheStartFreid || qtyInfo->freId > cacheEndFreid)
                {
                    DebugLog << "基站 扫描结果 频点:" << ShortToHexStr(qtyInfo->freId) << " Rssi:" << (int)qtyInfo->rssi << " Snr:" << (int)qtyInfo->snr << " 频点不在下发扫描范围内";
                    continue;
                }

                //if(qtyInfo->freId < startFreid || qtyInfo->freId > endFreid)
                //{
                //	DebugLog<<"基站 扫描结果 频点:"<<ShortToHexStr(qtyInfo->freId)<<" Rssi:"<<(int)qtyInfo->rssi<<" Snr:"<<(int)qtyInfo->snr<<" 频点不在范围内";
                //	continue;
                //}

                if (CZSingletonManagerInstance.getNodeManager().getNodeStatus(requestMessage->src_addr) == E_Node_status_Disturb && qtyInfo->freId == oldFreqId)
                {
                    DebugLog << "基站 扫描结果 频点:" << ShortToHexStr(qtyInfo->freId) << " Rssi:" << (int)qtyInfo->rssi << " Snr:" << (int)qtyInfo->snr << " 为原基站频点";
                    continue;
                }

                freidServiceNetworkAddr = CZSingletonManagerInstance.getNodeManager().getNetworkAddrByFreid(qtyInfo->freId);

                if (freidServiceNetworkAddr != 0 && freidServiceNetworkAddr != requestMessage->src_addr)
                {
                    DebugLog << "基站 扫描结果 频点:" << ShortToHexStr(qtyInfo->freId) << " Rssi:" << (int)qtyInfo->rssi << " Snr:" << (int)qtyInfo->snr << " 频点已被" << ShortToHexStr(freidServiceNetworkAddr) << "所占用";
                    continue;
                }

                DebugLog << "基站 扫描结果 频点:" << ShortToHexStr(qtyInfo->freId) << " Rssi:" << (int)qtyInfo->rssi << " Snr:" << (int)qtyInfo->snr;

                vecQtyInfo.push_back(*qtyInfo);
            }

            if (vecQtyInfo.size() == 0)
            {
                DebugLog << "基站:" << ShortToHexStr(requestMessage->src_addr) << "获取质量表 频点一个都没有合适的 添加定时任务 再次请求";
                addTimerTask(requestMessage->src_addr, CZSingletonManagerInstance.getNodeManager().getNodeOverSessionRoad(requestMessage->src_addr), requestMessage->data_id, NULL, 0, 5);

                return 1;
            }

            std::sort(vecQtyInfo.begin(), vecQtyInfo.end(), [](QtyInfo &a, QtyInfo &b) {
                return a.rssi < b.rssi || (a.rssi == b.rssi && a.snr < b.snr);
            });

            bool bSuccChoose = false;

            for (std::vector<QtyInfo>::iterator it = vecQtyInfo.begin(); it != vecQtyInfo.end(); it++)
            {
                if (!CZSingletonManagerInstance.getNodeManager().setFreidInfo((*it).freId, requestMessage->src_addr))
                {
                    DebugLog << "基站 选取 频点:" << ShortToHexStr((*it).freId) << " Rssi:" << (int)(*it).rssi << " Snr:" << (int)(*it).snr;
                    bSuccChoose = true;
                    break;
                }
            }

            pData = NULL;

            if (bSuccChoose)
            {
                if (CZSingletonManagerInstance.getNodeManager().getNodeStatus(requestMessage->src_addr) == E_Node_status_Disturb)
                {//信道干扰
                    DebugLog << "发送跳频帧";

                    bufferLen = 6;
                    responseBuffer[0] = 5;
                    responseBuffer[1] = (CZUChar)(requestMessage->src_addr);
                    *(CZUShort*)(responseBuffer + 2) = oldFreqId;
                    CZSingletonManagerInstance.getNodeManager().getSiteFreid(requestMessage->src_addr, oldFreqId);
                    *(CZUShort*)(responseBuffer + 4) = oldFreqId;

                    //broadcastToAllSite(HopFreq, responseBuffer, bufferLen);
                    sendMessage(requestMessage->session_id, requestMessage->src_addr, HopFreq, responseBuffer, bufferLen, true);

                    nwkResource(requestMessage, true, false);
                }
                else
                {
                    //网络资源表配置
                    setApNwkConfig(requestMessage);
                }
            }
            else
            {
                DebugLog << "没有合适的频点供选择";
            }
        }
    }

    return 0;
}

int CZLogicCenter::setModifyEdNwkConfig(cz_types::RequestMessage* requestMessage, bool bServerCall, bool bNeedReply)
{
    bufferLen = 0;

    if (bServerCall)
    {
        eNodeType faultType;
        CZUShort faultSite;
        CZSingletonManagerInstance.getNodeManager().getFaultSite(requestMessage->src_addr, faultSite);
        CZSingletonManagerInstance.getNodeManager().getNodeType(faultSite, faultType);

        std::vector<CZUShort> *sitesVec;
        CZSingletonManagerInstance.getNodeManager().getCanReachSites(requestMessage->src_addr, faultType, sitesVec);

        DebugLog << "发送更新终端配置帧";
        responseBuffer[0] = faultType;
        responseBuffer[1] = 1;
        responseBuffer[2] = faultSite;
        responseBuffer[3] = sitesVec->front();

        CZUShort groupId;
        CZUChar pos;
        CZSingletonManagerInstance.getNodeManager().findEdGroupInfo(sitesVec->front(), requestMessage->src_addr, groupId, pos);

        groupId++;

        switch (faultType)
        {
        case M_AP:
            bufferLen = 4;
            break;
        case R_AP:
            bufferLen = 8;
            *(CZUShort*)(responseBuffer + 4) = groupId;
            responseBuffer[6] = pos;
            responseBuffer[7] = 0;
            break;
        }

        sendMessage(requestMessage->session_id, requestMessage->src_addr, ModifyEdNwkConfig, responseBuffer, bufferLen, bNeedReply);

    }
    else
    {
        //收到返回
        if (requestMessage->data_len != 1)
        {
            DebugLog << "未发现参数项";
            return 1;
        }

        if (requestMessage->data[0] != 0)
        {
            DebugLog << "更新终端网络配置表失败 返回码:" << ByteToHexStr(requestMessage->data[0]);
            return 1;
        }

        CZUShort faultSite;
        eNodeType type;
        CZSingletonManagerInstance.getNodeManager().getFaultSite(requestMessage->src_addr, faultSite);
        CZSingletonManagerInstance.getNodeManager().getNodeType(faultSite, type);

        switch (type)
        {
        case M_AP:
            std::vector<CZUShort> *siteVec;

            CZSingletonManagerInstance.getNodeManager().getCanReachSites(requestMessage->src_addr, M_AP, siteVec);

            noticeMSiteAddDevice((*siteVec)[0], requestMessage->src_addr, true);
            break;
        case R_AP:
            break;
        }

        CZSingletonManagerInstance.getNodeManager().setFaultSite(requestMessage->src_addr, 0);
        CZSingletonManagerInstance.getNodeManager().setNodeStatus(requestMessage->src_addr, E_Node_Status_Running);

        DebugLog << "更新终端网络配置表成功";
    }

    return 0;
}

int CZLogicCenter::setEdNwkConfig(cz_types::RequestMessage* requestMessage, bool bServerCall, bool bNeedReply)
{
    bufferLen = 0;

    if (bServerCall)
    {
        //网络ID
        CZUShort network_id = CZSingletonManagerInstance.getConfiguration().getNetworkId();
        bufferLen = 0;
        memcpy(responseBuffer + bufferLen, &network_id, sizeof(network_id));
        bufferLen += sizeof(network_id);

        std::vector<CZUShort> *vecMap;

        std::map<CZUChar, std::vector<CZUShort>>::iterator it;

        CZUChar size = 0;

        CZUShort network_addr = 0;

        CZUShort groupId = 0;

        CZUChar pos = 0;

        for (CZUChar i = M_AP; i < Multiplex_AP; i++)
        {
            //获取终端的质量信息
            CZSingletonManagerInstance.getNodeManager().getCanReachSites(requestMessage->src_addr, (eNodeType)i, vecMap);

            if (vecMap == 0)
                continue;

            //填充个数
            size = vecMap->size();

            if (size <= 0)
            {
                continue;
            }

            //添加设备类型
            responseBuffer[bufferLen] = i;
            bufferLen += 1;

            //添加设备个数
            memcpy(responseBuffer + bufferLen, &size, sizeof(size));
            bufferLen += sizeof(size);

            for (std::vector<CZUShort>::iterator vIt = vecMap->begin(); vIt != vecMap->end(); vIt++)
            {
                network_addr = (*vIt);

                if (network_addr <= 0)
                {
                    continue;
                }

                if (i == M_AP)
                {
                    responseBuffer[bufferLen] = (CZUChar)network_addr;
                    bufferLen += 1;
                }
                else if (i == R_AP)
                {
                    responseBuffer[bufferLen] = (CZUChar)network_addr;
                    bufferLen += 1;
                    //获取组号
                    CZSingletonManagerInstance.getNodeManager().findEdGroupInfo(network_addr, requestMessage->src_addr, groupId, pos);
                    groupId++;
                    memcpy(responseBuffer + bufferLen, &groupId, sizeof(groupId));
                    bufferLen += sizeof(groupId);

                    memcpy(responseBuffer + bufferLen, &pos, sizeof(pos));
                    bufferLen += sizeof(pos);

                    //mask
                    responseBuffer[bufferLen] = 0;
                    bufferLen += 1;
                }
                else
                {
                    continue;
                }
            }
        }
        sendMessage(requestMessage->session_id, requestMessage->src_addr, SetEdNwkConfig, responseBuffer, bufferLen, bNeedReply);
    }
    else
    {
        //收到返回
        if (requestMessage->data_len != 1)
        {
            DebugLog << "未发现参数项";
            return 1;
        }

        if (requestMessage->data[0] != 0)
        {
            DebugLog << "设置终端网络配置表失败 返回码:" << ByteToHexStr(requestMessage->data[0]);
            return 1;
        }

        DebugLog << "设置终端网络配置表成功";
    }

    return 0;
}

int CZLogicCenter::setApNwkConfig(cz_types::RequestMessage* requestMessage, bool bServerCall, bool bNeedReply)
{
    bufferLen = 0;

    if (bServerCall)
    {
        //网络ID
        CZUShort network_id = CZSingletonManagerInstance.getConfiguration().getNetworkId();
        bufferLen = 0;
        memcpy(responseBuffer + bufferLen, &network_id, sizeof(network_id));
        bufferLen += sizeof(network_id);

        //设备类型
        eNodeType nodeType;
        CZSingletonManagerInstance.getNodeManager().getNodeType(requestMessage->src_addr, nodeType);
        responseBuffer[bufferLen] = (CZUChar)nodeType;
        bufferLen += 1;

        //网络地址
        CZUShort network_addr = requestMessage->src_addr;
        memcpy(responseBuffer + bufferLen, &network_addr, sizeof(network_addr));
        bufferLen += sizeof(network_addr);

        //无线工作参数 WlInfo
        CZUShort freId = 0;
        CZSingletonManagerInstance.getNodeManager().getSiteFreid(network_addr, freId);
        memcpy(responseBuffer + bufferLen, &freId, sizeof(freId));
        bufferLen += sizeof(freId);

        //发送功率
        CZUChar txPower = (CZUChar)CZSingletonManagerInstance.getConfiguration().getShortFromServerConfig("site_tx_power");
        memcpy(responseBuffer + bufferLen, &txPower, sizeof(txPower));
        bufferLen += sizeof(txPower);

        sendMessage(requestMessage->session_id, requestMessage->src_addr, SetApNwkConfig, responseBuffer, bufferLen, bNeedReply);
    }
    else
    {
        //收到返回
        if (requestMessage->data_len != 1)
        {
            DebugLog << "未发现参数项";
            return 1;
        }

        if (requestMessage->data[0] != 0)
        {
            std::bitset<8> retStatus(requestMessage->data[0]);

            if (retStatus[0])
            {
                DebugLog << "设置基站网络配置表失败 DeviceInfo错误";
                disconnect(requestMessage);
            }

            if (retStatus[1])
            {
                DebugLog << "设置基站网络配置表失败 不支持的FreId";
                disconnect(requestMessage);
            }

            if (retStatus[2])
            {
                DebugLog << "设置基站网络配置表失败 不支持的设备类型";
                disconnect(requestMessage);
            }

            return 1;
        }

        CZSingletonManagerInstance.getNodeManager().setNodeStatus(requestMessage->src_addr, E_Node_Status_Running);

        setApWorkTab(requestMessage, true, true);
        //成功则发送广播网络资源表

    }

    return 0;
}

int CZLogicCenter::noticeMSiteAddDevice(CZUShort siteNetworkAddr, CZUShort edNetworkAddr, bool add)
{
    bufferLen = 0;

    //获取对应基站的信息

    eNodeType nodeType;

    CZInt iRet = CZSingletonManagerInstance.getNodeManager().getNodeType(siteNetworkAddr, nodeType);

    if (iRet)
    {
        DebugLog << "通知M基站添加/删除终端获取对应的设备类型错误";
        return 1;
    }

    if (nodeType != M_AP)
    {
        DebugLog << "通知M基站添加/删除终端目标设备类型不为M基站";
        return 1;
    }

    eNodeType ed_nodeType;

    iRet = CZSingletonManagerInstance.getNodeManager().getNodeType(edNetworkAddr, ed_nodeType);

    if (iRet)
    {
        return 1;
    }
    //操作（Option）：0为删除，1为添加
    responseBuffer[bufferLen] = add ? 1 : 0;
    bufferLen += 1;

    //要添加或删除的终端的设备类型
    responseBuffer[bufferLen] = (CZUChar)ed_nodeType;
    bufferLen += 1;

    //要添加或删除的终端的网络地址
    memcpy(responseBuffer + bufferLen, &edNetworkAddr, sizeof(edNetworkAddr));
    bufferLen += sizeof(edNetworkAddr);

    if (add)
        DebugLog << "通知M基站" << ShortToHexStr(siteNetworkAddr) << "添加终端" << ShortToHexStr(edNetworkAddr);
    else
        DebugLog << "通知M基站" << ShortToHexStr(siteNetworkAddr) << "删除终端" << ShortToHexStr(edNetworkAddr);

    CZUShort sessionId = CZSingletonManagerInstance.getNodeManager().getNodeOverSessionRoad(siteNetworkAddr);

    sendMessage(sessionId, siteNetworkAddr, ReqAddOrDelEd, responseBuffer, bufferLen, true);

    return 0;
}

int CZLogicCenter::reqAddOrDelEd(cz_types::RequestMessage* requestMessage, bool bServerCall, bool bNeedReply)
{
    bufferLen = 0;

    if (bServerCall)
    {

    }
    else
    {
        if (requestMessage->data_len == 1)
        {
            switch ((int)requestMessage->data[0])
            {
            case 0:
            {
                DebugLog << "收到通知M基站添加/删除终端返回结果:成功";
                break;
            }
            case 1:
            {
                DebugLog << "收到通知M基站添加/删除终端返回结果:无效的终端参数";
                return -1;
            }
            default:
                return -1;
            }
        }
    }

    return 0;
}

int CZLogicCenter::noticeRSiteAddGroup(CZUShort siteNetworkAddr, CZUShort groupId)
{
    bufferLen = 0;

    //获取对应基站的信息

    eNodeType nodeType;

    CZInt iRet = CZSingletonManagerInstance.getNodeManager().getNodeType(siteNetworkAddr, nodeType);

    if (iRet)
    {
        DebugLog << "通知R基站添加/删除分组获取对应的设备类型错误";
        return 1;
    }

    if (nodeType != R_AP)
    {
        DebugLog << "通知R基站添加/删除分组目标设备类型不为M基站";
        return 1;
    }

    //操作（Option）：0为删除，1为添加
    responseBuffer[bufferLen] = 1;
    bufferLen += 1;

    //要添加或删除的终端的网络地址
    memcpy(responseBuffer + bufferLen, &groupId, sizeof(groupId));
    bufferLen += sizeof(groupId);

    DebugLog << "通知R基站" << ShortToHexStr(siteNetworkAddr) << "添加分组" << groupId;

    CZUShort sessionId = CZSingletonManagerInstance.getNodeManager().getNodeOverSessionRoad(siteNetworkAddr);

    sendMessage(sessionId, siteNetworkAddr, ReqAddOrDelGroup, responseBuffer, bufferLen, true);

    return 0;
}

int CZLogicCenter::test(cz_types::RequestMessage* requestMessage, bool bServerCall, bool bNeedReply)
{
    DebugLog << "test run.............";
    return 0;
}

int CZLogicCenter::reqAddOrDelGroup(cz_types::RequestMessage* requestMessage, bool bServerCall, bool bNeedReply)
{
    bufferLen = 0;

    if (bServerCall)
    {
        DebugLog << "reqAddOrDelGroup bServerCall";
    }
    else
    {
        if (requestMessage->data_len == 1)
        {
            switch ((int)requestMessage->data[0])
            {
            case 0:
            {
                DebugLog << "收到通知R基站添加或删除一个组返回结果:成功";
                break;
            }
            case 1:
            {
                DebugLog << "收到通知R基站添加或删除一个组返回结果:添加组时组号小于或等于目前基站组号";
                break;
            }
            case 2:
            {
                DebugLog << "收到通知R基站添加或删除一个组返回结果:删除组时组号大于目前基站组号";
                break;
            }
            default:
                break;
            }
        }
    }

    return 0;
}

/*********************主动触发end***********************/

int CZLogicCenter::addTimerTask(CZUShort dst_network_addr, CZUShort session_id, CZUChar data_id, CZUChar* data_buffer, CZUShort data_len, CZUShort seconds)
{
    task->dst_addr = dst_network_addr;
    task->session_id = session_id;
    task->type = E_Task_Normal_Timer;
    task->millisecond = CZMiscs::getMillisecond() + (seconds * 1000);
    task->data_id = data_id;
    task->isAddTask = true;
    task->data_len = data_len;

    if (data_len > 0)
    {
        memcpy(task->data, data_buffer, data_len);
    }

    CZSingletonManagerInstance.getMessagePool().addTimerTask(task);

    return 0;
}

int CZLogicCenter::sendMessage(cz_types::RequestMessage* requestMessage, CZUChar* data_buffer, CZUShort data_len, bool bNeedReply)
{
    return sendMessage(requestMessage->session_id, requestMessage->src_addr, requestMessage->data_id, data_buffer, data_len, bNeedReply);
}

int CZLogicCenter::sendMessage(CZUShort sessionId, CZUShort dst_network_addr, CZUChar data_id, CZUChar* data_buffer, CZUShort data_len, bool bNeedReply)
{
    CZSingletonManagerInstance.getMessagePool().addResponseMessage(sessionId, dst_network_addr, data_id, data_buffer, data_len, bNeedReply);
    return 0;
}

int CZLogicCenter::broadcastToAllSite(CZUChar data_id, CZUChar* data_buffer, CZUShort data_len, bool bNeedReply)
{
    std::vector<CZUShort> sitesVec;

    for (CZUChar type = M_AP; type < Multiplex_AP; type++)
    {
        CZSingletonManagerInstance.getNodeManager().getApByType((eNodeType)type, sitesVec);

        for (std::vector<CZUShort>::iterator it = sitesVec.begin(); it != sitesVec.end(); it++)
        {
            DebugLog << "广播给基站:" << ShortToHexStr(*it) << " 类型:" << ByteToHexStr(type);
            sendMessage(0, *it, data_id, data_buffer, data_len, bNeedReply);
        }
    }
    return 0;
}