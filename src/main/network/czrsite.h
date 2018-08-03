#ifndef CZ_RSITE_H
#define CZ_RSITE_H

#ifndef CZ_SITE_H
#include <network/czsite.h>
#endif //#ifndef CZ_SITE_H

/*!
 * \brief ��վ���ն˵Ƚڵ�ĳ���
 */
class CZRSite:public CZSite
{
public:
   
    CZRSite(CZUShort _network_addr, const CZPhysicalAddr &_phy_addr, cz_types::eNodeType node_type);
    ~CZRSite();
public:

	/*!
	 * \brief     R��վ���һ���ն˵�������
	 *
	 * \param     CZUShort ed_network_addr �ն������ַ
	 * \param     CZUShort & groupId ��������
	 * \param     CZUChar & pos �������е�λ��
	 * \param     bool& bGroupChange ���Ƿ�仯
	 * \exception
	 * \return    CZInt 0��ӳɹ� ��0���ʧ��
	 * \remarks  
	 */
	virtual CZInt addEdToGroup(CZUShort ed_network_addr,CZUShort& groupId,CZUChar& pos,bool& bGroupChange,bool isUseDesignGroup = false);

	/*!
	 * \brief     ͨ��λ����Ϣ���Ҷ�Ӧ���ն�
	 *
	 * \param     CZUShort groupId
	 * \param     CZUChar pos
	 * \exception
	 * \return    CZUShort 0 ��ʾδ�ҵ� ��0��ʾ��Ӧ�������ַ
	 * \remarks  
	 */
	virtual CZUShort findEdByPos(CZUShort groupId,CZUChar pos);

	/*!
	 * \brief     �����ն˶�Ӧ��R������Ϣ
	 * 
	 * \param     CZUShort networkAddr
	 * \param     CZUShort & groupId
	 * \param     CZUChar & pos
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	virtual CZInt findEdGroupInfo(CZUShort networkAddr,CZUShort& groupId, CZUChar& pos);

	/*!
	 * \brief     ��ȡR��վ��ǰ������
	 *
	 * \exception
	 * \return    CZUShort
	 * \remarks  
	 */
	virtual inline CZUShort getRsiteCurrentGroupSize(){return currentGroupSize;}

private:
	
	/*!
	 * \brief     ��Ψ  ӳ���ϵ��
	 * 
	 * \param     CZInt key
	 * \param     CZUShort & groupId
	 * \param     CZUChar & pos
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	void increaseDimension(CZInt key, CZUShort& groupId, CZUChar& pos);

	/*!
	 * \brief     ��ά ӳ���ϵ��
	 * 
	 * \param     CZUShort groupId
	 * \param     CZUChar pos
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt decreaseDimension(CZUShort groupId, CZUChar pos);
	
private:

	/*!
	 *	��ǰ����
	 */
	CZUShort currentGroupSize;

	/*!
	 *	��¼��վ������Ϣ
	 *  key:currentGroupId * 1000 + currentGroupMemberSize value:networkAddr
	 */
	std::unordered_map<CZInt, CZUShort> groupInfoMap;

	/*!
	 *	��¼�ն˶�Ӧ��λ����Ϣ
	 */
	std::unordered_map<CZUShort, CZInt> edInfoMap;
};

#endif //#ifndef CZ_RSITE_H
