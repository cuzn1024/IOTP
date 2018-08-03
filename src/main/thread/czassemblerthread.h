/*!
*   \brief      ��Ϣ��װ�߳�
*   \details    ���ڵ��ûỰ��װ��Ϣ��
*   \file       thread/czassemblerthread.h
*   \author     cuzn
*   \version    0.1.0
*
* ������ʷ
*
* �汾��|˵��|�޶���|�޶�����
* ------|----|------|--------
* v0.1.0|����|cuzn|2016/04/10
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
 * \brief �����������ְ���
 */
class CZAssemblerThread : public CZIThread
{
public:

    /*!
     * \brief     ���캯��
     *
     */
    CZAssemblerThread();

    /*!
     * \brief     ��������
	 *
     */
    ~CZAssemblerThread();

protected:

    /*!
     * \brief     �����������ְ�
     * 
     */
    void run();
private:

	/*!
	 * \brief     ��ȡ����֡ID
	 *
	 * \return    CZUChar
	 * \remarks  
	 */
	CZUChar getFrameId();

	/*!
	 * \brief     ����frame����ṹ
	 *
	 * \param     LFrame_t * frame
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	void resetFrame(LFrame_t* frame);
private:
	/*!
	 *	��ȡ���ݻ���
	 */
	CZUChar *readFrameBuffer;

	/*!
	 *	����֡�ṹ
	 */
	cz_types::RequestFrame *requestFrame;

	/*!
	 *	�������ݻ���
	 */
	CZUChar *writeFrameBuffer;

	/*!
	 *	����֡�ṹ
	 */
	cz_types::ResponseFrame *responseFrame;

	/*!
	 *	�������ݻ���
	 */
	CZUChar *readMessageBuffer;

	/*!
	 *	����֡�ṹ
	 */
	cz_types::RequestMessage *requestMessage;

	/*!
	 *	�������ݻ���
	 */
	CZUChar *writeMessageBuffer;

	/*!
	 *	����֡�ṹ
	 */
	cz_types::ResponseMessage *responseMessage;

	/*!
	 *	֡���ݻ��� 
	 */
	CZUChar *frameBuffer;

	/*!
	 *	֡�ṹ
	 */
	LFrame_t *frame;

	/*!
	 *	֡���ݻ��� 
	 */
	CZUChar *frameSendBuffer;

	/*!
	 *	֡�ṹ
	 */
	LFrame_t *frameSend;

	/*!
	 *	����֡���
	 */
	CZUChar sendFrameId;
};

#endif //#ifndef CZ_ASSEMBLERTHREAD_H
