#ifndef CZLOGICCENTER_H
#define CZLOGICCENTER_H

#ifndef CZ_DEFINES_H
#include <defines.h>
#endif //#ifndef CZ_DEFINES_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#include <boost/function.hpp>

#define FUN_ENTRY_RETRY_TYPE boost::function3<int, cz_types::RequestMessage*,bool,bool>
#define ENTRY_RETRY_TYPE_BIND(entry) boost::bind(entry, this, _1,_2,_3)

enum eDataId
{
	ApReqJoinNwk			 = 0x08, 		/*! *	基站请求入网 */
	EdReqJoinNwk			 = 0x09, 		/*! *	终端请求入网 */
	GetWlQty			     = 0x0A,        /*! *	获取指定信道的网络质量表 */
	SetApNwkConfig			 = 0x0B,        /*! *	设置基站网络配置表 */
	SetEdNwkConfig			 = 0x0C,        /*! *	设置终端网络配置表 */
	NwkResource				 = 0x0D,        /*! *	广播网络资源表 */
	ReqAddOrDelEd			 = 0x0E,        /*! *	向M基站添加或删除一个终端 */
	ReqAddOrDelGroup		 = 0x0F,        /*! *	向R基站添加或删除一个组 */
	GetSysTime				 = 0x10,        /*! *	获取系统时间 */
    SetApWorkTab             = 0x11,        /*! *	基站工作任务表 */
	EdActive    			 = 0x13,        /*! *	终端激活信息 */
	Disconnect	             = 0x15,        /*! *   断开链接 */		
    Fault                    = 0x16,        /*! *   故障上报 */		
    HopFreq                  = 0x17,        /*! *   跳频 */
    ModifyEdNwkConfig        = 0x18,        /*! *   修改终端网络配置表 */
    GetDeviceInfo            = 0x20,        /*! *   获取设备信息 */
	Test                     = 0x55
};


/*!
 * \brief 逻辑处理
 */
class CZLogicCenter
{
public:
    /*!
     * \brief     构造函数
     * 
     * \exception
     * \return    
     * \remarks  
     */
    CZLogicCenter();

    /*!
     * \brief     析构函数
     * 
     * \exception
     * \return    
     * \remarks  
     */
    ~CZLogicCenter();
	
	/*!
	 * \brief     初始化
	 * 
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	void init();

	/*!
	 * \brief     分发并处理数据
	 * 
	 * \param     cz_types::RequestMessage * requestMessage 来源消息
	 * \exception
	 * \return    int 
	 * \remarks  
	 */
	int processMessage(cz_types::RequestMessage* requestMessage);
private:

	/*!
	 * \brief     检测是否为重传逻辑
	 *
	 * \param     CZUChar data_id
	 * \exception
	 * \return    bool
	 * \remarks  
	 */
	bool bRetryLogic(CZUChar data_id);

	/*!
	 * \brief     注册逻辑处理函数
	 * 
	 * \param     cz_defines::eDataId data_id 数据ID
	 * \param     FUN_ENTRY_TYPE entry
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	inline void registerEntry(eDataId data_id, FUN_ENTRY_TYPE entry) { logicTable[data_id] = entry; }

	/*!
	 * \brief     注册重传逻辑处理函数
	 * 
	 * \param     cz_defines::eDataId data_id 数据ID
	 * \param     FUN_ENTRY_TYPE entry
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	inline void registerRetryEntry(eDataId data_id, FUN_ENTRY_RETRY_TYPE entry) { retryLogicTable[data_id] = entry; }

	/*!
	 * \brief     广播网络资源表
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \param     bool bServerCall
	 * \param     bool bNeedReply
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int nwkResource(cz_types::RequestMessage* requestMessage,bool bServerCall,bool bNeedReply);

	/*!
	 * \brief     获取网络质量表
	 *
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int getWlQty(cz_types::RequestMessage* requestMessage,bool bServerCall = true,bool bNeedReply = true);

    int getDeviceInfo(cz_types::RequestMessage *requestMessage);

	/*!
	 * \brief     设置终端网络配置表
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \param     bool bServerCall
	 * \param     bool bNeedReply
	 * \exception
	 * \return    int
	 * \remarks  对应指令0x33
	 */
	CZInt setEdNwkConfig(cz_types::RequestMessage* requestMessage,bool bServerCall = true,bool bNeedReply = true);

    CZInt setModifyEdNwkConfig(cz_types::RequestMessage* requestMessage,bool bServerCall = true,bool bNeedReply = true);

	/*!
	 * \brief     设置基站网络配置表
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \param     bool bServerCall
	 * \param     bool bNeedReply
	 * \exception
	 * \return    int
	 * \remarks  对应指令0x35
     */
    CZInt setApNwkConfig(cz_types::RequestMessage* requestMessage, bool bServerCall = true, bool bNeedReply = true);

	
	int test(cz_types::RequestMessage* requestMessage,bool bServerCall = true,bool bNeedReply = true);

	/*!
	 * \brief     基站工作任务表
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \param     bool bServerCall
	 * \param     bool bNeedReply
	 * \exception
	 * \return    int
	 * \remarks  对应指令0x36
     */
    int setApWorkTab(cz_types::RequestMessage* requestMessage, bool bServerCall = true, bool bNeedReply = true);

