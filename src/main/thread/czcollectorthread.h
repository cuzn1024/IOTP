/*!
*   \brief      ���ݲɼ��߳�
*   \details    ���ڵ��ûỰ�ɼ�����
*   \file       thread/czcollectorthread.h
*   \author     cuzn
*   \version    0.1.0
*
* ������ʷ
*
* �汾��|˵��|�޶���|�޶�����
* ------|----|------|--------
* v0.1.0|����|cuzn|2016/04/12
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
 *	\brief	��·����
 * 
 *	�ײ����ݷ��������
 *
 */
class CZCollectorThread : public CZIThread
{
public:
    /*!
     * \brief     ���캯��
     * 
     */
    CZCollectorThread(void);

    /*!
     * \brief     ��������
     * 
     */
    ~CZCollectorThread(void);

protected:
    

    /*!
     * \brief     �߳�����
     * 
     * \return    void
     * \remarks  
     */
    void run();
    
private:
    

    /*!
     * \brief     ���ڵ���س�ʼ��
     * 
	 * ��ʼ������session�ʹ�������session(�ػ�session �����ַΪ0xfffe)
	 *
     * \return    void
     * \remarks  
     */
    void serialportConnectionHandler();
    
	/*!
	 * \brief     ��鳬ʱ
	 *
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	void checkTimeout();

    /*!
     * \brief     ����Ƿ�����socket����
     * 
     * \return    void
     * \remarks  
     */
    void establishRequestHandler();

    /*!
     * \brief     ��·���ݽ���(�����뷢��)
     * 
     * \return    void
     * \remarks  
     */
    void processDataHandler();
};

#endif //#ifndef CZ_COLLECTORTHREAD_H
