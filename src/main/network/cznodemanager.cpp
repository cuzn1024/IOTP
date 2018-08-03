#include <network/cznodemanager.h>

#include <algorithm>

typedef std::lock_guard<std::mutex> readLock;
typedef std::unique_lock<std::mutex> writeLock;

#include <network/cznode.h>
#include <network/czrsite.h>
#include <network/czmsite.h>
#include <network/czdevice.h>
#include <system/czlog.h>
#include <logic/czdatahelper.h>
#include <czsingletonmanager.h>
#include <utils/czmiscs.h>

CZNodeManager::CZNodeManager() :site_network_addr_index(SITE_NETWORK_ADDR_MIN), device_network_addr_index(DEVICE_NETWORK_ADDR_MIN)
{
    isIniting = false;
    msiteRange = 20;
}

CZNodeManager::~CZNodeManager()
{
}

CZInt CZNodeManager::init()
{
    isIniting = true;

    msiteRange = (CZUShort)(CZMiscs::s2i(CZSingletonManagerInstance.getDataHelper().getServerConfig("msite_range")));

    //�����ݿ��������������վ����
    CZSingletonManagerInstance.getDataHelper().loadAllSiteAllow();

    //����Node�ڵ���Ϣ
    nodePool.clear();
    CZSingletonManagerInstance.getDataHelper().loadAllNodeInfo();

    //����Ƶ����Ϣ
    CZSingletonManagerInstance.getDataHelper().loadAllFreidInfo();

    //����m��վ���������
    CZSingletonManagerInstance.getDataHelper().loadAllMSiteWork();

    //����r��վ���������
    CZSingletonManagerInstance.getDataHelper().loadAllRSiteWork();

    //��ʼ���������������node
    createNode(0, No_Care, JOIN_ED_NETWORK_ADDR);
    createNode(0, No_Care, JOIN_SITE_NETWORK_ADDR);

    isIniting = false;

    return 0;
}

CZInt CZNodeManager::test()
{
    //   std::for_each(siteMap.begin(), siteMap.end(), [](std::unordered_map<const std::string,eNodeType>::value_type &node){
       //	std::cout<<node.first<<" --- "<<(int)(node.second)<<std::endl;
       //});

       //std::cout<<std::endl<<"------------------------------------"<<std::endl;

       //BOOST_FOREACH(NodePool::value_type &node,nodePool)
       //{
       //	std::cout<<ShortToHexStr(node.second->getNetworkAddr())<<"---------"<<LongLongToHexStr(node.second->getPhyAddr())<<std::endl;	
       //}

       //for(std::map<CZUShort,CZUShort>::iterator it = freidUseMap.begin();it!= freidUseMap.end();it++)
       //{
       //	std::cout<<it->first<<" --- "<<(int)(it->second)<<std::endl;
       //}

    return 0;
}

CZUShort CZNodeManager::createNode(const CZPhysicalAddr &phy_addr, cz_types::eNodeType node_type, CZUShort network_addr)
{
    if (network_addr == 0)
    {
        //�Լ�������ַ
        do
        {
            if (CZMiscs::isDevice(node_type))
            {
                network_addr = device_network_addr_index++;

                if (network_addr < DEVICE_NETWORK_ADDR_MIN || network_addr >= JOIN_ED_NETWORK_ADDR)
                {
                    network_addr = DEVICE_NETWORK_ADDR_MIN;
                }
            }
            else
            {
                network_addr = site_network_addr_index++;

                if (network_addr < SITE_NETWORK_ADDR_MIN || network_addr >= 255)
                {
                    network_addr = SITE_NETWORK_ADDR_MIN;
                }
            }

        } while (nodePool.find(network_addr) != nodePool.end());
    }

    //����addr����node
    NodePool::iterator it = nodePool.find(network_addr);

    if (it == nodePool.end())
    {
        CZNode* node = NULL;

        if (node_type <= Server)
        {
            node = new CZNode(network_addr, phy_addr, node_type);
        }
        else
        {
            if (CZMiscs::isDevice(node_type))
            {
                node = new CZDevice(network_addr, phy_addr, node_type);
            }
            else if (node_type == M_AP)
            {
                node = new CZMSite(network_addr, phy_addr, node_type);
            }
            else if (node_type == R_AP)
            {
                node = new CZRSite(network_addr, phy_addr, node_type);
            }
            else
            {
                DebugLog << "CreateNode error,can not find type to match";
            }
        }

        if (NULL != node)
        {
            nodePool.insert(NodePool::value_type(network_addr, CZNodePtr(node)));

            std::vector<CZUShort> *vec = &(nodeTypeMap[node_type]);

            vec->push_back(network_addr);

            phyAddrToNetworkAddrMap.insert(boost::unordered_map<CZPhysicalAddr, CZUShort>::value_type(phy_addr, network_addr));

            //DebugLog<<"�����ڵ�,�����ַ:"<<LongLongToHexStr(phy_addr)<<" �����ַ:"<<ShortToHexStr(network_addr)<<" ����:"<<(int)node_type;

            if (!isIniting && node_type > Server)
            {
                //Server����Ϊ�����豸  ��д�����ݿ�
                CZSingletonManagerInstance.getDataHelper().createNode(network_addr, phy_addr.str(), node_type);
            }
        }
    }

    return network_addr;
}

