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
    //�����߼�������
    registerEntry(EdReqJoinNwk, ENTRY_TYPE_BIND(&CZLogicCenter::edReqJoinNetwork));
    registerEntry(ApReqJoinNwk, ENTRY_TYPE_BIND(&CZLogicCenter::apReqJoinNetwork));
    registerEntry(GetSysTime, ENTRY_TYPE_BIND(&CZLogicCenter::getSysTime));
    registerEntry(EdActive, ENTRY_TYPE_BIND(&CZLogicCenter::edActive));
    registerEntry(Disconnect, ENTRY_TYPE_BIND(&CZLogicCenter::disconnectAck));
    registerEntry(Fault, ENTRY_TYPE_BIND(&CZLogicCenter::faultReport));
    registerEntry(GetDeviceInfo, ENTRY_TYPE_BIND(&CZLogicCenter::getDeviceInfo));

    //�����ش��߼�������
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

    //�ж����������data_id �Ķ�Ӧ�Ƿ���ȷ

    //������������ַ��dataidУ��
    if (requestMessage->src_addr == JOIN_ED_NETWORK_ADDR && requestMessage->data_id != EdReqJoinNwk)
    {
        DebugLog << "�ն�����������ַ�����Բ�������ҵ��,data_id:" << ByteToHexStr(requestMessage->data_id);
        return 1;
    }

    if (requestMessage->src_addr == JOIN_SITE_NETWORK_ADDR && requestMessage->data_id != ApReqJoinNwk)
    {
        DebugLog << "��վ����������ַ�����Բ�������ҵ��,data_id:" << ByteToHexStr(requestMessage->data_id);
        return 1;
    }

    if (requestMessage->src_addr != JOIN_ED_NETWORK_ADDR && requestMessage->data_id == EdReqJoinNwk)
    {
        DebugLog << "�ն�������������ID�������ַ��ƥ��";
        return 1;
    }

    if (requestMessage->src_addr != JOIN_SITE_NETWORK_ADDR && requestMessage->data_id == ApReqJoinNwk)
    {
        DebugLog << "��վ������������ID�������ַ��ƥ��";
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
                DebugLog << "��������δ������ " << ShortToHexStr(requestMessage->src_addr) << " ���� ����ID:$i:" << ByteToHexStr(requestMessage->data_id) << "$ �������ݣ�" <<
                    CharsToHexStr(requestMessage->data, requestMessage->data_len) << " �ж�Ϊ�Ƿ����� �ܾ���Ӧ";
                return 1;
            }

            //ɾ���ش�����
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
                DebugLog << "��ȡ�豸��Ϣ " << requestMessage->session_id;
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
    DebugLog << "�豸��Ϣ��Ӧ֡ " << requestMessage->src_addr;
    if (requestMessage->data_len != 13)
    {
        DebugLog << "���Ȳ�����13";
        return -1;
    }

    if (*(CZUShort*)(requestMessage->data + 8) == 0)
    {
        DebugLog << "�����ַ����Ϊ��";
        return -1;
    }

    CZSingletonManagerInstance.getSessionManager().updateSiteInfo(requestMessage->session_id, *(CZUShort*)(requestMessage->data + 8));

    return 0;
}

