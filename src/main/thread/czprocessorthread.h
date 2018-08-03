/*!
*   \brief      ��Ϣ�����߳�
*   \details    ���ڵ��û�վ�����װ�����Ϣ��δ�����ͻ��Ŀǰ���Ϊ��ʵ�����������ܻῼ�Ƕ�ʵ������
*   \file       thread/czprocessorthread.h
*   \author     cuzn
*   \version    0.1.0
*
* ������ʷ
*
* �汾��|˵��|�޶���|�޶�����
* ------|----|------|--------
* v0.1.0|����|cuzn|2016/04/05
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

/*! \brief  ��Ϣ�����߳�
*   <pre>���̴߳�\a CZMessagepool ��ȡ��װ�����Ϣ����������Ӧ�Ļ�վ���ն˽��д���
*   Ϊ�����ͻ�����ڵ���\a CZProcessorThread ʵ���з��ʻ�վ��Ϣ���͹��ܡ�
*   ���������Ʒ��ͻ�����У��ɿ��Ƕ��\a CZProcessorThread ʵ����</pre>
*/
class CZProcessorThread : public CZIThread
{
public:
    CZProcessorThread();
    ~CZProcessorThread(void);

protected:
    /*! <pre>�̴߳�����
    *   �����վ���ն˴�����Ϣ���ˣ������Ҳ�����������Դ��C��վ������������������������·���
    *   ��\a CZMessagePool ��ȡ��Ϣ��������վ���ն�ִ�в��·���
    *   �̳���\a CZIThread </pre>
    *   \see CZIThread
    */
    void run();
private:
	/*!
	 *	�߼�������
	 */
	CZLogicCenter logic;

	/*!
	 *	�߼�����Ԫ���ݻ���
	 */
	CZUChar *requestMessageBuffer;

	/*!
	 *	������Ϣ�ṹ
	 */
	cz_types::RequestMessage *requestMessage;

	/*!
	 *	�߼�����Ԫ���ݻ���
	 */
	CZUChar *responseMessageBuffer;

	/*!
	 *	������Ϣ�ṹ
	 */
	cz_types::ResponseMessage *responseMessage;
};

#endif //#ifndef CZ_PROCESSORTHREAD_H
