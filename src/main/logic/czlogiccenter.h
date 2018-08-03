#ifndef CZLOGICCENTER_H
#define CZLOGICCENTER_H

#ifndef CZ_DEFINES_H
#include <defines.h>
#endif //#ifndef CZ_DEFINES_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#include <boost/function.hpp>

#define FUN_ENTRY_RETRY_TYPE boost::function3<int, cz_types::RequestMessage*,bool,bool>
#define ENTRY_RETRY_TYPE_BIND(entry) boost::bind(entry, this, _1,_2,_3)

enum eDataId
{
	ApReqJoinNwk			 = 0x08, 		/*! *	��վ�������� */
	EdReqJoinNwk			 = 0x09, 		/*! *	�ն��������� */
	GetWlQty			     = 0x0A,        /*! *	��ȡָ���ŵ������������� */
	SetApNwkConfig			 = 0x0B,        /*! *	���û�վ�������ñ� */
	SetEdNwkConfig			 = 0x0C,        /*! *	�����ն��������ñ� */
	NwkResource				 = 0x0D,        /*! *	�㲥������Դ�� */
	ReqAddOrDelEd			 = 0x0E,        /*! *	��M��վ��ӻ�ɾ��һ���ն� */
	ReqAddOrDelGroup		 = 0x0F,        /*! *	��R��վ��ӻ�ɾ��һ���� */
	GetSysTime				 = 0x10,        /*! *	��ȡϵͳʱ�� */
    SetApWorkTab             = 0x11,        /*! *	��վ��������� */
	EdActive    			 = 0x13,        /*! *	�ն˼�����Ϣ */
	Disconnect	             = 0x15,        /*! *   �Ͽ����� */		
    Fault                    = 0x16,        /*! *   �����ϱ� */		
    HopFreq                  = 0x17,        /*! *   ��Ƶ */
    ModifyEdNwkConfig        = 0x18,        /*! *   �޸��ն��������ñ� */
    GetDeviceInfo            = 0x20,        /*! *   ��ȡ�豸��Ϣ */
	Test                     = 0x55
};


/*!
 * \brief �߼�����
 */
class CZLogicCenter
{
public:
    /*!
     * \brief     ���캯��
     * 
     * \exception
     * \return    
     * \remarks  
     */
    CZLogicCenter();

    /*!
     * \brief     ��������
     * 
     * \exception
     * \return    
     * \remarks  
     */
    ~CZLogicCenter();
	
	/*!
	 * \brief     ��ʼ��
	 * 
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	void init();

	/*!
	 * \brief     �ַ�����������
	 * 
	 * \param     cz_types::RequestMessage * requestMessage ��Դ��Ϣ
	 * \exception
	 * \return    int 
	 * \remarks  
	 */
	int processMessage(cz_types::RequestMessage* requestMessage);
private:

	/*!
	 * \brief     ����Ƿ�Ϊ�ش��߼�
	 *
	 * \param     CZUChar data_id
	 * \exception
	 * \return    bool
	 * \remarks  
	 */
	bool bRetryLogic(CZUChar data_id);

	/*!
	 * \brief     ע���߼�������
	 * 
	 * \param     cz_defines::eDataId data_id ����ID
	 * \param     FUN_ENTRY_TYPE entry
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	inline void registerEntry(eDataId data_id, FUN_ENTRY_TYPE entry) { logicTable[data_id] = entry; }

	/*!
	 * \brief     ע���ش��߼�������
	 * 
	 * \param     cz_defines::eDataId data_id ����ID
	 * \param     FUN_ENTRY_TYPE entry
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	inline void registerRetryEntry(eDataId data_id, FUN_ENTRY_RETRY_TYPE entry) { retryLogicTable[data_id] = entry; }

	/*!
	 * \brief     �㲥������Դ��
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \param     bool bServerCall
	 * \param     bool bNeedReply
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int nwkResource(cz_types::RequestMessage* requestMessage,bool bServerCall,bool bNeedReply);

	/*!
	 * \brief     ��ȡ����������
	 *
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int getWlQty(cz_types::RequestMessage* requestMessage,bool bServerCall = true,bool bNeedReply = true);

    int getDeviceInfo(cz_types::RequestMessage *requestMessage);

	/*!
	 * \brief     �����ն��������ñ�
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \param     bool bServerCall
	 * \param     bool bNeedReply
	 * \exception
	 * \return    int
	 * \remarks  ��Ӧָ��0x33
	 */
	CZInt setEdNwkConfig(cz_types::RequestMessage* requestMessage,bool bServerCall = true,bool bNeedReply = true);

    CZInt setModifyEdNwkConfig(cz_types::RequestMessage* requestMessage,bool bServerCall = true,bool bNeedReply = true);

	/*!
	 * \brief     ���û�վ�������ñ�
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \param     bool bServerCall
	 * \param     bool bNeedReply
	 * \exception
	 * \return    int
	 * \remarks  ��Ӧָ��0x35
     */
    CZInt setApNwkConfig(cz_types::RequestMessage* requestMessage, bool bServerCall = true, bool bNeedReply = true);

	
	int test(cz_types::RequestMessage* requestMessage,bool bServerCall = true,bool bNeedReply = true);

	/*!
	 * \brief     ��վ���������
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \param     bool bServerCall
	 * \param     bool bNeedReply
	 * \exception
	 * \return    int
	 * \remarks  ��Ӧָ��0x36
     */
    int setApWorkTab(cz_types::RequestMessage* requestMessage, bool bServerCall = true, bool bNeedReply = true);

