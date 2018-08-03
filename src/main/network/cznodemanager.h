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
 * brief\	节点管理
 *
 */
class CZNodeManager
{
public:
    typedef std::unordered_map<CZUShort, CZNodePtr> NodePool;

    /*!
     * \brief     构造函数
     *
     * \exception
     * \return
     * \remarks
     */
    CZNodeManager();

    /*!
     * \brief     析构函数
     *
     * \exception
     * \return
     * \remarks
     */
    ~CZNodeManager();

public:
    /*!
     * \brief     初始化
     *
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt init();

    CZInt test();

    /*!
     * \brief     创建终端节点
     *
     * \param     CZULongLong phy_addr 物理地址
     * \param     cz_types::eNodeType node_type 节点类型
     * \param     CZUShort network_addr 网络地址 不为0表示指定创建
     * \exception
     * \return    CZUShort 大于0 表示网络地址 0表示创建异常
     * \remarks
     */
    CZUShort createNode(const CZPhysicalAddr &phy_addr, cz_types::eNodeType node_type, CZUShort network_addr);

    /*!
     * \brief     添加解析后的帧数据到对应的Node节点中
     *
     * \param     LFrame_t * frame 解析后的帧数据
     * \param     CZUInt sessionId sessionID
     * \param     bool& bExistOkData 是否存在完整数据
     * \param     bool& bNeedAck 是否需要ack
     * \exception
     * \return    CZUShort
     * \remarks
     */
    CZUShort addFrameToNode(LFrame_t* frame, CZUInt sessionId, bool& bExistOkData, bool& bNeedAck);

    /*!
     * \brief     添加从node节点中获取数据
     *
     * \param     CZUShort network_addr 网络地址
     * \param     cz_types::RequestMessage* requestMessage 完整frame数据
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZUShort getMessageFromNode(CZUShort network_addr, cz_types::RequestMessage* requestMessage);

    /*!
     * \brief     通过物理地址获取网络地址
     *
     * \param     const CZPhysicalAddr &phy_addr 物理地址
     * \exception
     * \return    CZUShort 非0表示网络地址 0 表示未找到
     * \remarks
     */
    CZUShort getNetworkAddrByPhyAddr(const CZPhysicalAddr &phy_addr);

    /*!
     * \brief     通过网络地址找物理地址
     *
     * \param     CZUShort network 网络地址
     * \exception
     * \return    const CZPhysicalAddr & 非0表示物理地址  0 表示未找到
     * \remarks
     */
    const CZPhysicalAddr &getPhyAddrByNetworkAddr(CZUShort network);

    /*!
     * \brief     获取节点对应的链路
     *
     * \param     CZUShort network
     * \exception
     * \return    CZUShort
     * \remarks
     */
    CZUShort getNodeOverSessionRoad(CZUShort network);


    /*!
     * \brief     获取节点类型
     *
     * \param     CZUShort network
     * \param     eNodeType & type
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt getNodeType(CZUShort network, cz_types::eNodeType& type);

    /*!
     * \brief     获取节点存储
     *
     * \exception
     * \return    CZNodeManager::NodePool&
     * \remarks
     */
    CZNodeManager::NodePool& getNodePool();

    /*!
     * \brief     获取配置中基站身份
     *
     * \param     const CZPhysicalAddr &phy_addr
     * \param     eNodeType & nodeType
     * \exception
     * \return    bool
     * \remarks
     */
    bool getSiteIdentity(const CZPhysicalAddr &phy_addr, cz_types::eNodeType& nodeType);

    /*!
     * \brief     设置节点属性
     *
     * \param     CZUShort network_addr
     * \param     cz_types::eNodeStatus status
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt setNodeStatus(CZUShort network_addr, cz_types::eNodeStatus status);

    /*!
     * \brief     获取节点当前状态
     *
     * \param     CZUShort network_addr
     * \exception
     * \return    cz_types::eNodeStatus
     * \remarks
     */
    cz_types::eNodeStatus getNodeStatus(CZUShort network_addr);

    /*!
     * \brief     设置基站频点
     *
     * \param     CZUShort network_addr
     * \param     CZUShort freId
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt setSiteFreid(CZUShort network_addr, CZUShort freId);

    /*!
     * \brief     获取基站频点
     *
     * \param     CZUShort network_addr
     * \param     CZUShort & freId
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt getSiteFreid(CZUShort network_addr, CZUShort& freId);

    /*!
     * \brief     设置终端能搜索到的基站
     *
     * \param     CZUShort network_addr
     * \param     eNodeType nodeType 基站类型
     * \param     std::vector<CZUShort> & vec
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt setCanReachSites(CZUShort network_addr, cz_types::eNodeType nodeType, std::vector<CZUShort>& vec);

    /*!
     * \brief     获取终端能扫描到的基站信息
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
     * \brief     删除某个类型的基站
     *
     * \param     CZUShort network_addr
     * \param     cz_types::eNodeType nodeType
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt delCanReachSites(CZUShort network_addr, cz_types::eNodeType nodeType);

    /*!
     * \brief     通过频点查找对应终端的网络地址
     *
     * \param     CZUShort freId
     * \exception
     * \return    CZUShort
     * \remarks
     */
    CZUShort getNetworkAddrByFreid(CZUShort freId);

    /*!
     * \brief     添加超时重传任务
     *
     * \param     cz_types::TimeoutTask* task
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt addTimerTask(cz_types::TimerTask* task);

    /*!
     * \brief     删除某个超时重传任务
     *
     * \param     CZUShort network
     * \param     CZUChar data_id
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt delTimerTask(CZUShort network, CZUChar data_id);

    /*!
     * \brief     检测是否为服务器主动请求 防止黑入
     *
     * \param     CZUShort network
     * \param     CZUChar data_id
     * \exception
     * \return    bool
     * \remarks
     */
    bool checkIsHaveCase(CZUShort network, CZUChar data_id);

