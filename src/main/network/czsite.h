#ifndef CZ_SITE_H
#define CZ_SITE_H

#ifndef CZ_DEFINES_H
#include <defines.h>
#endif //#ifndef CZ_DEFINES_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#ifndef CZ_NODE_H
#include <network/cznode.h>
#endif //#ifndef CZ_NODE_H

/*!
 * \brief ��վ�Ƚڵ�ĳ���
 */
class CZSite:public CZNode
{
public:
   
    CZSite(CZUShort _network_addr, const CZPhysicalAddr &_phy_addr, cz_types::eNodeType node_type);
    ~CZSite();
	
public:
	/*!
	* \brief     ��ȡ����Ƶ��
	*
	* \exception
	* \return    CZUShort
	* \remarks  ����վ��Ч
	*/
	virtual inline CZUShort getFreid() { return freId; }

	/*!
	* \brief     ���û�վ����Ƶ��
	*
	* \param     CZUShort freId
	* \exception
	* \return    void
	* \remarks
	*/
	virtual inline void setFreId(CZUShort id) { freId = id; }

	/************************************************************************/
	/* ��������ʵ���ھ�������                                               */
	/************************************************************************/

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
	 *	czmsite������ʵ��
	 */
	virtual CZInt getMSiteLastOperAction(bool& bAdd,CZUShort& oper_network_addr){ return 1;}

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
	 *	czrsite������ʵ��
	 */
	virtual CZInt getRSiteLastGroupChangeInfo(bool& isGroupIdAdd,CZUShort& groupId){return 1;}

	/*!
	*	czrsite������ʵ��
	*/
	virtual CZUShort getRsiteCurrentGroupSize(){return 0;}

	/************************************************************************/
	/* ��������ʵ���ھ�������                                               */
	/************************************************************************/

private:

	/*!
	*	����Ƶ��
	*/
	CZUShort freId;
	
};

#endif //#ifndef CZ_NODE_H
