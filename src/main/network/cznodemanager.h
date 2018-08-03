#ifndef CZ_NODE_MANAGER_H
#define CZ_NODE_MANAGER_H

#ifndef _MAP_
#include <map>
#endif //#ifndef _MAP_


#ifndef _UNORDERED_MAP_
#include <memory>
#endif //#ifndef _UNORDERED_MAP_

#ifndef _UNORDERED_MAP_
#include <unordered_map>
#endif //#ifndef _UNORDERED_MAP_

#ifndef _VECTOR_
#include <vector>
#endif //#ifndef _VECTOR_

#ifndef _MUTEX_
#include <mutex>
#endif //#ifndef _MUTEX_

#ifndef CZ_DEFINES_H
#include <defines.h>
#endif //#ifndef CZ_DEFINES_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

extern "C"
{
#include <resolver/pro_l.h>
};

class CZNode;

/*!
 * brief\	�ڵ����
 *
 */
class CZNodeManager
{
public:
    typedef std::unordered_map<CZUShort, CZNodePtr> NodePool;

    /*!
     * \brief     ���캯��
     *
     * \exception
     * \return
     * \remarks
     */
    CZNodeManager();

    /*!
     * \brief     ��������
     *
     * \exception
     * \return
     * \remarks
     */
    ~CZNodeManager();

public:
    /*!
     * \brief     ��ʼ��
     *
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt init();

    CZInt test();

    /*!
     * \brief     �����ն˽ڵ�
     *
     * \param     CZULongLong phy_addr �����ַ
     * \param     cz_types::eNodeType node_type �ڵ�����
     * \param     CZUShort network_addr �����ַ ��Ϊ0��ʾָ������
     * \exception
     * \return    CZUShort ����0 ��ʾ�����ַ 0��ʾ�����쳣
     * \remarks
     */
    CZUShort createNode(const CZPhysicalAddr &phy_addr, cz_types::eNodeType node_type, CZUShort network_addr);

    /*!
     * \brief     ��ӽ������֡���ݵ���Ӧ��Node�ڵ���
     *
     * \param     LFrame_t * frame �������֡����
     * \param     CZUInt sessionId sessionID
     * \param     bool& bExistOkData �Ƿ������������
     * \param     bool& bNeedAck �Ƿ���Ҫack
     * \exception
     * \return    CZUShort
     * \remarks
     */
    CZUShort addFrameToNode(LFrame_t* frame, CZUInt sessionId, bool& bExistOkData, bool& bNeedAck);

    /*!
     * \brief     ��Ӵ�node�ڵ��л�ȡ����
     *
     * \param     CZUShort network_addr �����ַ
     * \param     cz_types::RequestMessage* requestMessage ����frame����
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZUShort getMessageFromNode(CZUShort network_addr, cz_types::RequestMessage* requestMessage);

    /*!
     * \brief     ͨ�������ַ��ȡ�����ַ
     *
     * \param     const CZPhysicalAddr &phy_addr �����ַ
     * \exception
     * \return    CZUShort ��0��ʾ�����ַ 0 ��ʾδ�ҵ�
     * \remarks
     */
    CZUShort getNetworkAddrByPhyAddr(const CZPhysicalAddr &phy_addr);

    /*!
     * \brief     ͨ�������ַ�������ַ
     *
     * \param     CZUShort network �����ַ
     * \exception
     * \return    const CZPhysicalAddr & ��0��ʾ�����ַ  0 ��ʾδ�ҵ�
     * \remarks
     */
    const CZPhysicalAddr &getPhyAddrByNetworkAddr(CZUShort network);

    /*!
     * \brief     ��ȡ�ڵ��Ӧ����·
     *
     * \param     CZUShort network
     * \exception
     * \return    CZUShort
     * \remarks
     */
    CZUShort getNodeOverSessionRoad(CZUShort network);


    /*!
     * \brief     ��ȡ�ڵ�����
     *
     * \param     CZUShort network
     * \param     eNodeType & type
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt getNodeType(CZUShort network, cz_types::eNodeType& type);

    /*!
     * \brief     ��ȡ�ڵ�洢
     *
     * \exception
     * \return    CZNodeManager::NodePool&
     * \remarks
     */
    CZNodeManager::NodePool& getNodePool();

    /*!
     * \brief     ��ȡ�����л�վ���
     *
     * \param     const CZPhysicalAddr &phy_addr
     * \param     eNodeType & nodeType
     * \exception
     * \return    bool
     * \remarks
     */
    bool getSiteIdentity(const CZPhysicalAddr &phy_addr, cz_types::eNodeType& nodeType);

    /*!
     * \brief     ���ýڵ�����
     *
     * \param     CZUShort network_addr
     * \param     cz_types::eNodeStatus status
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt setNodeStatus(CZUShort network_addr, cz_types::eNodeStatus status);

    /*!
     * \brief     ��ȡ�ڵ㵱ǰ״̬
     *
     * \param     CZUShort network_addr
     * \exception
     * \return    cz_types::eNodeStatus
     * \remarks
     */
    cz_types::eNodeStatus getNodeStatus(CZUShort network_addr);

