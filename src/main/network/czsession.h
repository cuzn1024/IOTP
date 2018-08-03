#ifndef CZ_SESSION_H
#define CZ_SESSION_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#ifndef CZ_DEFINES_H
#include <defines.h>
#endif //#ifndef CZ_DEFINES_H

class CZITransceiverInterface;

/*!
 * \brief �������ͻ�վ��·�ĳ���
 */
class CZSession
{
public:
	/*!
	 * \brief     session���캯��
	 * 
	 * \param     CZITransceiverInterface * transceiverInterface �ײ���·����
	 * \param     CZUInt session_id Ψһ��ʶ
	 */
	CZSession(CZITransceiverInterface *transceiverInterface,CZUInt session_id);

    /*!
     * \brief    session��������
     */
    ~CZSession();

public:
	/*!
	 * \brief     ��ȡ��·����������(����&����)
	 */
    CZInt collectData();

	/*!
	 * \brief     ��ȡָ����������
	 *
	 * \param     CZUChar receiveBuffer[]
	 * \param     CZUInt length
     * \param     bool dropData �Ƿ�ֱ�Ӷ���
	 * \exception
	 * \return    CZInt
	 * \remarks   ����ȡ��ɾ��ԭ����
	 */
	CZInt getData(CZUChar receiveBuffer[], CZUInt length);
    

	/*!
	 * \brief     �ƶ���ָ��
	 *
	 * \param     CZUInt length
	 * \exception
	 * \return    CZInt
	 * \remarks   
	 */
    CZInt move(CZUInt length);

	/*!
	 * \brief     ����һ֡����
	 * 
	 * \param     CZUChar dataBuffer[] ���ݴ洢
	 * \param     CZUInt length ���ݳ���
	 * \return    CZInt 0 �ɹ����� ��0��ʾ�쳣����
	 */
	CZInt sendFrame(CZUChar dataBuffer[], CZUInt length);

	/*!
	 * \brief     ��ȡsessionid
	 *
	 * \return    CZUInt
	 */
	inline CZUInt getId(){return id;}

	/*!
	 * \brief     ��ȡ״ֵ̬
	 * 
	 * \exception
	 * \return    eSessionStatus
	 * \remarks  
	 */
	inline cz_types::eSessionStatus getStatus(){return status;}

	/*!
	 * \brief     ����session״̬
	 *
	 * \param     eSessionStatus status
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	inline void setStatus(cz_types::eSessionStatus status){this->status = status;}

	/*!
	 * \brief     ˢ�½�������ʱ��
	 *
	 * \param     CZUInt time
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	inline void refreshLastRecvDataTime(CZUInt time){this->lastRecvDataTime = time;}

	/*!
	 * \brief     ��ȡ���ˢ��ʱ��
	 *
	 * \exception
	 * \return    CZUInt
	 * \remarks  
	 */
	inline CZUInt getLastRecvDataTime(){return lastRecvDataTime;}

	/*!
	 * \brief     �ر���·
	 *
	 * \exception
	 * \return    CZUInt
	 * \remarks  
	 */
	CZUInt close();

private:    
	/*!
	 *	�ײ���·������
	 */
    CZITransceiverInterface *transceiverInterface;

	/*!
	 *	session_id
	 */
	CZUInt id;

	/*!
	 *	��һ�ν��յ�������ʱ��
	 */
	CZUInt lastRecvDataTime;

	/*!
	 *	session״̬
	 */
	cz_types::eSessionStatus status;
};

#endif //#ifndef CZ_SESSION_H