    /*!
     * \brief     获取超时重传任务
     *
     * \param     CZUShort network
     * \param     cz_types::TimeoutTask * task
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt getTimerTask(CZUShort network, cz_types::TimerTask* task);

    /*!
     * \brief     添加缓存
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
     * \brief     删除缓存
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
     * \brief     添加消息到队列中 目前只提供给重传使用
     *
     * \param     cz_types::RequestMessage * requestMessage
     * \exception
     * \return    CZUShort
     * \remarks
     */
    CZUShort addRequestMessageToQueue(CZUShort network, cz_types::RequestMessage* requestMessage);

    /*!
     * \brief     根据类型获取基站ID
     *
     * \param     eNodeType type
     * \param     std::vector<CZUShort> & vecSite
     * \exception
     * \return    void
     * \remarks
     */
    void getApByType(cz_types::eNodeType type, std::vector<CZUShort>& vecSite);

    /*!
     * \brief     获取扫描频点范围
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
     * \brief     设置频点
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
     * \brief     获取频点所服务的基站类型
     *
     * \param     CZUShort freId
     * \param     eNodeType & nodeType
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt getFreIdServiceType(CZUShort freId, cz_types::eNodeType& nodeType);

    /*!
     * \brief     获取M基站管理的终端列表
     *
     * \param     std::vector<CZUShort> & edList
     * \param     bool bInService
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt getEdManagerList(CZUShort network, std::vector<CZUShort>& edList, bool bInService = true);

    /*!
     * \brief     获取允许入网基站列表
     *
     * \exception
     * \return    boost::boost::unordered_map<boost::std::string,cz_types::eNodeType>
     * \remarks
     */
    inline std::unordered_map<CZPhysicalAddr, cz_types::eNodeType, CZPhysicalAddrHash<CZPhysicalAddr>> &getAllowSites() { return siteMap; }

    /*!
     * \brief     添加允许某个基站入网
     *
     * \param     const CZPhysicalAddr &phyAddr
     * \param     eNodeType type
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt addSiteAllow(const CZPhysicalAddr &phyAddr, cz_types::eNodeType type);

    /*!
     * \brief     通过位置信息查找对应的终端
     *
     * \param     CZUShort groupId
     * \param     CZUChar pos
     * \exception
     * \return    CZUShort 0 表示未找到 非0表示对应的网络地址
     * \remarks
     */
    CZUShort findEdByPos(CZUShort network, CZUShort groupId, CZUChar pos);

    /*!
     * \brief     查询R基站中某个终端的位置
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
     * \brief     获取当前分组大小
     *
     * \param     CZUShort rsiteNetworkAddr
     * \exception
     * \return    CZUShort
     * \remarks
     */
    CZUShort getRsiteCurrentGroupSize(CZUShort rsiteNetworkAddr);


    /*!
     * \brief     R基站添加一个终端到分组中
     *
     * \param     CZUShort ed_network_addr 终端网络地址
     * \param     CZUShort & groupId 分配的组号
     * \param     CZUChar & pos 分配组中的位置
     * \param	  bool& bGroupChange 组是否变化
     * \param	  bool isUseDesignGroup 是否使用指定分组
     * \exception
     * \return    CZInt 0添加成功 非0添加失败
     * \remarks
     */
    CZInt addEdToGroup(CZUShort network, CZUShort ed_network_addr, CZUShort& groupId, CZUChar& pos, bool& bGroupChange, bool isUseDesignGroup = false);

    CZInt setFaultSite(CZUShort network_addr, CZUShort faultSite);
    CZInt getFaultSite(CZUShort network_addr, CZUShort &faultSite);

    /*!
     * \brief     获取节点
     *
     * \exception
     * \return    CZNode&
     * \remarks
     */
    CZNodePtr getNodeByNwkAddr(CZUShort network_addr);

public:

    /*!
     * \brief     添加一个终端到管理列表
     *
     * \param     CZUShort ed_network_addr
     * \exception
     * \return    CZInt
     * \remarks
     */
    CZInt addEdToManagerList(CZUShort network, CZUShort ed_network_addr, bool inService);

    /*!
     * \brief     从管理列表中删除一个终端
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
     *	存储node
     */
    NodePool nodePool;

    /*!
     *	物理地址与网络地址的映射关系
     */
    std::unordered_map<CZPhysicalAddr, CZUShort, CZPhysicalAddrHash<CZPhysicalAddr>> phyAddrToNetworkAddrMap;

    /*!
     *	存储基站身份配置
     */
    std::unordered_map<CZPhysicalAddr, cz_types::eNodeType, CZPhysicalAddrHash<CZPhysicalAddr>> siteMap;

    /*!
     *	基站网络地址
     */
    CZUShort site_network_addr_index;

    /*!
     *	终端网络地址
     */
    CZUShort device_network_addr_index;

    /*!
     *	频点使用记录
     *  Key(FreId)->Value(Network) 要求顺序遍历
     */
    std::map<CZUShort, CZUShort> freidUseMap;

    /*!
     *	读写锁
     */
    std::mutex rwmutex;

    /*!
     *	key:nodeType,value(network_addr)
     */
    std::unordered_map<cz_types::eNodeType, std::vector<CZUShort>> nodeTypeMap;

    /*!
     *	是否正在初始化
     */
    bool isIniting;

    /*!
     *	m基站频点范围
     */
    CZUShort msiteRange;
};

#endif //#ifndef CZ_NODE_MANAGER_H