    /*!
     * \brief     ���û�վƵ��
     *
     * \param     CZUShort network_addr
     * \param     CZUShort freId
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt setSiteFreid(CZUShort network_addr, CZUShort freId);

    /*!
     * \brief     ��ȡ��վƵ��
     *
     * \param     CZUShort network_addr
     * \param     CZUShort & freId
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt getSiteFreid(CZUShort network_addr, CZUShort& freId);

    /*!
     * \brief     �����ն����������Ļ�վ
     *
     * \param     CZUShort network_addr
     * \param     eNodeType nodeType ��վ����
     * \param     std::vector<CZUShort> & vec
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt setCanReachSites(CZUShort network_addr, cz_types::eNodeType nodeType, std::vector<CZUShort>& vec);

    /*!
     * \brief     ��ȡ�ն���ɨ�赽�Ļ�վ��Ϣ
     *
     * \param     CZUShort network_addr
     * \param     std::map<CZUChar
     * \param     std::::vector<CZUShort>& sitesVec
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt getCanReachSites(CZUShort network_addr, cz_types::eNodeType nodeType, std::vector<CZUShort> *&sitesVec);

    /*!
     * \brief     ɾ��ĳ�����͵Ļ�վ
     *
     * \param     CZUShort network_addr
     * \param     cz_types::eNodeType nodeType
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt delCanReachSites(CZUShort network_addr, cz_types::eNodeType nodeType);

    /*!
     * \brief     ͨ��Ƶ����Ҷ�Ӧ�ն˵������ַ
     *
     * \param     CZUShort freId
     * \exception
     * \return    CZUShort
     * \remarks
     */
    CZUShort getNetworkAddrByFreid(CZUShort freId);

    /*!
     * \brief     ��ӳ�ʱ�ش�����
     *
     * \param     cz_types::TimeoutTask* task
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt addTimerTask(cz_types::TimerTask* task);

    /*!
     * \brief     ɾ��ĳ����ʱ�ش�����
     *
     * \param     CZUShort network
     * \param     CZUChar data_id
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt delTimerTask(CZUShort network, CZUChar data_id);

    /*!
     * \brief     ����Ƿ�Ϊ�������������� ��ֹ����
     *
     * \param     CZUShort network
     * \param     CZUChar data_id
     * \exception
     * \return    bool
     * \remarks
     */
    bool checkIsHaveCase(CZUShort network, CZUChar data_id);

    /*!
     * \brief     ��ȡ��ʱ�ش�����
     *
     * \param     CZUShort network
     * \param     cz_types::TimeoutTask * task
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt getTimerTask(CZUShort network, cz_types::TimerTask* task);

    /*!
     * \brief     ��ӻ���
     *
     * \param     CZUShort network
     * \param     const std::string &key
     * \param     const std::string &value
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt setCache(CZUShort network, const std::string &key, const std::string &value);

    /*!
     * \brief     ɾ������
     *
     * \param     CZUShort network
     * \param     const std::string &key
     * \param     std::string &value
     * \param     bool isDeleteThisKey
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt getCache(CZUShort network, const std::string &key, std::string &value, bool isDeleteThisKey = true);

    /*!
     * \brief     �����Ϣ�������� Ŀǰֻ�ṩ���ش�ʹ��
     *
     * \param     cz_types::RequestMessage * requestMessage
     * \exception
     * \return    CZUShort
     * \remarks
     */
    CZUShort addRequestMessageToQueue(CZUShort network, cz_types::RequestMessage* requestMessage);

    /*!
     * \brief     �������ͻ�ȡ��վID
     *
     * \param     eNodeType type
     * \param     std::vector<CZUShort> & vecSite
     * \exception
     * \return    void
     * \remarks
     */
    void getApByType(cz_types::eNodeType type, std::vector<CZUShort>& vecSite);

    /*!
     * \brief     ��ȡɨ��Ƶ�㷶Χ
     *
     * \param     eNodeType type
     * \param     CZUShort & startFreid
     * \param     CZUShort & endFreid
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt getFreidRange(cz_types::eNodeType type, CZUShort& startFreid, CZUShort& endFreid);

    /*!
     * \brief     ����Ƶ��
     *
     * \param     eNodeType type
     * \param     CZUShort freId
     * \param     CZUShort network
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt setFreidInfo(CZUShort freId, CZUShort network);

    /*!
     * \brief     ��ȡƵ��������Ļ�վ����
     *
     * \param     CZUShort freId
     * \param     eNodeType & nodeType
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt getFreIdServiceType(CZUShort freId, cz_types::eNodeType& nodeType);

    /*!
     * \brief     ��ȡM��վ������ն��б�
     *
     * \param     std::vector<CZUShort> & edList
     * \param     bool bInService
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt getEdManagerList(CZUShort network, std::vector<CZUShort>& edList, bool bInService = true);

    /*!
     * \brief     ��ȡ����������վ�б�
     *
     * \exception
     * \return    boost::boost::unordered_map<boost::std::string,cz_types::eNodeType>
     * \remarks
     */
    inline std::unordered_map<CZPhysicalAddr, cz_types::eNodeType, CZPhysicalAddrHash<CZPhysicalAddr>> &getAllowSites() { return siteMap; }