CZUShort CZNodeManager::getMessageFromNode(CZUShort network_addr, cz_types::RequestMessage* requestMessage)
{
    if (0 == network_addr)
    {
        return 1;
    }

    //ͨ��network_addr�ҵ���Ӧ�Ľڵ�

    NodePool::iterator it = nodePool.find(network_addr);
    if (it == nodePool.end())
    {
        //˵����û���ҵ�
        return 1;
    }

    return (it->second)->getMessageFromNode(requestMessage);
}

CZUShort CZNodeManager::addRequestMessageToQueue(CZUShort network, cz_types::RequestMessage* requestMessage)
{
    if (0 == network)
    {
        return 1;
    }

    //ͨ��network_addr�ҵ���Ӧ�Ľڵ�

    NodePool::iterator it = nodePool.find(network);
    if (it == nodePool.end())
    {
        //˵����û���ҵ�
        return 1;
    }

    return (it->second)->addRequestMessageToQueue(requestMessage);
}

CZUShort CZNodeManager::addFrameToNode(LFrame_t* frame, CZUInt sessionId, bool& bExistOkData, bool& bNeedAck)
{
    if (NULL == frame)
    {
        return 1;
    }

    CZUShort src_network_addr = frame->head.src_addr;

    NodePool::iterator it = nodePool.find(src_network_addr);

    if (it == nodePool.end())
    {
        //��ǰ�ڵ�δ��������
        DebugLog << "Node:" << ShortToHexStr(frame->head.src_addr) << " is not in network";
        return 1;
    }

    //��ӵ�ǰ֡����Ӧ�Ľڵ���ȥ
    return (it->second)->addFrame(frame, sessionId, bExistOkData, bNeedAck);
}

CZUShort CZNodeManager::getNetworkAddrByPhyAddr(const CZPhysicalAddr &phy_addr)
{
    if (phy_addr.empty())
    {
        return 0;
    }

    if (std::unordered_map<CZPhysicalAddr, CZUShort>::iterator it = phyAddrToNetworkAddrMap.find(phy_addr); it != phyAddrToNetworkAddrMap.end())
    {
        return it->second;
    }

    return 0;
}

void CZNodeManager::getApByType(eNodeType type, std::vector<CZUShort>& vecSite)
{
    vecSite.clear();

    if (type < M_AP || type > Multiplex_AP)
    {
        return;
    }

    for_each(nodePool.begin(), nodePool.end(), [type, &vecSite](NodePool::value_type &node) {
        if ((node.second)->getType() == type)
        {
            vecSite.push_back((node.second)->getNetworkAddr());
        }
    });
}


CZInt CZNodeManager::getFreidRange(eNodeType type, CZUShort& startFreid, CZUShort& endFreid)
{
    readLock rdLock(rwmutex);

    startFreid = endFreid = 0;

    //������ʼλ��
    CZUInt iBeginSearchPos = (type == M_AP) ? 0 : msiteRange;

    if (freidUseMap.size() < iBeginSearchPos)
    {
        return 1;
    }

    std::map<CZUShort, CZUShort>::iterator it = freidUseMap.begin();

    bool bFind = false;

    std::advance(it, iBeginSearchPos);

    for (; it != freidUseMap.end(); it++)
    {
        if ((it->second) == 0)
        {
            startFreid = it->first;
            bFind = true;
            break;
        }
    }

    if (!bFind)
    {
        return 1;
    }

    endFreid = startFreid + 9;

    return 0;
}

