#ifndef CZ_NODE_H
#define CZ_NODE_H

#ifndef _UNORDERED_MAP_
#include <unordered_map>
#endif //#ifndef _UNORDERED_MAP_

#ifndef _VECTOR_
#include <vector>
#endif //#ifndef _VECTOR_

#ifndef BOOST_POOL_HPP
#include <boost/pool/pool.hpp>
#endif //#ifndef BOOST_POOL_HPP

#ifndef BOOST_CIRCULAR_BUFFER_HPP
#include <boost/circular_buffer.hpp>
#endif //#ifndef BOOST_CIRCULAR_BUFFER_HPP

#ifndef CZ_DEFINES_H
#include <defines.h>
#endif //#ifndef CZ_DEFINES_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

class CZFrameBuffer;

extern "C"
{
#include <resolver/pro_l.h>
};

/*!
 * \brief 节点的基类
 */
class CZNode
{
public:
    /*!
     * \brief     构造函数
     * 
     * \param     CZUShort _network_addr 网络地址
     * \param     CZULongLong _phy_addr 物理地址
	 * \param     cz_types::eNodeType node_type 身份标识
     * \exception
     * \return    
     * \remarks  
     */
    CZNode(CZUShort _network_addr, const CZPhysicalAddr &_phy_addr,cz_types::eNodeType node_type);

    /*!
     * \brief     释放申请的资源
     * 
     * \exception
     * \return    
     * \remarks  
     */
    ~CZNode();
	
public:
	/*!
	 * \brief     设置网络地址
	 * 
	 * \param     CZUShort _network_addr 网络地址
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	inline void setNetworkAddr(CZUShort _network_addr){network_addr = _network_addr;}

	/*!
	* \brief     获取网络地址
	*
	* \exception
	* \return    CZUShort 基站的地址只有1byte、终端的地址才是2个byte
	* \remarks
	*/
	inline CZUShort getNetworkAddr() { return network_addr; }

	/*!
	 * \brief     设置物理地址
	 * 
	 * \param     CZUShort _phy_addr 物理地址
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	inline void setPhyAddr(const CZPhysicalAddr &_phy_addr) {phy_addr = _phy_addr;}

	/*!
	 * \brief     获取物理地址
	 * 
	 * \exception
	 * \return    CZULongLong 物理地址
	 * \remarks  8个byte组成
	 */
	inline const CZPhysicalAddr &getPhyAddr() const {return phy_addr;}

	/*!
	 * \brief     获取节点类型
	 *
	 * \exception
	 * \return    cz_types::eNodeType 自身的节点类型
	 * \remarks  
	 */
	inline cz_types::eNodeType getType(){return type;}

	/*!
	* \brief     设置节点属性
	*
	* \param     eNodeStatus status
	* \exception
	* \return    void
	* \remarks
	*/
	inline void setStatus(cz_types::eNodeStatus status) { this->status = status; }
	
	/*!
	 * \brief     获取节点状态
	 *
	 * \exception
	 * \return    eNodeStatus
	 * \remarks  
	 */
	inline cz_types::eNodeStatus getStatus(){return status;}

	/*!
	* \brief     添加帧到node节点
	*
	* \param     LFrame_t * parseFrame 帧数据
	* \param     CZUInt sessionId sessionid
	* \param     bool & bExistOkData 是否存在完整数据
	* \param     bool & bNeedAck 是否需要ACK
	* \exception
	* \return    CZUShort 0 正常 1 参数及数据异常
	* \remarks
	*/
	CZUShort addFrame(LFrame_t* parseFrame, CZUInt sessionId, bool& bExistOkData, bool& bNeedAck);

	/*!
	 * \brief     从node队列中获取一条请求消息
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \exception
	 * \return    CZUShort 0 标识有 非0标识没有
	 * \remarks  
	 */
	CZUShort getMessageFromNode(cz_types::RequestMessage* requestMessage);
	
	/*!
	* \brief     添加请求消息到队列
	*
	* \param     cz_types::RequestMessage * requestMessage 请求消息
	* \exception
	* \return    CZUShort
	* \remarks
	*/
	CZUShort addRequestMessageToQueue(cz_types::RequestMessage* requestMessage);

	/*!
	 * \brief     添加超时重传任务
	 * 
	 * \param     cz_types::TimerTask* task
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt addTimerTask(cz_types::TimerTask* task);

	/*!
	 * \brief     删除超时重传任务
	 * 
	 * \param     CZUChar data_id
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt delTimerTask(CZUChar data_id);

	/*!
	 * \brief     检测是否由服务器主动发起请求 防止黑入
	 *
	 * \param     CZUChar data_id
	 * \exception
	 * \return    bool
	 * \remarks  
	 */
	bool checkIsHaveCase(CZUChar data_id);
	