    /*!
     * \brief     �������ĳ����վ����
     *
     * \param     const CZPhysicalAddr &phyAddr
     * \param     eNodeType type
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt addSiteAllow(const CZPhysicalAddr &phyAddr, cz_types::eNodeType type);

    /*!
     * \brief     ͨ��λ����Ϣ���Ҷ�Ӧ���ն�
     *
     * \param     CZUShort groupId
     * \param     CZUChar pos
     * \exception
     * \return    CZUShort 0 ��ʾδ�ҵ� ��0��ʾ��Ӧ�������ַ
     * \remarks
     */
    CZUShort findEdByPos(CZUShort network, CZUShort groupId, CZUChar pos);

    /*!
     * \brief     ��ѯR��վ��ĳ���ն˵�λ��
     *
     * \param     CZUShort rsiteNetworkAddr
     * \param     CZUShort edNetworkAddr
     * \param     CZUShort & groupId
     * \param     CZUChar & pos
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt findEdGroupInfo(CZUShort rsiteNetworkAddr, CZUShort edNetworkAddr, CZUShort& groupId, CZUChar& pos);

    /*!
     * \brief     ��ȡ��ǰ�����С
     *
     * \param     CZUShort rsiteNetworkAddr
     * \exception
     * \return    CZUShort
     * \remarks
     */
    CZUShort getRsiteCurrentGroupSize(CZUShort rsiteNetworkAddr);


    /*!
     * \brief     R��վ���һ���ն˵�������
     *
     * \param     CZUShort ed_network_addr �ն������ַ
     * \param     CZUShort & groupId ��������
     * \param     CZUChar & pos �������е�λ��
     * \param	  bool& bGroupChange ���Ƿ�仯
     * \param	  bool isUseDesignGroup �Ƿ�ʹ��ָ������
     * \exception
     * \return    CZInt 0��ӳɹ� ��0���ʧ��
     * \remarks
     */
    CZInt addEdToGroup(CZUShort network, CZUShort ed_network_addr, CZUShort& groupId, CZUChar& pos, bool& bGroupChange, bool isUseDesignGroup = false);

    CZInt setFaultSite(CZUShort network_addr, CZUShort faultSite);
    CZInt getFaultSite(CZUShort network_addr, CZUShort &faultSite);

    /*!
     * \brief     ��ȡ�ڵ�
     *
     * \exception
     * \return    CZNode&
     * \remarks
     */
    CZNodePtr getNodeByNwkAddr(CZUShort network_addr);

public:

    /*!
     * \brief     ���һ���ն˵������б�
     *
     * \param     CZUShort ed_network_addr
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt addEdToManagerList(CZUShort network, CZUShort ed_network_addr, bool inService);

    /*!
     * \brief     �ӹ����б���ɾ��һ���ն�
     *
     * \param     CZUShort network
     * \param     CZUShort ed_network_addr
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt delEdFromManagerList(CZUShort network, CZUShort ed_network_addr);

private:
    /*!
     *	�洢node
     */
    NodePool nodePool;

    /*!
     *	�����ַ�������ַ��ӳ���ϵ
     */
    std::unordered_map<CZPhysicalAddr, CZUShort, CZPhysicalAddrHash<CZPhysicalAddr>> phyAddrToNetworkAddrMap;

    /*!
     *	�洢��վ�������
     */
    std::unordered_map<CZPhysicalAddr, cz_types::eNodeType, CZPhysicalAddrHash<CZPhysicalAddr>> siteMap;

    /*!
     *	��վ�����ַ
     */
    CZUShort site_network_addr_index;

    /*!
     *	�ն������ַ
     */
    CZUShort device_network_addr_index;

    /*!
     *	Ƶ��ʹ�ü�¼
     *  Key(FreId)->Value(Network) Ҫ��˳�����
     */
    std::map<CZUShort, CZUShort> freidUseMap;

    /*!
     *	��д��
     */
    std::mutex rwmutex;

    /*!
     *	key:nodeType,value(network_addr)
     */
    std::unordered_map<cz_types::eNodeType, std::vector<CZUShort>> nodeTypeMap;

    /*!
     *	�Ƿ����ڳ�ʼ��
     */
    bool isIniting;

    /*!
     *	m��վƵ�㷶Χ
     */
    CZUShort msiteRange;
};

#endif //#ifndef CZ_NODE_MANAGER_H
