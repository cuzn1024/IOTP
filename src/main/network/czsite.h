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
 * \brief 基站等节点的抽象
 */
class CZSite:public CZNode
{
public:
   
    CZSite(CZUShort _network_addr, const CZPhysicalAddr &_phy_addr, cz_types::eNodeType node_type);
    ~CZSite();
	
public:
	/*!
	* \brief     获取工作频点
	*
	* \exception
	* \return    CZUShort
	* \remarks  仅基站有效
	*/
	virtual inline CZUShort getFreid() { return freId; }

	/*!
	* \brief     设置基站工作频点
	*
	* \param     CZUShort freId
	* \exception
	* \return    void
	* \remarks
	*/
	virtual inline void setFreId(CZUShort id) { freId = id; }

	/************************************************************************/
	/* 以下真正实现在具体类中                                               */
	/************************************************************************/

	/*!
	 *	czmsite中真正实现
	 */
	virtual CZInt addEdToManagerList(CZUShort ed_network_addr,bool inService){return 1;}

	/*!
	 *	czmsite中真正实现
	 */
	virtual CZInt delEdFromManagerList(CZUShort ed_network_addr){return 1;}

	/*!
	 *	czmsite中真正实现
	 */
	virtual CZInt getEdManagerList(std::vector<CZUShort>& edList,bool bInService = true){return 1;}

	/*!
	 *	czmsite中真正实现
	 */
	virtual CZInt getMSiteLastOperAction(bool& bAdd,CZUShort& oper_network_addr){ return 1;}

	/*!
	 *	czrsite中真正实现
	 */
	virtual CZInt addEdToGroup(CZUShort ed_network_addr,CZUShort& groupId,CZUChar& pos,bool& bGroupChange,bool isUseDesignGroup = false){return 1;}

	/*!
	 *	czrsite中真正实现
	 */
	virtual CZUShort findEdByPos(CZUShort groupId,CZUChar pos){return 0;}

	/*!
	 *	czrsite中真正实现
	 */
	virtual CZInt findEdGroupInfo(CZUShort networkAddr, CZUShort& groupId, CZUChar& pos) { return 1; }

	/*!
	 *	czrsite中真正实现
	 */
	virtual CZInt getRSiteLastGroupChangeInfo(bool& isGroupIdAdd,CZUShort& groupId){return 1;}

	/*!
	*	czrsite中真正实现
	*/
	virtual CZUShort getRsiteCurrentGroupSize(){return 0;}

	/************************************************************************/
	/* 以上真正实现在具体类中                                               */
	/************************************************************************/

private:

	/*!
	*	工作频点
	*/
	CZUShort freId;
	
};

#endif //#ifndef CZ_NODE_H
