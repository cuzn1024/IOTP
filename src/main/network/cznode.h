#ifndef CZ_NODE_H
#define CZ_NODE_H

#ifndef _UNORDERED_MAP_
#include <unordered_map>
#endif //#ifndef _UNORDERED_MAP_

#ifndef _VECTOR_
#include <vector>
#endif //#ifndef _VECTOR_

#ifndef BOOST_POOL_HPP
#include <boost/pool/pool.hpp>
#endif //#ifndef BOOST_POOL_HPP

#ifndef BOOST_CIRCULAR_BUFFER_HPP
#include <boost/circular_buffer.hpp>
#endif //#ifndef BOOST_CIRCULAR_BUFFER_HPP

#ifndef CZ_DEFINES_H
#include <defines.h>
#endif //#ifndef CZ_DEFINES_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

class CZFrameBuffer;

extern "C"
{
#include <resolver/pro_l.h>
};

/*!
 * \brief �ڵ�Ļ���
 */
class CZNode
{
public:
    /*!
     * \brief     ���캯��
     * 
     * \param     CZUShort _network_addr �����ַ
     * \param     CZULongLong _phy_addr �����ַ
	 * \param     cz_types::eNodeType node_type ��ݱ�ʶ
     * \exception
     * \return    
     * \remarks  
     */
    CZNode(CZUShort _network_addr, const CZPhysicalAddr &_phy_addr,cz_types::eNodeType node_type);

    /*!
     * \brief     �ͷ��������Դ
     * 
     * \exception
     * \return    
     * \remarks  
     */
    ~CZNode();
	
public:
	/*!
	 * \brief     ���������ַ
	 * 
	 * \param     CZUShort _network_addr �����ַ
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	inline void setNetworkAddr(CZUShort _network_addr){network_addr = _network_addr;}

	/*!
	* \brief     ��ȡ�����ַ
	*
	* \exception
	* \return    CZUShort ��վ�ĵ�ַֻ��1byte���ն˵ĵ�ַ����2��byte
	* \remarks
	*/
	inline CZUShort getNetworkAddr() { return network_addr; }

	/*!
	 * \brief     ���������ַ
	 * 
	 * \param     CZUShort _phy_addr �����ַ
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	inline void setPhyAddr(const CZPhysicalAddr &_phy_addr) {phy_addr = _phy_addr;}

	/*!
	 * \brief     ��ȡ�����ַ
	 * 
	 * \exception
	 * \return    CZULongLong �����ַ
	 * \remarks  8��byte���
	 */
	inline const CZPhysicalAddr &getPhyAddr() const {return phy_addr;}

	/*!
	 * \brief     ��ȡ�ڵ�����
	 *
	 * \exception
	 * \return    cz_types::eNodeType ����Ľڵ�����
	 * \remarks  
	 */
	inline cz_types::eNodeType getType(){return type;}

	/*!
	* \brief     ���ýڵ�����
	*
	* \param     eNodeStatus status
	* \exception
	* \return    void
	* \remarks
	*/
	inline void setStatus(cz_types::eNodeStatus status) { this->status = status; }
	
	/*!
	 * \brief     ��ȡ�ڵ�״̬
	 *
	 * \exception
	 * \return    eNodeStatus
	 * \remarks  
	 */
	inline cz_types::eNodeStatus getStatus(){return status;}

	/*!
	* \brief     ���֡��node�ڵ�
	*
	* \param     LFrame_t * parseFrame ֡����
	* \param     CZUInt sessionId sessionid
	* \param     bool & bExistOkData �Ƿ������������
	* \param     bool & bNeedAck �Ƿ���ҪACK
	* \exception
	* \return    CZUShort 0 ���� 1 �����������쳣
	* \remarks
	*/
	CZUShort addFrame(LFrame_t* parseFrame, CZUInt sessionId, bool& bExistOkData, bool& bNeedAck);

	/*!
	 * \brief     ��node�����л�ȡһ��������Ϣ
	 *
	 * \param     cz_types::RequestMessage * requestMessage
	 * \exception
	 * \return    CZUShort 0 ��ʶ�� ��0��ʶû��
	 * \remarks  
	 */
	CZUShort getMessageFromNode(cz_types::RequestMessage* requestMessage);
	
	/*!
	* \brief     ���������Ϣ������
	*
	* \param     cz_types::RequestMessage * requestMessage ������Ϣ
	* \exception
	* \return    CZUShort
	* \remarks
	*/
	CZUShort addRequestMessageToQueue(cz_types::RequestMessage* requestMessage);

	/*!
	 * \brief     ��ӳ�ʱ�ش�����
	 * 
	 * \param     cz_types::TimerTask* task
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt addTimerTask(cz_types::TimerTask* task);

	/*!
	 * \brief     ɾ����ʱ�ش�����
	 * 
	 * \param     CZUChar data_id
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt delTimerTask(CZUChar data_id);

	/*!
	 * \brief     ����Ƿ��ɷ����������������� ��ֹ����
	 *
	 * \param     CZUChar data_id
	 * \exception
	 * \return    bool
	 * \remarks  
	 */
	bool checkIsHaveCase(CZUChar data_id);
	