	/*!
	 * \brief     获取超时重传任务
	 *
	 * \param     cz_types::TimerTask * task
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt getTimerTask(cz_types::TimerTask* task);

	/*!
	 * \brief     设置缓存
	 *
	 * \param     const std::string &key
	 * \param     const std::string &value
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt setCache(const std::string &key, const std::string &value);

	/*!
	 * \brief     获取缓存
	 *
	 * \param     const std::string &key
	 * \param     std::string &value
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt getCache(const std::string &key, std::string &value,bool isDeleteThisKey=true);

	/*!
	* \brief     获取链路ID
	*
	* \exception
	* \return    CZUShort 链路ID
	* \remarks  对应session链路传输
	*/
	virtual inline CZUShort getSessionId() { return sessionId; }
	
	/*!
	 * \brief     设置链路ID
	 *
	 * \param     CZUShort id
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	virtual inline void setSessionId(CZUShort id) { sessionId = id; }

public:
	/************************************************************************/
	/* 以下为虚函数                                                         */
	/************************************************************************/

	/*!
	 *	czsite中真正实现
	 */
	virtual CZUShort getFreid() { return 0 ;}

	/*!
	 *	czsite中真正实现
	 */
	virtual void setFreId(CZUShort id){}

	/*!
	 *	csdevice中真正实现
	 */
	virtual CZInt setCanReachSites(cz_types::eNodeType nodeType, std::vector<CZUShort>& vec) {return 1;}

	/*!
	 *	csdevice中真正实现
	 */
	virtual CZInt getCanReachSites(cz_types::eNodeType nodeType,std::vector<CZUShort> *&vec) {return 1;}

	/*!
	 *	csdevice中真正实现
	 */
	virtual CZInt delCanReachSites(cz_types::eNodeType nodeType){return 1;}

	/*!
	 *	czmsite中真正实现
	 */
	virtual CZInt addEdToManagerList(CZUShort ed_network_addr,bool inService){return 1;}

	/*!
	 *	czmsite中真正实现
	 */
	virtual CZInt delEdFromManagerList(CZUShort ed_network_addr){return 1;}

	/*!
	 *	czmsite中真正实现
	 */
	virtual CZInt getEdManagerList(std::vector<CZUShort>& edList,bool bInService = true){return 1;}

	/*!
	 *	czrsite中真正实现
	 */
	virtual CZInt addEdToGroup(CZUShort ed_network_addr,CZUShort& groupId,CZUChar& pos,bool& bGroupChange,bool isUseDesignGroup = false){return 1;}

	/*!
	 *	czrsite中真正实现
	 */
	virtual CZUShort findEdByPos(CZUShort groupId,CZUChar pos){return 0;}

	/*!
	*	czrsite中真正实现
	*/
	virtual CZInt findEdGroupInfo(CZUShort networkAddr, CZUShort& groupId, CZUChar& pos) { return 1; }

	/*!
	 *	czmsite中真正实现
	 */
	virtual CZInt getMSiteLastOperAction(bool& bAdd,CZUShort& oper_network_addr){ return 1;}

	/*!
	*	czrsite中真正实现
	*/
	virtual CZUShort getRsiteCurrentGroupSize(){return 0;}

    CZInt setFaultSite(CZUShort site);
    CZInt getFaultSite(CZUShort &site);

	/************************************************************************/
	/* 以上为虚函数                                                         */
	/************************************************************************/

private:

	/*!
	*	链路id
	*/
	CZUShort sessionId;
	/*!
	 *	网络地址
	 */
	CZUShort network_addr;

	/*!
	 *	物理地址
	 */
	CZPhysicalAddr phy_addr;

	/*!
	 *	节点类型
	 * 
	 *  身份标识
	 */
	cz_types::eNodeType type;

	/*!
	 *	临时数据存储
	 */
	CZUChar *dataBuffer;

	/*!
	 *	用于存储临时数据
	 */
	cz_types::RequestMessage* requestMessage;

	/*!
	 *	存储一条消息
	 */
    boost::pool<> requestMessagePool;
	/*!
	 *	发送消息队列
	 */
    boost::circular_buffer<CZUChar*> requestMessagePoolQueue;

	/*!
	 *	帧缓存
	 */
	CZFrameBuffer* pFrameBuffer;

	/*!
	 *	消息id
	 */
	CZUShort mIndex;

	/*!
	 *	当前节点状态
	 */
	cz_types::eNodeStatus status;

	/*!
	 *	存储一条超时重传任务
	 */
    boost::pool<> timeoutMsgPool;

	/*!
	 *	超时重传任务
	 */
	std::unordered_map<CZUChar, CZUChar*> timeoutTaskMap;

	/*!
	 *	缓存存储
	 */
	std::unordered_map<std::string, std::string> cacheMap;

    CZUShort faultSite;
};

#endif //#ifndef CZ_NODE_H