int CZLogicCenter::setApWorkTab(cz_types::RequestMessage* requestMessage, bool bServerCall, bool bNeedReply)
{
    if (bServerCall)
    {
        DebugLog << "���������������";

        bufferLen = 0;

        eNodeType nodeType;

        CZInt iRet = CZSingletonManagerInstance.getNodeManager().getNodeType(requestMessage->src_addr, nodeType);

        if (iRet)
        {
            DebugLog << "δ�ҵ���Ӧ�Ľڵ�����";
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

        DebugLog << "���͹��������ɹ�";
    }
    else
    {
        DebugLog << "�յ������������Ӧ";

        //�ɹ����͹㲥������Դ��
        DebugLog << "���û�վ�������ñ�ɹ� �㲥������Դ��";
        nwkResource(requestMessage, true, false);
    }
    return 0;
}

int CZLogicCenter::edReqJoinNetwork(cz_types::RequestMessage* requestMessage)
{
    bufferLen = 0;

    //�ն���������
    if (requestMessage->data_len != 9)
    {
        DebugLog << CharsToHexStr(requestMessage->data, requestMessage->data_len) << " ��������������Ϊ9���ֽ�";
        return 1;
    }

    //�豸����

    CZUChar cDeviceType = requestMessage->data[8];

    //�ն������ַ
    CZPhysicalAddr phyAddr(requestMessage->data);

    DebugLog << "�յ��ն�" << phyAddr << " �������� �豸����:" << ByteToHexStr(cDeviceType);

    //���ڸ��ն˵����� ����

    //״̬
    CZUChar status = 0;

    if (phyAddr.empty())
    {
        DebugLog << "�ն�δ��ʼ��������ϵ����֧��";

        status = 2;

        //��������ַ
        memcpy(responseBuffer, requestMessage->data, requestMessage->data_len);
        bufferLen = requestMessage->data_len;

        //���״̬��
        memcpy(responseBuffer + bufferLen, &status, sizeof(status));
        bufferLen += sizeof(status);

        //���ظ��ն�
        sendMessage(requestMessage, responseBuffer, bufferLen);

        return 0;
    }

    //�鿴֮ǰ�Ƿ��������ַ
    CZUShort network_addr = 0;

    bool bSucc = true;

    //�ж��Ƿ�Ϊ֧�ֵ��豸����
    if (!CZMiscs::isDevice(cDeviceType))
    {
        status = 0x04;
        network_addr = 0;
        bSucc = false;
        DebugLog << phyAddr << " ������������ʧ�ܣ��豸���ʹ���";
    }

    //����ն������ַ�Ƿ�Ϊģ��Ļ�վ��ַ

    eNodeType siteType;
    bool bSiteConfig = CZSingletonManagerInstance.getNodeManager().getSiteIdentity(phyAddr, siteType);

    if (bSiteConfig)
    {
        status = 0x02;
        DebugLog << "�ն������ַ�����Ϊ��վ��ַ,�Է�������룬��ֹ����";
        bSucc = false;
    }

    if (bSucc)
    {
        if (CZSingletonManagerInstance.getConfiguration().isNetworkOpen())
        {
            network_addr = CZSingletonManagerInstance.getNodeManager().getNetworkAddrByPhyAddr(phyAddr);

            if (network_addr > 0)
            {
                DebugLog << phyAddr << " ������������ɹ�,�ҵ�֮ǰ������������ַ:" << ShortToHexStr(network_addr);
            }
            else
            {
                network_addr = CZSingletonManagerInstance.getNodeManager().createNode(phyAddr, (eNodeType)cDeviceType, 0);
                DebugLog << phyAddr << " ������������ɹ�,���������ַ:" << ShortToHexStr(network_addr);
            }

            bSucc = true;
        }
        else
        {
            bSucc = false;
            //�����ѹر�
            status = 0x01;

            DebugLog << phyAddr << " ������������ʧ��,�����ѹر� ��������������";
        }
    }

    CZSingletonManagerInstance.getNodeManager().setNodeStatus(network_addr, E_Node_Status_Ready);
    CZSingletonManagerInstance.getNodeManager().setFaultSite(network_addr, 0);

    if (bSucc)
    {
        //��������ַ
        memcpy(responseBuffer, requestMessage->data, requestMessage->data_len - 1);
        bufferLen = requestMessage->data_len - 1;

        //���״̬��
        memcpy(responseBuffer + bufferLen, &status, sizeof(status));
        bufferLen += sizeof(status);

        //��������ַ
        memcpy(responseBuffer + bufferLen, &network_addr, sizeof(network_addr));
        bufferLen += sizeof(network_addr);

        //���ظ��ն�
        sendMessage(requestMessage, responseBuffer, bufferLen);

        requestMessage->src_addr = network_addr;

        //�ն�����������Ҫִ������������
        DebugLog << "ִ����������������";
        //û�� ����������������
        getWlQty(requestMessage);
    }
    else
    {
        //��������ַ
        memcpy(responseBuffer, requestMessage->data, requestMessage->data_len);
        bufferLen = requestMessage->data_len;

        //���״̬��
        memcpy(responseBuffer + bufferLen, &status, sizeof(status));
        bufferLen += sizeof(status);

        //���ظ��ն�
        sendMessage(requestMessage, responseBuffer, bufferLen);
    }

    return 0;
}

int CZLogicCenter::apReqJoinNetwork(cz_types::RequestMessage* requestMessage)
{
    bufferLen = 0;

    //��վ��������
    if (requestMessage->data_len != 8)
    {
        DebugLog << CharsToHexStr(requestMessage->data, requestMessage->data_len) << " ��������������Ϊ8λ";
        return 1;
    }

    //��վ�����ַ
    CZPhysicalAddr phyAddr(requestMessage->data);

    DebugLog << "�յ���վ" << phyAddr << " ��������";

    //����״̬��
    CZUChar status = 0;

    //�����ַ
    CZUShort network_addr = 0;

    bool bSucc = false;

    //��վ���
    eNodeType nodeType;

    bool bSiteConfig = CZSingletonManagerInstance.getNodeManager().getSiteIdentity(phyAddr, nodeType);

    if (bSiteConfig)
    {
        //���ڸû�վ������ ����

        //�鿴֮ǰ�Ƿ��������ַ
        network_addr = CZSingletonManagerInstance.getNodeManager().getNetworkAddrByPhyAddr(phyAddr);

        if (network_addr > 0)
        {
            eNodeType oldNodeType;

            CZSingletonManagerInstance.getNodeManager().getNodeType(network_addr, oldNodeType);

            if (oldNodeType == nodeType)
            {
                DebugLog << phyAddr << " ������������ɹ�,�ҵ�֮ǰ������������ַ:" << ShortToHexStr(network_addr);
                bSucc = true;
            }
            else
            {
                DebugLog << phyAddr << " ������������ʧ��,�ҵ�֮ǰ������������ַ:" << ShortToHexStr(network_addr) << " �������Ͳ�ƥ�� ����ϵ����Ա�Ų���";
                bSucc = false;
                status = 0x04;
            }
        }
        else
        {
            network_addr = CZSingletonManagerInstance.getNodeManager().createNode(phyAddr, nodeType, 0);
            DebugLog << phyAddr << " ������������ɹ�,���������ַ:" << ShortToHexStr(network_addr);
            bSucc = true;
        }
    }
    else
    {
        //��վδ����
        status = 0x02;
        DebugLog << phyAddr << " ������������ʧ��,δ���л�վ����";
    }

    if (bSucc)
    {
        //��������ַ
        memcpy(responseBuffer, requestMessage->data, requestMessage->data_len);
        bufferLen = requestMessage->data_len;

        //���״̬��
        memcpy(responseBuffer + bufferLen, &status, sizeof(status));
        bufferLen += sizeof(status);

        //��������ַ
        memcpy(responseBuffer + bufferLen, &network_addr, sizeof(network_addr));
        bufferLen += sizeof(network_addr);

        //���ظ���վ
        sendMessage(requestMessage, responseBuffer, bufferLen);

        //�жϻ�վ�Ƿ���Ƶ����Ϣ
        CZUShort freId = 0;

        CZInt iRet = CZSingletonManagerInstance.getNodeManager().getSiteFreid(network_addr, freId);

        requestMessage->src_addr = network_addr;

        //������·���վ��ϵ
        CZSingletonManagerInstance.getSessionManager().updateSiteInfo(requestMessage->session_id, network_addr);
    }
    else
    {
        //��������ַ
        memcpy(responseBuffer, requestMessage->data, requestMessage->data_len);
        bufferLen = requestMessage->data_len;

        //���״̬��
        memcpy(responseBuffer + bufferLen, &status, sizeof(status));
        bufferLen += sizeof(status);

        //���ظ���վ
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

    //���ʱ��
    memcpy(responseBuffer, &time, sizeof(time));
    bufferLen = sizeof(time);

    sendMessage(requestMessage, responseBuffer, bufferLen);
    CZUShort freId = 0;

    CZUShort na = requestMessage->src_addr;
    CZInt iRet = CZSingletonManagerInstance.getNodeManager().getSiteFreid(na, freId);

    if (iRet)
    {
        DebugLog << "ִ����������������";
        //û�� ����������������
        getWlQty(requestMessage);
    }
    else
    {
        DebugLog << "����������Ƶ��:" << freId << "�ϣ��·���վ�������ñ�";
        //�� �·���վ�������ñ�
        setApNwkConfig(requestMessage);
    }
    return 0;
}

int CZLogicCenter::edActive(cz_types::RequestMessage* requestMessage)
{
    bufferLen = 0;

    //�ж��ϱ������

    eNodeType nodeType;

    if (CZSingletonManagerInstance.getNodeManager().getNodeType(requestMessage->src_addr, nodeType))
    {
        return 1;
    }

    if (!CZMiscs::isDevice((CZUChar)nodeType))
    {
        return 1;
    }

    //�״����� ��Ҫ֪ͨ��Ӧ��M��վ����ն�

    std::vector<CZUShort> *msiteVec;

    CZSingletonManagerInstance.getNodeManager().getCanReachSites(requestMessage->src_addr, M_AP, msiteVec);

    if (msiteVec == 0 || msiteVec->size() < 1)
    {
        DebugLog << "�����쳣 �����ַΪ" << ShortToHexStr(requestMessage->src_addr) << "���ն˲�δ���ָ��е�M��վ";
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
        DebugLog << "������Ƶ֡";

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
        DebugLog << "�յ���Ƶ֡��Ӧ";

        return nwkResource(requestMessage, true, false);
    }
}

int CZLogicCenter::disconnectAck(cz_types::RequestMessage* requestMessage)
{
    if (requestMessage->data_len < 2)
    {
        DebugLog << "�յ�֪ͨ��վ" << ShortToHexStr(requestMessage->src_addr) << "�Ͽ����ӷ���";
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
        DebugLog << "���վ" << ShortToHexStr(requestMessage->src_addr) << "����Ͽ���������";
        sendMessage(requestMessage->session_id, requestMessage->src_addr, Disconnect, responseBuffer, bufferLen, bNeedReply);
    }
    else
    {
        if (requestMessage->data_len == 0)
        {
            DebugLog << "�յ�֪ͨ��վ" << ShortToHexStr(requestMessage->src_addr) << "�Ͽ����ӷ���";
        }
        else if (requestMessage->data_len == 4)
        {
            bufferLen = 0;
            DebugLog << "�յ�֪ͨ��վ" << ShortToHexStr(requestMessage->src_addr) << "�Ͽ���������";
            sendMessage(requestMessage->session_id, requestMessage->src_addr, Disconnect, responseBuffer, bufferLen, false);
        }
    }

    return 0;
}

int CZLogicCenter::faultReport(cz_types::RequestMessage* requestMessage)
{//ֻ���ܽ��յ����߹���֡
    if (requestMessage->data_len == 0)
    {
        DebugLog << "�յ�����֡���ȴ��� " << ShortToHexStr(requestMessage->data_len);
        return 1;
    }

    bufferLen = 2;
    responseBuffer[0] = requestMessage->data[0];
    responseBuffer[1] = 0;
    sendMessage(requestMessage->session_id, requestMessage->src_addr, Fault, responseBuffer, bufferLen);

    switch (requestMessage->data[0] & 0x7f)
    {
    case AP_WChnDisturb:
        //�ŵ�����
        DebugLog << "�յ���վ�ŵ����Ź���֡" << ShortToHexStr(requestMessage->src_addr);

        CZSingletonManagerInstance.getNodeManager().setNodeStatus(requestMessage->src_addr, E_Node_status_Disturb);

        DebugLog << "�����վƵ��";

        DebugLog << "ɨ������������Ϣ";
        getWlQty(requestMessage);

        DebugLog << "�㲥�ŵ�������Ϣ";
        DebugLog << "�㲥������Դ֡";
        break;
    case AP_WHardFault:
        //��վ����Ӳ������
        DebugLog << "�յ���վ����Ӳ������֡" << ShortToHexStr(requestMessage->src_addr);
        DebugLog << "���øû�վΪ�����ã��������ñ��û�վʱ��Ӧ���ǻ�վ�޸����⣬�������Ƿ���Ҫ��������Ϣ��";

        CZSingletonManagerInstance.getNodeManager().setNodeStatus(requestMessage->src_addr, E_Node_status_Fault);

        DebugLog << "�����ն˻������ϱ�������·����֡�������ɺ���֡��Ӧ��������";
        break;
    case AP_LLinkFault:
        //��վ������·����
        break;
    case AP_WLinkFault:
        //��վ������·����
        DebugLog << "�յ���վ������·����֡";

        if ((requestMessage->data[0] & 0x7f) == requestMessage->data[0])
        {
            CZSingletonManagerInstance.getNodeManager().setNodeStatus(requestMessage->src_addr, E_Node_status_Fault);
            DebugLog << "���û�վ" << ShortToHexStr(requestMessage->src_addr) << "������";
        }
        else
        {
            CZSingletonManagerInstance.getNodeManager().setNodeStatus(requestMessage->src_addr, E_Node_Status_Running);
            DebugLog << "�ָ���վ" << ShortToHexStr(requestMessage->src_addr) << "����";
        }

        break;
    case ED_WLinkFault:
        //�ն�������·���ϣ���վ����
        DebugLog << "�յ�������·����֡";

        if (requestMessage->data[0] == 0)
        {
            DebugLog << "��Ӧ�ó��������������վӦ�ϱ��ն˶�ʧ����֡";
            return 1;
        }
        else
        {
            DebugLog << "��վ" << ByteToHexStr(requestMessage->data[1]) << "�������֡";

            DebugLog << "���ն�" << ShortToHexStr(requestMessage->src_addr) << "�ظ�����֡��Ӧ";
            requestMessage->session_id = CZSingletonManagerInstance.getNodeManager().getNodePool()[requestMessage->data[2]]->getSessionId();
            CZSingletonManagerInstance.getNodeManager().setNodeStatus(requestMessage->src_addr, E_Node_status_Fault);
            CZSingletonManagerInstance.getNodeManager().setFaultSite(requestMessage->src_addr, requestMessage->data[1]);

            DebugLog << "�����ն˻�ȡ����������";
            getWlQty(requestMessage);

            //std::vector<CZUShort> *msiteVec;
         //   CZSingletonManagerInstance.getNodeManager().getCanReachSites(requestMessage->src_addr, M_AP, msiteVec);
         //   msiteVec->front() = requestMessage->data[2];
        }

        break;
    case AP_EdLost:
        //�ն˶�ʧ
        DebugLog << "�յ��ն˶�ʧ����֡ " << ShortToHexStr(requestMessage->src_addr);

        std::shared_ptr<CZNode> node = CZSingletonManagerInstance.getNodeManager().getNodeByNwkAddr(*(CZUShort*)(requestMessage->data + 1));

        if (node->getStatus() == E_Node_Status_Ready)
            return 0;

        node->setStatus(E_Node_status_Fault);
        DebugLog << "��Ƕ�ʧ�����ǿ������ڻ�վ���𣬺��������ն�������·����֡�ϱ�";
        CZSingletonManagerInstance.getNodeManager().addEdToManagerList(requestMessage->src_addr, *(CZUShort*)(requestMessage->data + 1), false);
        CZSingletonManagerInstance.getDataHelper().addToMSiteList(requestMessage->src_addr, *(CZUShort*)(requestMessage->data + 1), false);
        CZSingletonManagerInstance.getDataHelper().setMSiteInservice(requestMessage->src_addr, *(CZUShort*)(requestMessage->data + 1), false);
        noticeMSiteAddDevice(requestMessage->src_addr, *(CZUShort*)(requestMessage->data + 1), false);
        break;
    }

    return 0;
}

/*********************��������***********************/

int CZLogicCenter::nwkResource(cz_types::RequestMessage *requestMessage, bool bServerCall, bool bNeedReply)
{
    bufferLen = 0;

    if (!bServerCall)
    {
        return 1;
    }

    bNeedReply = false;

    //����ID
    CZUShort network_id = CZSingletonManagerInstance.getConfiguration().getNetworkId();
    bufferLen = 0;
    memcpy(responseBuffer + bufferLen, &network_id, sizeof(network_id));
    bufferLen += sizeof(network_id);

    //���翪��״̬
    CZUChar network_status = CZSingletonManagerInstance.getConfiguration().isNetworkOpen() ? 1 : 0;
    memcpy(responseBuffer + bufferLen, &network_status, sizeof(network_status));
    bufferLen += sizeof(network_status);

    //��վ��Ŀ
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
            //��վ�����ַshort Э��Ҫ���дΪchar
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

    //�ж��Ƿ�Ϊ��վ

    eNodeType nodeType;

    CZInt iRet = CZSingletonManagerInstance.getNodeManager().getNodeType(requestMessage->src_addr, nodeType);

    if (iRet || nodeType < M_AP)
    {
        return 1;
    }

    if (CZMiscs::isDevice((CZUChar)nodeType))
    {//�ն�
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
                    DebugLog << "û�б��û�վ��������ֹ";

                    return 1;
                }

                responseBuffer[0] = type - M_AP + 1;
                bufferLen = 1;

                DebugLog << "�����ն�ɨ�������� ��վ���ͣ�" << ShortToHexStr(type);

                sendMessage(requestMessage->session_id, requestMessage->src_addr, GetWlQty, responseBuffer, bufferLen, bNeedReply);
            }
            else
            {
                //���Range  ȫ����վ���ŵ���StartFreId��EndFreId�����
                responseBuffer[0] = 0x06;
                bufferLen = 1;

                DebugLog << "�����ն�ɨ�������� Ƶ�� ȫ��";

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
                    DebugLog << "�ն�:" << ShortToHexStr(requestMessage->src_addr) << "��ȡ������ ����δ׼���ã���Ҫʱ��ɨ�� ��Ӷ�ʱ���� �ٴ�����";
                    addTimerTask(requestMessage->src_addr, requestMessage->session_id, requestMessage->data_id, NULL, 0, 5);
                }
                else if (status == 2)
                {
                    DebugLog << "�ն�:" << ShortToHexStr(requestMessage->src_addr) << "��ǰû���ն˿�ʹ�õ�Ƶ��";
                }
                else if (status > 2)
                {
                    DebugLog << "�ն�:" << ShortToHexStr(requestMessage->src_addr) << "��ȡ������ ��֧�ֵ�״̬����";
                }

                return 1;
            }

            if (requestMessage->data_len < 4)
            {
                DebugLog << CharsToHexStr(requestMessage->data, requestMessage->data_len) << " �ն������������ز�������4λ";
                return 1;
            }

            CZUChar range = requestMessage->data[1];

            //�ж�range�Ƿ�Ϊ0x06
            if (CZSingletonManagerInstance.getNodeManager().getNodeStatus(requestMessage->src_addr) != E_Node_status_Fault && range != 0x06)
            {
                DebugLog << CharsToHexStr(requestMessage->data, requestMessage->data_len) << " �ն�������������Range��Ϊ0x06";
                return 1;
            }

            const CZUShort &count = *(CZUShort*)(requestMessage->data + 2);

            //�жϳ����Ƿ���ȷ

            if (requestMessage->data_len != (4 + 4 * count))
            {
                DebugLog << CharsToHexStr(requestMessage->data, requestMessage->data_len) << " �ն������������ز������Ȳ���ȷ";
                return 1;
            }

            eNodeType serviceNodeType = No_Care;

            CZUShort serviceNetworkAddr = 0;

            //�Ƿ���M��վ
            bool bHaveMsite = false;

            //�Ƿ���R��վ
            bool bHaveRsite = false;

            QtyInfo* qtyInfo = (QtyInfo*)(requestMessage->data + 4);

            std::sort(qtyInfo, qtyInfo + count, [](QtyInfo &a, QtyInfo &b) {
                return a.rssi > b.rssi || (a.rssi == b.rssi && a.snr < b.snr);
            });

            std::vector<CZUShort> *sitesVec;

            ////����õ�ÿ����վ���͵�����ֵ

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
                    DebugLog << "�ն� ɨ���� Ƶ��:" << ShortToHexStr(qtyInfo[i].freId) << " Rssi:" << (int)qtyInfo[i].rssi << " Snr:" << (int)qtyInfo[i].snr << " δ���ֶ�Ӧ�Ļ�վ��Ϣ";
                    continue;
                }
                else
                    DebugLog << "�ն� ɨ���� Ƶ��:" << ShortToHexStr(qtyInfo[i].freId) << " Rssi:" << (int)qtyInfo[i].rssi << " Snr:" << (int)qtyInfo[i].snr;

                CZSingletonManagerInstance.getNodeManager().getNodeType(serviceNetworkAddr, serviceNodeType);

                if (CZSingletonManagerInstance.getNodeManager().getNodeStatus(requestMessage->src_addr) == E_Node_status_Fault && faultSite == serviceNetworkAddr)
                    continue;

                CZSingletonManagerInstance.getNodeManager().getCanReachSites(requestMessage->src_addr, serviceNodeType, sitesVec);

                if (sitesVec->size() < 2)
                {
                    DebugLog << "�ն� ѡȡ ����:" << ByteToHexStr(serviceNodeType) << " Ƶ��:" << ShortToHexStr(qtyInfo[i].freId) << " Rssi:" << (int)qtyInfo[i].rssi << " Snr:" << (int)qtyInfo[i].snr;

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

                        DebugLog << "�նˣ�" << ShortToHexStr(requestMessage->src_addr) << " �����䵽R��վ:" << ShortToHexStr(serviceNetworkAddr) << " ��ţ�" << (groupId + 1) << " λ�ã�" << (int)pos;

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

            //������Դ������
            if (CZSingletonManagerInstance.getNodeManager().getNodeStatus(requestMessage->src_addr) == E_Node_status_Fault)
            {
                CZUShort faultSite;
                eNodeType faultType;
                CZSingletonManagerInstance.getNodeManager().getFaultSite(requestMessage->src_addr, faultSite);
                CZSingletonManagerInstance.getNodeManager().getNodeType(faultSite, faultType);
                if (!(faultType == M_AP && bHaveMsite) && !(faultType == R_AP && bHaveRsite))
                    DebugLog << "δ�ҵ����ñ��û�վ������";
                setModifyEdNwkConfig(requestMessage);
            }
            else
                setEdNwkConfig(requestMessage);
        }
    }
    else
    {
        //��վ
        if (bServerCall)
        {
            //���Range ָ���ŵ���Χ��StartFreId��EndFreId��Ч
            responseBuffer[0] = 0xff;
            bufferLen = 1;

            //���StartFreId��EndFreId

            CZUShort startFreId;

            CZUShort endFreId;

            if (CZSingletonManagerInstance.getNodeManager().getFreidRange(nodeType, startFreId, endFreId))
            {
                DebugLog << "��ȡ��վ����:" << (int)nodeType << " Ƶ�㷶Χʧ��";
                return 1;
            }

            memcpy(responseBuffer + bufferLen, &startFreId, sizeof(startFreId));
            bufferLen += sizeof(startFreId);

            memcpy(responseBuffer + bufferLen, &endFreId, sizeof(endFreId));
            bufferLen += sizeof(endFreId);

            DebugLog << "������վɨ�������� Ƶ��" << startFreId << " --> " << endFreId;

            //��ӵ�����
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
                    DebugLog << "��վ:" << ShortToHexStr(requestMessage->src_addr) << "��ȡ������ ����δ׼���ã���Ҫʱ��ɨ�� ��Ӷ�ʱ���� �ٴ�����";
                    addTimerTask(requestMessage->src_addr, CZSingletonManagerInstance.getNodeManager().getNodeOverSessionRoad(requestMessage->src_addr), requestMessage->data_id, NULL, 0, 5);
                    //disconnect(requestMessage);
                }
                else if (status == 2)
                {
                    DebugLog << "��վ:" << ShortToHexStr(requestMessage->src_addr) << "��ȡ������ ����оƬ֧�ֵ�Ƶ�ʷ�Χ";
                    disconnect(requestMessage);
                }
                else if (status > 2)
                {
                    DebugLog << "��վ:" << ShortToHexStr(requestMessage->src_addr) << "��ȡ������ ��֧�ֵ�״̬����";
                    disconnect(requestMessage);
                }

                return 1;
            }

            if (requestMessage->data_len < 8)
            {
                DebugLog << CharsToHexStr(requestMessage->data, requestMessage->data_len) << " ��վ�����������ز�������8λ";
                return 1;
            }

            CZUChar range = requestMessage->data[1];

            //�ж�range�Ƿ�Ϊ0xff
            if (range != 0xff)
            {
                DebugLog << CharsToHexStr(requestMessage->data, requestMessage->data_len) << " ��վ������������Range��Ϊ0xff";
                return 1;
            }

            CZUShort count = *(CZUShort*)(requestMessage->data + 2);

            //�жϳ����Ƿ���ȷ

            if (requestMessage->data_len != (4 + 4 * count))
            {
                DebugLog << CharsToHexStr(requestMessage->data, requestMessage->data_len) << " ��վ�����������ز������Ȳ���ȷ";
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
            {//�ŵ����� ��Ҫ����һ���������λ�ȡ����������
                CZSingletonManagerInstance.getNodeManager().getSiteFreid(requestMessage->src_addr, oldFreqId);
                //CZSingletonManagerInstance.getNodeManager().setSiteFreid(requestMessage->src_addr, 0);
            }

            for (int i = 0; i < count; i++)
            {
                QtyInfo* qtyInfo = (QtyInfo*)pData;

                pData += (sizeof(QtyInfo));

                if (qtyInfo->freId < cacheStartFreid || qtyInfo->freId > cacheEndFreid)
                {
                    DebugLog << "��վ ɨ���� Ƶ��:" << ShortToHexStr(qtyInfo->freId) << " Rssi:" << (int)qtyInfo->rssi << " Snr:" << (int)qtyInfo->snr << " Ƶ�㲻���·�ɨ�跶Χ��";
                    continue;
                }

                //if(qtyInfo->freId < startFreid || qtyInfo->freId > endFreid)
                //{
                //	DebugLog<<"��վ ɨ���� Ƶ��:"<<ShortToHexStr(qtyInfo->freId)<<" Rssi:"<<(int)qtyInfo->rssi<<" Snr:"<<(int)qtyInfo->snr<<" Ƶ�㲻�ڷ�Χ��";
                //	continue;
                //}

                if (CZSingletonManagerInstance.getNodeManager().getNodeStatus(requestMessage->src_addr) == E_Node_status_Disturb && qtyInfo->freId == oldFreqId)
                {
                    DebugLog << "��վ ɨ���� Ƶ��:" << ShortToHexStr(qtyInfo->freId) << " Rssi:" << (int)qtyInfo->rssi << " Snr:" << (int)qtyInfo->snr << " Ϊԭ��վƵ��";
                    continue;
                }

                freidServiceNetworkAddr = CZSingletonManagerInstance.getNodeManager().getNetworkAddrByFreid(qtyInfo->freId);

                if (freidServiceNetworkAddr != 0 && freidServiceNetworkAddr != requestMessage->src_addr)
                {
                    DebugLog << "��վ ɨ���� Ƶ��:" << ShortToHexStr(qtyInfo->freId) << " Rssi:" << (int)qtyInfo->rssi << " Snr:" << (int)qtyInfo->snr << " Ƶ���ѱ�" << ShortToHexStr(freidServiceNetworkAddr) << "��ռ��";
                    continue;
                }

                DebugLog << "��վ ɨ���� Ƶ��:" << ShortToHexStr(qtyInfo->freId) << " Rssi:" << (int)qtyInfo->rssi << " Snr:" << (int)qtyInfo->snr;

                vecQtyInfo.push_back(*qtyInfo);
            }

            if (vecQtyInfo.size() == 0)
            {
                DebugLog << "��վ:" << ShortToHexStr(requestMessage->src_addr) << "��ȡ������ Ƶ��һ����û�к��ʵ� ��Ӷ�ʱ���� �ٴ�����";
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
                    DebugLog << "��վ ѡȡ Ƶ��:" << ShortToHexStr((*it).freId) << " Rssi:" << (int)(*it).rssi << " Snr:" << (int)(*it).snr;
                    bSuccChoose = true;
                    break;
                }
            }

            pData = NULL;

            if (bSuccChoose)
            {
                if (CZSingletonManagerInstance.getNodeManager().getNodeStatus(requestMessage->src_addr) == E_Node_status_Disturb)
                {//�ŵ�����
                    DebugLog << "������Ƶ֡";

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
                    //������Դ������
                    setApNwkConfig(requestMessage);
                }
            }
            else
            {
                DebugLog << "û�к��ʵ�Ƶ�㹩ѡ��";
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

        DebugLog << "���͸����ն�����֡";
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
        //�յ�����
        if (requestMessage->data_len != 1)
        {
            DebugLog << "δ���ֲ�����";
            return 1;
        }

        if (requestMessage->data[0] != 0)
        {
            DebugLog << "�����ն��������ñ�ʧ�� ������:" << ByteToHexStr(requestMessage->data[0]);
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

        DebugLog << "�����ն��������ñ�ɹ�";
    }

    return 0;
}

