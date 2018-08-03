#ifndef CZ_MSITE_H
#define CZ_MSITE_H


#ifndef CZ_SITE_H
#include <network/czsite.h>
#endif //#ifndef CZ_SITE_H

/*!
 * \brief ��վ���ն˵Ƚڵ�ĳ���
 */
class CZMSite:public CZSite
{
public:
   
    CZMSite(CZUShort _network_addr, const CZPhysicalAddr &_phy_addr, cz_types::eNodeType node_type);
    ~CZMSite();
	
public:
	/*!
	 * \brief     ���һ���ն˵������б�
	 *
	 * \param     CZUShort ed_network_addr
	 * \param     bool inService �Ƿ����
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	virtual CZInt addEdToManagerList(CZUShort ed_network_addr,bool inService);

	/*!
	 * \brief     �ӹ����б���ɾ��һ���ն�
	 *
	 * \param     CZUShort ed_network_addr
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	virtual CZInt delEdFromManagerList(CZUShort ed_network_addr);

	/*!
	 * \brief     ��ȡM��վ������ն��б�
	 *
	 * \param     std::vector<CZUShort> & edList
	 * \param     bool bInService true��ʾ��ȡ���ڷ���Ķ��� �����ȡȫ��
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	virtual CZInt getEdManagerList(std::vector<CZUShort> &edList,bool bInService = true);

private:

	/*!
	 *	Key �����ַ  Value �Ƿ������
	 */
	std::unordered_map<CZUShort,bool> managerEdList;
};

#endif //#ifndef CZ_NODE_H