CZUShort CZNodeManager::getNetworkAddrByFreid(CZUShort freId)
{
    if (freId <= 0)
    {
        return 0;
    }

    std::map<CZUShort, CZUShort>::iterator it = freidUseMap.find(freId);

    if (it == freidUseMap.end())
    {
        return 0;
    }

    return it->second;
}

CZInt CZNodeManager::setFreidInfo(CZUShort freId, CZUShort network)
{
    writeLock  wtlock(rwmutex);

    //��ʼ��ʱ���ж��߼�
    if (isIniting)
    {
        freidUseMap[freId] = network;

        if (network > 0)
        {
            setSiteFreid(network, freId);
        }
    }
    else
    {
        if (freId <= 0 || network <= 0)
        {
            return 1;
        }

        eNodeType nodeType;

        if (getNodeType(network, nodeType))
        {
            return 1;
        }

        if (!CZMiscs::isSite((CZUChar)nodeType))
        {
            return 1;
        }

        //���network֮ǰ�Ƿ��ж�Ӧ��Ƶ��
        CZUShort oldFreid = 0;

        getSiteFreid(network, oldFreid);

        if (oldFreid > 0)
        {
            freidUseMap[oldFreid] = 0;

            CZSingletonManagerInstance.getDataHelper().setFreidInfo(oldFreid, 0, No_Care);
        }

        freidUseMap[freId] = network;

        setSiteFreid(network, freId);

        CZSingletonManagerInstance.getDataHelper().setFreidInfo(freId, network, nodeType);
    }

    return 0;
}

CZInt CZNodeManager::getFreIdServiceType(CZUShort freId, eNodeType& nodeType)
{
    CZUShort service_network_addr = getNetworkAddrByFreid(freId);

    if (service_network_addr == 0)
    {
        return 1;
    }

    return getNodeType(service_network_addr, nodeType);
}

const CZPhysicalAddr &CZNodeManager::getPhyAddrByNetworkAddr(CZUShort network)
{
    if (network == 0)
    {
        return 0;
    }

    NodePool::iterator it = nodePool.find(network);

    if (it == nodePool.end())
    {
        return 0;
    }

    return (it->second)->getPhyAddr();
}

CZNodePtr CZNodeManager::getNodeByNwkAddr(CZUShort network_addr)
{
    if (network_addr == 0) return 0;

    NodePool::iterator it = nodePool.find(network_addr);

    return it == nodePool.end() ? 0 : it->second;
}

CZUShort CZNodeManager::getNodeOverSessionRoad(CZUShort network)
{
    if (network == 0)
    {
        return 0;
    }

    NodePool::iterator it = nodePool.find(network);

    if (it == nodePool.end())
    {
        return 0;
    }

    eNodeType nodeType = (it->second)->getType();

    if (!CZMiscs::isNodeTypeRight(nodeType) || CZMiscs::isDevice(nodeType))
    {
        return 0;
    }

    return (it->second)->getSessionId();
}

CZInt CZNodeManager::getNodeType(CZUShort network, eNodeType& type)
{
    if (network == 0)
    {
        return 1;
    }

    NodePool::iterator it = nodePool.find(network);

    if (it == nodePool.end())
    {
        return 1;
    }

    type = (it->second)->getType();

    return 0;
}

CZNodeManager::NodePool& CZNodeManager::getNodePool()
{
    return nodePool;
}

bool CZNodeManager::getSiteIdentity(const CZPhysicalAddr &phy_addr, eNodeType& nodeType)
{
    std::unordered_map<CZPhysicalAddr, eNodeType>::iterator it = siteMap.find(phy_addr);

    if (it == siteMap.end())
    {
        return false;
    }

    CZPhysicalAddr addr = siteMap.begin()->first;

    nodeType = it->second;

    return true;
}
CZInt CZNodeManager::setNodeStatus(CZUShort network_addr, cz_types::eNodeStatus status)
{
    if (network_addr == 0)
    {
        return 1;
    }

    NodePool::iterator it = nodePool.find(network_addr);

    if (it == nodePool.end())
    {
        return 1;
    }

    //��ӵ�ǰ֡����Ӧ�Ľڵ���ȥ
    (it->second)->setStatus(status);

    return 0;
}