int CZLogicCenter::setEdNwkConfig(cz_types::RequestMessage* requestMessage, bool bServerCall, bool bNeedReply)
{
    bufferLen = 0;

    if (bServerCall)
    {
        //����ID
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
            //��ȡ�ն˵�������Ϣ
            CZSingletonManagerInstance.getNodeManager().getCanReachSites(requestMessage->src_addr, (eNodeType)i, vecMap);

            if (vecMap == 0)
                continue;

            //������
            size = vecMap->size();

            if (size <= 0)
            {
                continue;
            }

            //����豸����
            responseBuffer[bufferLen] = i;
            bufferLen += 1;

            //����豸����
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
                    //��ȡ���
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
        //�յ�����
        if (requestMessage->data_len != 1)
        {
            DebugLog << "δ���ֲ�����";
            return 1;
        }

        if (requestMessage->data[0] != 0)
        {
            DebugLog << "�����ն��������ñ�ʧ�� ������:" << ByteToHexStr(requestMessage->data[0]);
            return 1;
        }

        DebugLog << "�����ն��������ñ�ɹ�";
    }

    return 0;
}

int CZLogicCenter::setApNwkConfig(cz_types::RequestMessage* requestMessage, bool bServerCall, bool bNeedReply)
{
    bufferLen = 0;

    if (bServerCall)
    {
        //����ID
        CZUShort network_id = CZSingletonManagerInstance.getConfiguration().getNetworkId();
        bufferLen = 0;
        memcpy(responseBuffer + bufferLen, &network_id, sizeof(network_id));
        bufferLen += sizeof(network_id);

        //�豸����
        eNodeType nodeType;
        CZSingletonManagerInstance.getNodeManager().getNodeType(requestMessage->src_addr, nodeType);
        responseBuffer[bufferLen] = (CZUChar)nodeType;
        bufferLen += 1;

        //�����ַ
        CZUShort network_addr = requestMessage->src_addr;
        memcpy(responseBuffer + bufferLen, &network_addr, sizeof(network_addr));
        bufferLen += sizeof(network_addr);

        //���߹������� WlInfo
        CZUShort freId = 0;
        CZSingletonManagerInstance.getNodeManager().getSiteFreid(network_addr, freId);
        memcpy(responseBuffer + bufferLen, &freId, sizeof(freId));
        bufferLen += sizeof(freId);

        //���͹���
        CZUChar txPower = (CZUChar)CZSingletonManagerInstance.getConfiguration().getShortFromServerConfig("site_tx_power");
        memcpy(responseBuffer + bufferLen, &txPower, sizeof(txPower));
        bufferLen += sizeof(txPower);

        sendMessage(requestMessage->session_id, requestMessage->src_addr, SetApNwkConfig, responseBuffer, bufferLen, bNeedReply);
    }
    else
    {
        //�յ�����
        if (requestMessage->data_len != 1)
        {
            DebugLog << "δ���ֲ�����";
            return 1;
        }

        if (requestMessage->data[0] != 0)
        {
            std::bitset<8> retStatus(requestMessage->data[0]);

            if (retStatus[0])
            {
                DebugLog << "���û�վ�������ñ�ʧ�� DeviceInfo����";
                disconnect(requestMessage);
            }

            if (retStatus[1])
            {
                DebugLog << "���û�վ�������ñ�ʧ�� ��֧�ֵ�FreId";
                disconnect(requestMessage);
            }

            if (retStatus[2])
            {
                DebugLog << "���û�վ�������ñ�ʧ�� ��֧�ֵ��豸����";
                disconnect(requestMessage);
            }

            return 1;
        }

        CZSingletonManagerInstance.getNodeManager().setNodeStatus(requestMessage->src_addr, E_Node_Status_Running);

        setApWorkTab(requestMessage, true, true);
        //�ɹ����͹㲥������Դ��

    }

    return 0;
}

