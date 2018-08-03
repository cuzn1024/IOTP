/*!
*   \brief      消息处理线程
*   \details    用于调用基站处理封装后的消息。未避免冲突，目前设计为单实例，后续可能会考虑多实例处理。
*   \file       thread/czprocessorthread.h
*   \author     cuzn
*   \version    0.1.0
*
* 更新历史
*
* 版本号|说明|修订者|修订日期
* ------|----|------|--------
* v0.1.0|创建|cuzn|2016/04/05
*
*/
#ifndef CZ_PROCESSORTHREAD_H
#define CZ_PROCESSORTHREAD_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#ifndef CZ_DEFINES_H
#include <defines.h>
#endif //#ifndef CZ_DEFINES_H

#ifndef CZ_ITHREAD_H
#include <thread/czithread.h>
#endif //#ifndef CZ_ITHREAD_H

#ifndef CZLOGICCENTER_H
#include <logic/czlogiccenter.h>
#endif

/*! \brief  消息处理线程
*   <pre>该线程从\a CZMessagepool 提取组装后的消息，并驱动相应的基站或终端进行处理。
*   为避免冲突，仅在单个\a CZProcessorThread 实例中访问基站消息发送功能。
*   后续如果设计发送缓冲队列，可考虑多个\a CZProcessorThread 实例。</pre>
*/
class CZProcessorThread : public CZIThread
{
public:
    CZProcessorThread();
    ~CZProcessorThread(void);

protected:
    /*! <pre>线程处理函数
    *   负责基站或终端处理消息事宜，包含且不限于网络资源表、C基站工作任务表及重新组网等命令下发。
    *   从\a CZMessagePool 提取消息并驱动基站或终端执行并下发。
    *   继承至\a CZIThread </pre>
    *   \see CZIThread
    */
    void run();
private:
	/*!
	 *	逻辑处理器
	 */
	CZLogicCenter logic;

	/*!
	 *	逻辑处理单元数据缓存
	 */
	CZUChar *requestMessageBuffer;

	/*!
	 *	请求消息结构
	 */
	cz_types::RequestMessage *requestMessage;

	/*!
	 *	逻辑处理单元数据缓存
	 */
	CZUChar *responseMessageBuffer;

	/*!
	 *	请求消息结构
	 */
	cz_types::ResponseMessage *responseMessage;
};

#endif //#ifndef CZ_PROCESSORTHREAD_H