	/*!
	 * \brief     终端申请入网
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \exception
	 * \return    int
	 * \remarks   对应指令0x38
	 */
	int edReqJoinNetwork(cz_types::RequestMessage* requestMessage);

	/*!
	 * \brief     基站申请入网
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \exception
	 * \return    int
	 * \remarks   对应指令0x39
	 */
	int apReqJoinNetwork(cz_types::RequestMessage* requestMessage);

	/*!
	 * \brief     基站申请入网
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \exception
	 * \return    int
	 * \remarks   对应指令0x39
	 */
	int getSysTime(cz_types::RequestMessage* requestMessage);

	/*!
	 * \brief     终端进入工作态
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int edActive(cz_types::RequestMessage* requestMessage);

    CZInt hopFreq(cz_types::RequestMessage* requestMessage, bool bServerCall = true, bool bNeedReply = true);
	
	/*!
	 * \brief     收到断开链接
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int disconnectAck(cz_types::RequestMessage* requestMessage);

	/*!
	 * \brief     收到故障上报
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int faultReport(cz_types::RequestMessage* requestMessage);

	/*!
	 * \brief     主动断开链接
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \param     bool bServerCall
	 * \param     bool bNeedReply
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int disconnect(cz_types::RequestMessage* requestMessage, bool bServerCall = true, bool bNeedReply = true);

	/*!
	 * \brief     通知M基站添加终端
	 *
	 * \param     CZUShort siteNetworkAddr
	 * \param     CZUShort edNetworkAddr
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int noticeMSiteAddDevice(CZUShort siteNetworkAddr, CZUShort edNetworkAddr, bool add);

	/*!
	 * \brief     向M基站添加或删除一个终端
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \param     bool bServerCall
	 * \param     bool bNeedReply
	 * \exception
	 * \return    int
	 * \remarks  对应指令0x3A
	 */
	int reqAddOrDelEd(cz_types::RequestMessage* requestMessage,bool bServerCall = true,bool bNeedReply = true);

	/*!
	 * \brief     通知R基站添加分组
	 *
	 * \param     CZUShort siteNetworkAddr
	 * \param     CZUShort groupId
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int noticeRSiteAddGroup(CZUShort siteNetworkAddr,CZUShort groupId);

	/*!
	 * \brief     向R基站添加或删除一个组
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \param     bool bServerCall
	 * \param     bool bNeedReply
	 * \exception
	 * \return    int
	 * \remarks  对应指令0x3B
	 */
	int reqAddOrDelGroup(cz_types::RequestMessage* requestMessage,bool bServerCall = true,bool bNeedReply = true);

	/*!
	 * \brief     添加定时任务
	 *
	 * \param     CZUShort dst_network_addr
	 * \param     CZUChar data_id
	 * \param     CZUChar * data_buffer
	 * \param     CZUShort data_len
	 * \param     CZUShort seconds
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int addTimerTask(CZUShort dst_network_addr, CZUShort session_id, CZUChar data_id,CZUChar* data_buffer,CZUShort data_len,CZUShort seconds);

	/*!
	 * \brief     原路返回数据
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \param     CZUChar * data_buffer
	 * \param     CZUShort data_len
	 * \param     bool bNeedReply 需要回复 用于定时重传
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int sendMessage(cz_types::RequestMessage* requestMessage,CZUChar* data_buffer,CZUShort data_len,bool bNeedReply=false);

	/*!
	 * \brief     发送消息给节点
	 * \param     CZUShort sessionId 链路id
	 * \param     CZUShort dst_network_addr 网络地址
	 * \param     CZUChar data_id 数据ID
	 * \param     CZUChar * data_buffer 数据内容指针
	 * \param     CZUShort data_len 数据内容长度
	 * \param     bool bNeedReply 需要回复 用于定时重传
	 * \return    int 0 正常  非0异常
	 * \remarks  
	 */
	int sendMessage(CZUShort sessionId,CZUShort dst_network_addr,CZUChar data_id,CZUChar* data_buffer,CZUShort data_len,bool bNeedReply=false);
	
	/*!
	 * \brief     广播给所有基站
	 *
	 * \param     CZUChar data_id
	 * \param     CZUChar * data_buffer
	 * \param     CZUShort data_len
	 * \param     bool bNeedReply
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int broadcastToAllSite(CZUChar data_id,CZUChar* data_buffer,CZUShort data_len,bool bNeedReply = false);

private:	
	/*!
	 *	逻辑函数表
	 */
	boost::function1<int, cz_types::RequestMessage*> logicTable[255];

	/*!
	 *	重传逻辑
	 */
	boost::function3<int, cz_types::RequestMessage*,bool,bool> retryLogicTable[255];

	/*!
	 *	返回结果缓存
	 */
	CZUChar *timerTaskBuffer;

	/*!
	 *	定时任务结构
	 */
	cz_types::TimerTask* task;

	/*!
	 *	返回结果缓存
	 */
	CZUChar *responseBuffer;

	/*!
	 *	返回结果长度
	 */
	CZUShort bufferLen;
};

#endif //#ifndef CZLOGICCENTER_H