int CZLogicCenter::noticeMSiteAddDevice(CZUShort siteNetworkAddr, CZUShort edNetworkAddr, bool add)
{
    bufferLen = 0;

    //��ȡ��Ӧ��վ����Ϣ

    eNodeType nodeType;

    CZInt iRet = CZSingletonManagerInstance.getNodeManager().getNodeType(siteNetworkAddr, nodeType);

    if (iRet)
    {
        DebugLog << "֪ͨM��վ���/ɾ���ն˻�ȡ��Ӧ���豸���ʹ���";
        return 1;
    }

    if (nodeType != M_AP)
    {
        DebugLog << "֪ͨM��վ���/ɾ���ն�Ŀ���豸���Ͳ�ΪM��վ";
        return 1;
    }

    eNodeType ed_nodeType;

    iRet = CZSingletonManagerInstance.getNodeManager().getNodeType(edNetworkAddr, ed_nodeType);

    if (iRet)
    {
        return 1;
    }
    //������Option����0Ϊɾ����1Ϊ���
    responseBuffer[bufferLen] = add ? 1 : 0;
    bufferLen += 1;

    //Ҫ��ӻ�ɾ�����ն˵��豸����
    responseBuffer[bufferLen] = (CZUChar)ed_nodeType;
    bufferLen += 1;

    //Ҫ��ӻ�ɾ�����ն˵������ַ
    memcpy(responseBuffer + bufferLen, &edNetworkAddr, sizeof(edNetworkAddr));
    bufferLen += sizeof(edNetworkAddr);

    if (add)
        DebugLog << "֪ͨM��վ" << ShortToHexStr(siteNetworkAddr) << "����ն�" << ShortToHexStr(edNetworkAddr);
    else
        DebugLog << "֪ͨM��վ" << ShortToHexStr(siteNetworkAddr) << "ɾ���ն�" << ShortToHexStr(edNetworkAddr);

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
                DebugLog << "�յ�֪ͨM��վ���/ɾ���ն˷��ؽ��:�ɹ�";
                break;
            }
            case 1:
            {
                DebugLog << "�յ�֪ͨM��վ���/ɾ���ն˷��ؽ��:��Ч���ն˲���";
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

    //��ȡ��Ӧ��վ����Ϣ

    eNodeType nodeType;

    CZInt iRet = CZSingletonManagerInstance.getNodeManager().getNodeType(siteNetworkAddr, nodeType);

    if (iRet)
    {
        DebugLog << "֪ͨR��վ���/ɾ�������ȡ��Ӧ���豸���ʹ���";
        return 1;
    }

    if (nodeType != R_AP)
    {
        DebugLog << "֪ͨR��վ���/ɾ������Ŀ���豸���Ͳ�ΪM��վ";
        return 1;
    }

    //������Option����0Ϊɾ����1Ϊ���
    responseBuffer[bufferLen] = 1;
    bufferLen += 1;

    //Ҫ��ӻ�ɾ�����ն˵������ַ
    memcpy(responseBuffer + bufferLen, &groupId, sizeof(groupId));
    bufferLen += sizeof(groupId);

    DebugLog << "֪ͨR��վ" << ShortToHexStr(siteNetworkAddr) << "��ӷ���" << groupId;

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
                DebugLog << "�յ�֪ͨR��վ��ӻ�ɾ��һ���鷵�ؽ��:�ɹ�";
                break;
            }
            case 1:
            {
                DebugLog << "�յ�֪ͨR��վ��ӻ�ɾ��һ���鷵�ؽ��:�����ʱ���С�ڻ����Ŀǰ��վ���";
                break;
            }
            case 2:
            {
                DebugLog << "�յ�֪ͨR��վ��ӻ�ɾ��һ���鷵�ؽ��:ɾ����ʱ��Ŵ���Ŀǰ��վ���";
                break;
            }
            default:
                break;
            }
        }
    }

    return 0;
}

/*********************��������end***********************/

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
            DebugLog << "�㲥����վ:" << ShortToHexStr(*it) << " ����:" << ByteToHexStr(type);
            sendMessage(0, *it, data_id, data_buffer, data_len, bNeedReply);
        }
    }
    return 0;
}