cz_types::eNodeStatus CZNodeManager::getNodeStatus(CZUShort network_addr)
{
    eNodeStatus status = E_Node_Status_Ready;

    if (network_addr == 0)
    {
        return status;
    }

    if (NodePool::iterator it = nodePool.find(network_addr); it != nodePool.end())
    {
        status = (it->second)->getStatus();
    }

    return status;
}

CZInt CZNodeManager::setSiteFreid(CZUShort network_addr, CZUShort freId)
{
    if (network_addr == 0)
    {
        return 1;
    }

    if (NodePool::iterator it = nodePool.find(network_addr); it != nodePool.end())
    {
        //��ӵ�ǰ֡����Ӧ�Ľڵ���ȥ
        (it->second)->setFreId(freId);
        return 0;
    }

    return 1;
}

CZInt CZNodeManager::setFaultSite(CZUShort network_addr, CZUShort faultSite)
{
    if (network_addr == 0)
    {
        return 1;
    }

    if (NodePool::iterator it = nodePool.find(network_addr); it != nodePool.end())
    {
        //��ӵ�ǰ֡����Ӧ�Ľڵ���ȥ
        (it->second)->setFaultSite(faultSite);

        return 0;
    }

    return 1;
}

CZInt CZNodeManager::getFaultSite(CZUShort network_addr, CZUShort &faultSite)
{
    if (network_addr == 0)
    {
        return 1;
    }

    if (NodePool::iterator it = nodePool.find(network_addr); it != nodePool.end())
    {
        //��ӵ�ǰ֡����Ӧ�Ľڵ���ȥ
        return (it->second)->getFaultSite(faultSite);
    }

    return 1;
}

CZInt CZNodeManager::setCanReachSites(CZUShort network_addr, eNodeType nodeType, std::vector<CZUShort>& vec)
{
    if (network_addr == 0 || vec.size() < 0)
    {
        return 1;
    }

    NodePool::iterator it = nodePool.find(network_addr);

    if (it == nodePool.end())
    {
        return 1;
    }

    eNodeType type = (it->second)->getType();

    if (!CZMiscs::isDevice((CZUChar)type))
    {
        return 1;
    }

    delCanReachSites(network_addr, nodeType);

    //��ӵ�ǰ֡����Ӧ�Ľڵ���ȥ

    for (std::vector<CZUShort>::iterator vIt = vec.begin(); vIt != vec.end(); vIt++)
    {
        if (nodeType == M_AP)
        {
            if (!isIniting)
            {
                //��ӵ����ݿ�
                CZSingletonManagerInstance.getDataHelper().addToMSiteList(*vIt, network_addr, true);
            }

            if (vIt == vec.begin())
            {
                if (!isIniting)
                {
                    CZSingletonManagerInstance.getDataHelper().setMSiteInservice(*vIt, network_addr, true);
                }

                //��ӵ��ڴ�
                addEdToManagerList(*vIt, network_addr, true);
            }
            else
            {
                //��ӵ��ڴ�
                addEdToManagerList(*vIt, network_addr, false);
            }
        }
        else if (nodeType == R_AP)
        {
            //R�����ڴ��е�����ڷ�����ִ�� ��Ϊ�漰��λ����Ϣ

            if (!isIniting)
            {
                //��ӵ����ݿ�
                CZSingletonManagerInstance.getDataHelper().addToRSiteGroup(*vIt, network_addr);

                if (vIt == vec.begin())
                {
                    CZSingletonManagerInstance.getDataHelper().setRSiteInservice(*vIt, network_addr, true);
                }
            }
        }
    }

    return (it->second)->setCanReachSites(nodeType, vec);
}

CZInt CZNodeManager::getCanReachSites(CZUShort network_addr, eNodeType nodeType, std::vector<CZUShort> *&sitesVec)
{
    sitesVec = 0;

    NodePool::iterator it = nodePool.find(network_addr);

    if (it == nodePool.end())
        return 1;

    eNodeType type = (it->second)->getType();

    if (!CZMiscs::isDevice(type))
    {
        return 1;
    }

    return (it->second)->getCanReachSites(nodeType, sitesVec);
}

