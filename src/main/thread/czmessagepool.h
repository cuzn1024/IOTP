/*!
*   \brief      ��Ϣ��
*   \details    ���ڴ洢��վ���ն����账����Ϣ��δ�����ͻ��Ŀǰ���Ϊ���뵥�����������ܻῼ�Ƕ��̴߳���
*   \file       thread/czmessagepool.h
*   \author     cuzn
*   \version    0.1.0
*
* ������ʷ
*
* �汾��|˵��|�޶���|�޶�����
* ------|----|------|--------
* v0.1.0|����|cuzn|2016/04/07
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
 * \brief ��Ϣ����
 */
class CZMessagePool
{
public:
	/*!
	 * \brief ���캯��
	 */
    CZMessagePool();

public:
    
    /*!
     * \brief     ����������Ϣ��node
     * 
     * \param     CZUShort node_network_addr
     * \exception
     * \return    CZInt
     * \remarks  
     */
    CZInt addRequestNode(CZUShort node_network_addr);

	/*!
	 * \brief     ��ȡ��������Ҫ�����node�ڵ�
	 * 
	 * \exception
	 * \return    CZUShort 0 ��ʾû�� ��0��ʾ����node�������ַ
	 * \remarks  
	 */
	CZUShort getRequestNode();

	/*!
	 * \brief     ��ӷ�����Ϣ�ṹ
	 *
	 * \param     CZUShort session_id ��·ID
	 * \param     CZUShort dst_network_addr �����ַ
	 * \param     CZUChar data_id ����ID
	 * \param     CZUChar * data_buffer ��������ָ��
	 * \param     CZUShort data_len �������ݳ���
	 * \param     bool bNeedTimeoutRetry �Ƿ���Ҫ��ʱ�ش���Ĭ�ϲ���Ҫ
	 * \return    CZInt 0 ��ӳɹ� ��0���ʧ��
	 * \remarks  
	 */
	CZInt addResponseMessage(CZUShort session_id,CZUShort dst_network_addr,CZUChar data_id,CZUChar* data_buffer,CZUShort data_len,bool bNeedTimeoutRetry = false);
	
	/*!
	 * \brief     ��ȡ��������
	 * 
	 * \param     cz_types::ResponseMessage * responseMessage
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt getResponseMessage(cz_types::ResponseMessage* responseMessage);

	/*!
	 * \brief     ��ӳ�ʱ�ش�����
	 *
	 * \param     cz_types::TimeoutTask * task
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt addTimerTask(cz_types::TimerTask* task);

	/*!
	 * \brief     ɾ����ʱ�ش�����
	 *
	 * \param     CZUShort network_addr
	 * \param     CZUChar data_id
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt delTimerTask(CZUShort network_addr,CZUChar data_id);

	/*!
	 * \brief     ��ȡ��ʱ�ش�����
	 *
	 * \param     cz_types::TimeoutTask * task
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt getTimerTask(cz_types::TimerTask* task);

private:

	/*!
	 *	������Ϣ����
	 */
    boost::circular_buffer<CZUShort> requestNodePoolQueue;

	/*!
	 *	�洢һ����Ϣ
	 */
    boost::pool<> responseMessagePool;
	/*!
	 *	������Ϣ����
	 */
    boost::circular_buffer<CZUChar*> responseMessagePoolQueue;

	/*!
	 *	�洢һ����ʱ�ش�����
	 */
    boost::pool<> timeoutMsgPool;
	/*!
	 *	��ʱ�ش�����
	 */
    boost::circular_buffer<CZUChar*> timeoutMsgPoolQueue;
};

#endif //#ifndef CZ_MESSAGEPOOL_H
