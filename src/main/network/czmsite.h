#ifndef CZ_MSITE_H
#define CZ_MSITE_H


#ifndef CZ_SITE_H
#include <network/czsite.h>
#endif //#ifndef CZ_SITE_H

/*!
 * \brief 基站、终端等节点的抽象
 */
class CZMSite:public CZSite
{
public:
   
    CZMSite(CZUShort _network_addr, const CZPhysicalAddr &_phy_addr, cz_types::eNodeType node_type);
    ~CZMSite();
	
public:
	/*!
	 * \brief     添加一个终端到管理列表
	 *
	 * \param     CZUShort ed_network_addr
	 * \param     bool inService 是否服务
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	virtual CZInt addEdToManagerList(CZUShort ed_network_addr,bool inService);

	/*!
	 * \brief     从管理列表中删除一个终端
	 *
	 * \param     CZUShort ed_network_addr
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	virtual CZInt delEdFromManagerList(CZUShort ed_network_addr);

	/*!
	 * \brief     获取M基站管理的终端列表
	 *
	 * \param     std::vector<CZUShort> & edList
	 * \param     bool bInService true表示获取正在服务的对象 否则获取全部
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	virtual CZInt getEdManagerList(std::vector<CZUShort> &edList,bool bInService = true);

private:

	/*!
	 *	Key 网络地址  Value 是否服务中
	 */
	std::unordered_map<CZUShort,bool> managerEdList;
};

#endif //#ifndef CZ_NODE_H