CZInt CZNodeManager::delCanReachSites(CZUShort network_addr, cz_types::eNodeType nodeType)
{
    if (network_addr == 0)
    {
        return 1;
    }

    NodePool::iterator it = nodePool.find(network_addr);

    if (it == nodePool.end())
    {
        return 1;
    }

    eNodeType type = (it->second)->getType();

    if (!CZMiscs::isDevice((CZUChar)type))
    {
        return 1;
    }

    std::vector<CZUShort> *siteVec = 0;

    getCanReachSites(network_addr, nodeType, siteVec);

    if (siteVec != 0 && siteVec->size() > 0)
    {
        if (!isIniting)
        {
            for (std::vector<CZUShort>::iterator vIt = siteVec->begin(); vIt != siteVec->end(); vIt++)
            {
                if (nodeType == M_AP)
                {
                    CZSingletonManagerInstance.getDataHelper().delFromMSiteList(*vIt, network_addr);
                }
                else if (nodeType == R_AP)
                {
                    CZSingletonManagerInstance.getDataHelper().delRSiteGroupNode(*vIt, network_addr);
                }
            }
        }
    }

    return (it->second)->delCanReachSites(nodeType);
}

CZInt CZNodeManager::getSiteFreid(CZUShort network_addr, CZUShort& freId)
{
    if (network_addr == 0)
    {
        return 1;
    }

    NodePool::iterator it = nodePool.find(network_addr);

    if (it == nodePool.end())
    {
        return 1;
    }

    eNodeType nodeType = (it->second)->getType();

    if (!CZMiscs::isNodeTypeRight(nodeType) || CZMiscs::isDevice(nodeType))
    {
        return 1;
    }

    //��ӵ�ǰ֡����Ӧ�Ľڵ���ȥ
    freId = (it->second)->getFreid();

    return freId > 0 ? 0 : 1;
}

CZInt CZNodeManager::addTimerTask(cz_types::TimerTask* task)
{
    //���������ַ��Ӧ�Ľڵ�
    if (NULL == task || task->dst_addr == 0)
    {
        return 1;
    }

    NodePool::iterator it = nodePool.find(task->dst_addr);

    if (it == nodePool.end())
    {
        return 1;
    }

    //DebugLog<<"������ӳ�ʱ�ش�����,network:"<<ShortToHexStr(task->dst_addr)<<" data_id:"<<ByteToHexStr(task->data_id)<<" retry times:"<<task->retryTimes;

    return (it->second)->addTimerTask(task);
}

CZInt CZNodeManager::delTimerTask(CZUShort network, CZUChar data_id)
{
    //���������ַ��Ӧ�Ľڵ�
    if (network == 0)
    {
        return 1;
    }

    NodePool::iterator it = nodePool.find(network);

    if (it == nodePool.end())
    {
        return 1;
    }

    return (it->second)->delTimerTask(data_id);
}

bool CZNodeManager::checkIsHaveCase(CZUShort network, CZUChar data_id)
{
    //���������ַ��Ӧ�Ľڵ�
    if (network == 0)
    {
        return false;
    }

    NodePool::iterator it = nodePool.find(network);

    if (it == nodePool.end())
    {
        return false;
    }

    return (it->second)->checkIsHaveCase(data_id);
}

CZInt CZNodeManager::getTimerTask(CZUShort network, cz_types::TimerTask* task)
{
    //���������ַ��Ӧ�Ľڵ�
    if (network == 0)
    {
        return 1;
    }

    NodePool::iterator it = nodePool.find(network);

    if (it == nodePool.end())
    {
        return 1;
    }

    return (it->second)->getTimerTask(task);
}

CZInt CZNodeManager::setCache(CZUShort network, const std::string &key, const std::string &value)
{
    //���������ַ��Ӧ�Ľڵ�
    if (network == 0)
    {
        return 1;
    }

    NodePool::iterator it = nodePool.find(network);

    if (it == nodePool.end())
    {
        return 1;
    }

    return (it->second)->setCache(key, value);
}

