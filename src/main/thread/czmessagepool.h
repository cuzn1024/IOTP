/*!
*   \brief      消息池
*   \details    用于存储基站或终端所需处理消息。未避免冲突，目前设计为单入单出，后续可能会考虑多线程处理。
*   \file       thread/czmessagepool.h
*   \author     cuzn
*   \version    0.1.0
*
* 更新历史
*
* 版本号|说明|修订者|修订日期
* ------|----|------|--------
* v0.1.0|创建|cuzn|2016/04/07
*
*/
#ifndef CZ_MESSAGEPOOL_H
#define CZ_MESSAGEPOOL_H

#ifndef BOOST_POOL_HPP
#include <boost/pool/pool.hpp>
#endif //#ifndef BOOST_POOL_HPP

#ifndef BOOST_CIRCULAR_BUFFER_HPP
#include <boost/circular_buffer.hpp>
#endif //#ifndef BOOST_CIRCULAR_BUFFER_HPP

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

/*!
 * \brief 消息队列
 */
class CZMessagePool
{
public:
	/*!
	 * \brief 构造函数
	 */
    CZMessagePool();

public:
    
    /*!
     * \brief     存在请求消息的node
     * 
     * \param     CZUShort node_network_addr
     * \exception
     * \return    CZInt
     * \remarks  
     */
    CZInt addRequestNode(CZUShort node_network_addr);

	/*!
	 * \brief     获取有数据需要处理的node节点
	 * 
	 * \exception
	 * \return    CZUShort 0 表示没有 非0表示具体node的网络地址
	 * \remarks  
	 */
	CZUShort getRequestNode();

	/*!
	 * \brief     添加返回消息结构
	 *
	 * \param     CZUShort session_id 链路ID
	 * \param     CZUShort dst_network_addr 网络地址
	 * \param     CZUChar data_id 数据ID
	 * \param     CZUChar * data_buffer 数据内容指针
	 * \param     CZUShort data_len 数据内容长度
	 * \param     bool bNeedTimeoutRetry 是否需要超时重传，默认不需要
	 * \return    CZInt 0 添加成功 非0添加失败
	 * \remarks  
	 */
	CZInt addResponseMessage(CZUShort session_id,CZUShort dst_network_addr,CZUChar data_id,CZUChar* data_buffer,CZUShort data_len,bool bNeedTimeoutRetry = false);
	
	/*!
	 * \brief     获取返回数据
	 * 
	 * \param     cz_types::ResponseMessage * responseMessage
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt getResponseMessage(cz_types::ResponseMessage* responseMessage);

	/*!
	 * \brief     添加超时重传任务
	 *
	 * \param     cz_types::TimeoutTask * task
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt addTimerTask(cz_types::TimerTask* task);

	/*!
	 * \brief     删除超时重传任务
	 *
	 * \param     CZUShort network_addr
	 * \param     CZUChar data_id
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt delTimerTask(CZUShort network_addr,CZUChar data_id);

	/*!
	 * \brief     获取超时重传任务
	 *
	 * \param     cz_types::TimeoutTask * task
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt getTimerTask(cz_types::TimerTask* task);

private:

	/*!
	 *	请求消息队列
	 */
    boost::circular_buffer<CZUShort> requestNodePoolQueue;

	/*!
	 *	存储一条消息
	 */
    boost::pool<> responseMessagePool;
	/*!
	 *	发送消息队列
	 */
    boost::circular_buffer<CZUChar*> responseMessagePoolQueue;

	/*!
	 *	存储一条超时重传任务
	 */
    boost::pool<> timeoutMsgPool;
	/*!
	 *	超时重传队列
	 */
    boost::circular_buffer<CZUChar*> timeoutMsgPoolQueue;
};

#endif //#ifndef CZ_MESSAGEPOOL_H
