/*!
*   \brief      连接管理器
*   \details    用于管理所有会话连接。
*   \file       network/czconnectionsuite.h
*   \author     cuzn
*   \version    0.1.0
*
* 更新历史
*
* 版本号|说明|修订者|修订日期
* ------|----|------|--------
* v0.1.0|创建|cuzn|2016/04/18
*
*/
#ifndef CZ_CONNECTIONSUITE_H
#define CZ_CONNECTIONSUITE_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

class CZNode;
class CZConnectionSuite_Private;
class CZITransceiverInterface;

/*!
 * \brief 连接管理器，作为各个模块的数据共享纽带
 *
 * 管理链路层的Session
 * 管理虚拟的终端Node节点
 * 管理不通的消息队列
 */
class CZConnectionSuite
{
    friend class CZSingletonManager;
private:
    CZConnectionSuite();
    virtual ~CZConnectionSuite();

public:
    /*! \brief  管理器初始化
    *   <pre>初始化串口连接。
    *   初始化网口监听。 </pre>
    */
    CZInt initialize();

    /*!
     * \brief     初始化串口session
     * \return    void
     */
    void initializeSessionPool();

    /*!
     * \brief     检查某个串口是否可用
     * 
     * \param     const std::string & port
     * \return    bool
     */
    bool serialportAvailable(const std::string &port);

    /*!
     * \brief     检查网口是否有新的连接
     *
     * \exception
     * \return    void
     */
    void acceptConnection();

	/*!
	 * \brief     从接收队列中获取一条frame数据
	 * 
	 * \param     cz_types::RequestFrame *&request frame存储
	 * \param     CZInt & length	数据长度
	 * \return    CZInt 0 正常 非0异常
	 */
	CZInt getFrameFromRecvQueue(cz_types::RequestFrame *&request);

	/*!
	 * \brief     添加消息到发送队列
	 * 
	 * \param     cz_types::ResponseFrame * rsp 待发送消息
	 * \return    CZInt CZInt 0 正常 非0异常
	 */
	CZInt addMsgToSendQueue(cz_types::ResponseFrame* rsp);

	/*!
	 * \brief     检查超时
	 *
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	void checkTimeout();

	/*!
	 * \brief     底层链路收取数据(串口或网口)
	 * 
	 * \return    void
	 */
	void collectData();

	/*!
	 * \brief     发送队列中存在的数据
	 * 
	 * \return    void
	 */
	void sendData();
private:
	

public:

    /*!
     * \brief     初始化串口
     * 
     * \exception
     * \return    void
     * \remarks  
     */
    void establishSerialPortPool();

    /*!
     * \brief     初始化tcp监听
     * 
     * \exception
     * \return    void
     * \remarks  
     */
    CZInt establishNetworkListener();

private:

	/*!
	 *	私有化操作类 主要用途避免头文件include过多
	 */
    CZConnectionSuite_Private *p;	
};

#endif // CZ_CONNECTIONSUITE_H
