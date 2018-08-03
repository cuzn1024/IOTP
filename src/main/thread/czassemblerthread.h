/*!
*   \brief      消息封装线程
*   \details    用于调用会话封装消息。
*   \file       thread/czassemblerthread.h
*   \author     cuzn
*   \version    0.1.0
*
* 更新历史
*
* 版本号|说明|修订者|修订日期
* ------|----|------|--------
* v0.1.0|创建|cuzn|2016/04/10
*
*/
#ifndef CZ_ASSEMBLERTHREAD_H
#define CZ_ASSEMBLERTHREAD_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#ifndef CZ_ITHREAD_H
#include <thread/czithread.h>
#endif //#ifndef CZ_ITHREAD_H

extern "C"
{
#include <resolver/pro_l.h>
};

/*!
 * \brief 解包、组包、分包等
 */
class CZAssemblerThread : public CZIThread
{
public:

    /*!
     * \brief     构造函数
     *
     */
    CZAssemblerThread();

    /*!
     * \brief     析构函数
	 *
     */
    ~CZAssemblerThread();

protected:

    /*!
     * \brief     解包、组包、分包
     * 
     */
    void run();
private:

	/*!
	 * \brief     获取发送帧ID
	 *
	 * \return    CZUChar
	 * \remarks  
	 */
	CZUChar getFrameId();

	/*!
	 * \brief     重置frame缓存结构
	 *
	 * \param     LFrame_t * frame
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	void resetFrame(LFrame_t* frame);
private:
	/*!
	 *	读取数据缓存
	 */
	CZUChar *readFrameBuffer;

	/*!
	 *	请求帧结构
	 */
	cz_types::RequestFrame *requestFrame;

	/*!
	 *	发送数据缓存
	 */
	CZUChar *writeFrameBuffer;

	/*!
	 *	返回帧结构
	 */
	cz_types::ResponseFrame *responseFrame;

	/*!
	 *	发送数据缓存
	 */
	CZUChar *readMessageBuffer;

	/*!
	 *	返回帧结构
	 */
	cz_types::RequestMessage *requestMessage;

	/*!
	 *	发送数据缓存
	 */
	CZUChar *writeMessageBuffer;

	/*!
	 *	返回帧结构
	 */
	cz_types::ResponseMessage *responseMessage;

	/*!
	 *	帧数据缓存 
	 */
	CZUChar *frameBuffer;

	/*!
	 *	帧结构
	 */
	LFrame_t *frame;

	/*!
	 *	帧数据缓存 
	 */
	CZUChar *frameSendBuffer;

	/*!
	 *	帧结构
	 */
	LFrame_t *frameSend;

	/*!
	 *	发送帧序号
	 */
	CZUChar sendFrameId;
};

#endif //#ifndef CZ_ASSEMBLERTHREAD_H