	/*!
	 * \brief     �ն���������
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \exception
	 * \return    int
	 * \remarks   ��Ӧָ��0x38
	 */
	int edReqJoinNetwork(cz_types::RequestMessage* requestMessage);

	/*!
	 * \brief     ��վ��������
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \exception
	 * \return    int
	 * \remarks   ��Ӧָ��0x39
	 */
	int apReqJoinNetwork(cz_types::RequestMessage* requestMessage);

	/*!
	 * \brief     ��վ��������
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \exception
	 * \return    int
	 * \remarks   ��Ӧָ��0x39
	 */
	int getSysTime(cz_types::RequestMessage* requestMessage);

	/*!
	 * \brief     �ն˽��빤��̬
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int edActive(cz_types::RequestMessage* requestMessage);

    CZInt hopFreq(cz_types::RequestMessage* requestMessage, bool bServerCall = true, bool bNeedReply = true);
	
	/*!
	 * \brief     �յ��Ͽ�����
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int disconnectAck(cz_types::RequestMessage* requestMessage);

	/*!
	 * \brief     �յ������ϱ�
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int faultReport(cz_types::RequestMessage* requestMessage);

	/*!
	 * \brief     �����Ͽ�����
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \param     bool bServerCall
	 * \param     bool bNeedReply
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int disconnect(cz_types::RequestMessage* requestMessage, bool bServerCall = true, bool bNeedReply = true);

	/*!
	 * \brief     ֪ͨM��վ����ն�
	 *
	 * \param     CZUShort siteNetworkAddr
	 * \param     CZUShort edNetworkAddr
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int noticeMSiteAddDevice(CZUShort siteNetworkAddr, CZUShort edNetworkAddr, bool add);

	/*!
	 * \brief     ��M��վ��ӻ�ɾ��һ���ն�
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \param     bool bServerCall
	 * \param     bool bNeedReply
	 * \exception
	 * \return    int
	 * \remarks  ��Ӧָ��0x3A
	 */
	int reqAddOrDelEd(cz_types::RequestMessage* requestMessage,bool bServerCall = true,bool bNeedReply = true);

	/*!
	 * \brief     ֪ͨR��վ��ӷ���
	 *
	 * \param     CZUShort siteNetworkAddr
	 * \param     CZUShort groupId
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int noticeRSiteAddGroup(CZUShort siteNetworkAddr,CZUShort groupId);

	/*!
	 * \brief     ��R��վ��ӻ�ɾ��һ����
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \param     bool bServerCall
	 * \param     bool bNeedReply
	 * \exception
	 * \return    int
	 * \remarks  ��Ӧָ��0x3B
	 */
	int reqAddOrDelGroup(cz_types::RequestMessage* requestMessage,bool bServerCall = true,bool bNeedReply = true);

	/*!
	 * \brief     ��Ӷ�ʱ����
	 *
	 * \param     CZUShort dst_network_addr
	 * \param     CZUChar data_id
	 * \param     CZUChar * data_buffer
	 * \param     CZUShort data_len
	 * \param     CZUShort seconds
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int addTimerTask(CZUShort dst_network_addr, CZUShort session_id, CZUChar data_id,CZUChar* data_buffer,CZUShort data_len,CZUShort seconds);

	/*!
	 * \brief     ԭ·��������
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \param     CZUChar * data_buffer
	 * \param     CZUShort data_len
	 * \param     bool bNeedReply ��Ҫ�ظ� ���ڶ�ʱ�ش�
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int sendMessage(cz_types::RequestMessage* requestMessage,CZUChar* data_buffer,CZUShort data_len,bool bNeedReply=false);

	/*!
	 * \brief     ������Ϣ���ڵ�
	 * \param     CZUShort sessionId ��·id
	 * \param     CZUShort dst_network_addr �����ַ
	 * \param     CZUChar data_id ����ID
	 * \param     CZUChar * data_buffer ��������ָ��
	 * \param     CZUShort data_len �������ݳ���
	 * \param     bool bNeedReply ��Ҫ�ظ� ���ڶ�ʱ�ش�
	 * \return    int 0 ����  ��0�쳣
	 * \remarks  
	 */
	int sendMessage(CZUShort sessionId,CZUShort dst_network_addr,CZUChar data_id,CZUChar* data_buffer,CZUShort data_len,bool bNeedReply=false);
	
	/*!
	 * \brief     �㲥�����л�վ
	 *
	 * \param     CZUChar data_id
	 * \param     CZUChar * data_buffer
	 * \param     CZUShort data_len
	 * \param     bool bNeedReply
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int broadcastToAllSite(CZUChar data_id,CZUChar* data_buffer,CZUShort data_len,bool bNeedReply = false);

private:	
	/*!
	 *	�߼�������
	 */
	boost::function1<int, cz_types::RequestMessage*> logicTable[255];

	/*!
	 *	�ش��߼�
	 */
	boost::function3<int, cz_types::RequestMessage*,bool,bool> retryLogicTable[255];

	/*!
	 *	���ؽ������
	 */
	CZUChar *timerTaskBuffer;

	/*!
	 *	��ʱ����ṹ
	 */
	cz_types::TimerTask* task;

	/*!
	 *	���ؽ������
	 */
	CZUChar *responseBuffer;

	/*!
	 *	���ؽ������
	 */
	CZUShort bufferLen;
};

#endif //#ifndef CZLOGICCENTER_H
