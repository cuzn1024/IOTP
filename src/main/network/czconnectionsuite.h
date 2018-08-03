/*!
*   \brief      ���ӹ�����
*   \details    ���ڹ������лỰ���ӡ�
*   \file       network/czconnectionsuite.h
*   \author     cuzn
*   \version    0.1.0
*
* ������ʷ
*
* �汾��|˵��|�޶���|�޶�����
* ------|----|------|--------
* v0.1.0|����|cuzn|2016/04/18
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
 * \brief ���ӹ���������Ϊ����ģ������ݹ���Ŧ��
 *
 * ������·���Session
 * ����������ն�Node�ڵ�
 * ����ͨ����Ϣ����
 */
class CZConnectionSuite
{
    friend class CZSingletonManager;
private:
    CZConnectionSuite();
    virtual ~CZConnectionSuite();

public:
    /*! \brief  ��������ʼ��
    *   <pre>��ʼ���������ӡ�
    *   ��ʼ�����ڼ����� </pre>
    */
    CZInt initialize();

    /*!
     * \brief     ��ʼ������session
     * \return    void
     */
    void initializeSessionPool();

    /*!
     * \brief     ���ĳ�������Ƿ����
     * 
     * \param     const std::string & port
     * \return    bool
     */
    bool serialportAvailable(const std::string &port);

    /*!
     * \brief     ��������Ƿ����µ�����
     *
     * \exception
     * \return    void
     */
    void acceptConnection();

	/*!
	 * \brief     �ӽ��ն����л�ȡһ��frame����
	 * 
	 * \param     cz_types::RequestFrame *&request frame�洢
	 * \param     CZInt & length	���ݳ���
	 * \return    CZInt 0 ���� ��0�쳣
	 */
	CZInt getFrameFromRecvQueue(cz_types::RequestFrame *&request);

	/*!
	 * \brief     �����Ϣ�����Ͷ���
	 * 
	 * \param     cz_types::ResponseFrame * rsp ��������Ϣ
	 * \return    CZInt CZInt 0 ���� ��0�쳣
	 */
	CZInt addMsgToSendQueue(cz_types::ResponseFrame* rsp);

	/*!
	 * \brief     ��鳬ʱ
	 *
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	void checkTimeout();

	/*!
	 * \brief     �ײ���·��ȡ����(���ڻ�����)
	 * 
	 * \return    void
	 */
	void collectData();

	/*!
	 * \brief     ���Ͷ����д��ڵ�����
	 * 
	 * \return    void
	 */
	void sendData();
private:
	

public:

    /*!
     * \brief     ��ʼ������
     * 
     * \exception
     * \return    void
     * \remarks  
     */
    void establishSerialPortPool();

    /*!
     * \brief     ��ʼ��tcp����
     * 
     * \exception
     * \return    void
     * \remarks  
     */
    CZInt establishNetworkListener();

private:

	/*!
	 *	˽�л������� ��Ҫ��;����ͷ�ļ�include����
	 */
    CZConnectionSuite_Private *p;	
};

#endif // CZ_CONNECTIONSUITE_H