	/*!
	 * \brief     ��ȡ��ʱ�ش�����
	 *
	 * \param     cz_types::TimerTask * task
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt getTimerTask(cz_types::TimerTask* task);

	/*!
	 * \brief     ���û���
	 *
	 * \param     const std::string &key
	 * \param     const std::string &value
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt setCache(const std::string &key, const std::string &value);

	/*!
	 * \brief     ��ȡ����
	 *
	 * \param     const std::string &key
	 * \param     std::string &value
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt getCache(const std::string &key, std::string &value,bool isDeleteThisKey=true);

	/*!
	* \brief     ��ȡ��·ID
	*
	* \exception
	* \return    CZUShort ��·ID
	* \remarks  ��Ӧsession��·����
	*/
	virtual inline CZUShort getSessionId() { return sessionId; }
	
	/*!
	 * \brief     ������·ID
	 *
	 * \param     CZUShort id
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	virtual inline void setSessionId(CZUShort id) { sessionId = id; }

public:
	/************************************************************************/
	/* ����Ϊ�麯��                                                         */
	/************************************************************************/

	/*!
	 *	czsite������ʵ��
	 */
	virtual CZUShort getFreid() { return 0 ;}

	/*!
	 *	czsite������ʵ��
	 */
	virtual void setFreId(CZUShort id){}

	/*!
	 *	csdevice������ʵ��
	 */
	virtual CZInt setCanReachSites(cz_types::eNodeType nodeType, std::vector<CZUShort>& vec) {return 1;}

	/*!
	 *	csdevice������ʵ��
	 */
	virtual CZInt getCanReachSites(cz_types::eNodeType nodeType,std::vector<CZUShort> *&vec) {return 1;}

	/*!
	 *	csdevice������ʵ��
	 */
	virtual CZInt delCanReachSites(cz_types::eNodeType nodeType){return 1;}

	/*!
	 *	czmsite������ʵ��
	 */
	virtual CZInt addEdToManagerList(CZUShort ed_network_addr,bool inService){return 1;}

	/*!
	 *	czmsite������ʵ��
	 */
	virtual CZInt delEdFromManagerList(CZUShort ed_network_addr){return 1;}

	/*!
	 *	czmsite������ʵ��
	 */
	virtual CZInt getEdManagerList(std::vector<CZUShort>& edList,bool bInService = true){return 1;}

	/*!
	 *	czrsite������ʵ��
	 */
	virtual CZInt addEdToGroup(CZUShort ed_network_addr,CZUShort& groupId,CZUChar& pos,bool& bGroupChange,bool isUseDesignGroup = false){return 1;}

	/*!
	 *	czrsite������ʵ��
	 */
	virtual CZUShort findEdByPos(CZUShort groupId,CZUChar pos){return 0;}

	/*!
	*	czrsite������ʵ��
	*/
	virtual CZInt findEdGroupInfo(CZUShort networkAddr, CZUShort& groupId, CZUChar& pos) { return 1; }

	/*!
	 *	czmsite������ʵ��
	 */
	virtual CZInt getMSiteLastOperAction(bool& bAdd,CZUShort& oper_network_addr){ return 1;}

	/*!
	*	czrsite������ʵ��
	*/
	virtual CZUShort getRsiteCurrentGroupSize(){return 0;}

    CZInt setFaultSite(CZUShort site);
    CZInt getFaultSite(CZUShort &site);

	/************************************************************************/
	/* ����Ϊ�麯��                                                         */
	/************************************************************************/

private:

	/*!
	*	��·id
	*/
	CZUShort sessionId;
	/*!
	 *	�����ַ
	 */
	CZUShort network_addr;

	/*!
	 *	�����ַ
	 */
	CZPhysicalAddr phy_addr;

	/*!
	 *	�ڵ�����
	 * 
	 *  ��ݱ�ʶ
	 */
	cz_types::eNodeType type;

	/*!
	 *	��ʱ���ݴ洢
	 */
	CZUChar *dataBuffer;

	/*!
	 *	���ڴ洢��ʱ����
	 */
	cz_types::RequestMessage* requestMessage;

	/*!
	 *	�洢һ����Ϣ
	 */
    boost::pool<> requestMessagePool;
	/*!
	 *	������Ϣ����
	 */
    boost::circular_buffer<CZUChar*> requestMessagePoolQueue;

	/*!
	 *	֡����
	 */
	CZFrameBuffer* pFrameBuffer;

	/*!
	 *	��Ϣid
	 */
	CZUShort mIndex;

	/*!
	 *	��ǰ�ڵ�״̬
	 */
	cz_types::eNodeStatus status;

	/*!
	 *	�洢һ����ʱ�ش�����
	 */
    boost::pool<> timeoutMsgPool;

	/*!
	 *	��ʱ�ش�����
	 */
	std::unordered_map<CZUChar, CZUChar*> timeoutTaskMap;

	/*!
	 *	����洢
	 */
	std::unordered_map<std::string, std::string> cacheMap;

    CZUShort faultSite;
};

#endif //#ifndef CZ_NODE_H