CZInt CZNodeManager::getCache(CZUShort network, const std::string &key, std::string &value, bool isDeleteThisKey)
{
    //���������ַ��Ӧ�Ľڵ�
    if (network == 0)
    {
        return 1;
    }

    NodePool::iterator it = nodePool.find(network);

    if (it == nodePool.end())
    {
        return 1;
    }

    return (it->second)->getCache(key, value, isDeleteThisKey);
}

CZInt CZNodeManager::addEdToManagerList(CZUShort network, CZUShort ed_network_addr, bool inService)
{
    //���������ַ��Ӧ�Ľڵ�
    if (network == 0)
    {
        return 1;
    }

    NodePool::iterator it = nodePool.find(network);

    if (it == nodePool.end())
    {
        return 1;
    }

    return (it->second)->addEdToManagerList(ed_network_addr, inService);
}

CZInt CZNodeManager::delEdFromManagerList(CZUShort network, CZUShort ed_network_addr)
{
    //���������ַ��Ӧ�Ľڵ�
    if (network == 0)
    {
        return 1;
    }

    NodePool::iterator it = nodePool.find(network);

    if (it == nodePool.end())
    {
        return 1;
    }

    CZInt iRet = (it->second)->delEdFromManagerList(ed_network_addr);

    if (iRet)
    {
        return iRet;
    }

    if (!isIniting)
    {
        CZSingletonManagerInstance.getDataHelper().delFromMSiteList(network, ed_network_addr);
    }

    return iRet;
}

CZInt CZNodeManager::getEdManagerList(CZUShort network, std::vector<CZUShort>& edList, bool bInService)
{
    if (network == 0)
    {
        return 1;
    }

    NodePool::iterator it = nodePool.find(network);

    if (it == nodePool.end())
    {
        return 1;
    }

    return (it->second)->getEdManagerList(edList, bInService);
}

CZInt CZNodeManager::addSiteAllow(const CZPhysicalAddr &phyAddr, eNodeType type)
{
    if (type >= N_ED)
        return 0;

    siteMap[phyAddr] = (type);

    if (!isIniting)
    {
        CZSingletonManagerInstance.getDataHelper().addSiteAllow(phyAddr.str(), type);
    }

    return 0;
}

CZInt CZNodeManager::addEdToGroup(CZUShort network, CZUShort ed_network_addr, CZUShort& groupId, CZUChar& pos, bool& bGroupChange, bool isUseDesignGroup)
{
    if (network == 0)
    {
        return 1;
    }

    NodePool::iterator it = nodePool.find(network);

    if (it == nodePool.end())
    {
        return 1;
    }

    if (CZInt iRet = (it->second)->addEdToGroup(ed_network_addr, groupId, pos, bGroupChange, isUseDesignGroup); iRet != 0)
    {
        return iRet;
    }

    if (!isIniting)
    {
        CZSingletonManagerInstance.getDataHelper().setRSiteGroupInfo(network, ed_network_addr, groupId, pos);
    }

    return 0;
}
CZUShort CZNodeManager::findEdByPos(CZUShort network, CZUShort groupId, CZUChar pos)
{
    if (network == 0)
    {
        return 1;
    }

    NodePool::iterator it = nodePool.find(network);

    if (it == nodePool.end())
    {
        return 1;
    }

    return (it->second)->findEdByPos(groupId, pos);
}

CZInt CZNodeManager::findEdGroupInfo(CZUShort rsiteNetworkAddr, CZUShort edNetworkAddr, CZUShort& groupId, CZUChar& pos)
{
    if (rsiteNetworkAddr == 0)
    {
        return 1;
    }

    NodePool::iterator it = nodePool.find(rsiteNetworkAddr);

    if (it == nodePool.end())
    {
        return 1;
    }

    return (it->second)->findEdGroupInfo(edNetworkAddr, groupId, pos);

}

CZUShort CZNodeManager::getRsiteCurrentGroupSize(CZUShort rsiteNetworkAddr)
{
    if (rsiteNetworkAddr == 0)
    {
        return 0;
    }

    NodePool::iterator it = nodePool.find(rsiteNetworkAddr);

    if (it == nodePool.end())
    {
        return 0;
    }

    return (it->second)->getRsiteCurrentGroupSize();
}