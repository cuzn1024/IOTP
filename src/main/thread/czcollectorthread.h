/*!
*   \brief      数据采集线程
*   \details    用于调用会话采集数据
*   \file       thread/czcollectorthread.h
*   \author     cuzn
*   \version    0.1.0
*
* 更新历史
*
* 版本号|说明|修订者|修订日期
* ------|----|------|--------
* v0.1.0|创建|cuzn|2016/04/12
*
*/
#ifndef CZ_COLLECTORTHREAD_H
#define CZ_COLLECTORTHREAD_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#ifndef CZ_ITHREAD_H
#include <thread/czithread.h>
#endif //#ifndef CZ_ITHREAD_H

/*!
 *	\brief	链路交互
 * 
 *	底层数据发送与接收
 *
 */
class CZCollectorThread : public CZIThread
{
public:
    /*!
     * \brief     构造函数
     * 
     */
    CZCollectorThread(void);

    /*!
     * \brief     析构函数
     * 
     */
    ~CZCollectorThread(void);

protected:
    

    /*!
     * \brief     线程任务
     * 
     * \return    void
     * \remarks  
     */
    void run();
    
private:
    

    /*!
     * \brief     串口等相关初始化
     * 
	 * 初始化串口session和创建申请session(特化session 网络地址为0xfffe)
	 *
     * \return    void
     * \remarks  
     */
    void serialportConnectionHandler();
    
	/*!
	 * \brief     检查超时
	 *
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	void checkTimeout();

    /*!
     * \brief     检测是否有新socket接入
     * 
     * \return    void
     * \remarks  
     */
    void establishRequestHandler();

    /*!
     * \brief     链路数据交互(接收与发送)
     * 
     * \return    void
     * \remarks  
     */
    void processDataHandler();
};

#endif //#ifndef CZ_COLLECTORTHREAD_H
