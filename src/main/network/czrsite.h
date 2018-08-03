#ifndef CZ_RSITE_H
#define CZ_RSITE_H

#ifndef CZ_SITE_H
#include <network/czsite.h>
#endif //#ifndef CZ_SITE_H

/*!
 * \brief 基站、终端等节点的抽象
 */
class CZRSite:public CZSite
{
public:
   
    CZRSite(CZUShort _network_addr, const CZPhysicalAddr &_phy_addr, cz_types::eNodeType node_type);
    ~CZRSite();
public:

	/*!
	 * \brief     R基站添加一个终端到分组中
	 *
	 * \param     CZUShort ed_network_addr 终端网络地址
	 * \param     CZUShort & groupId 分配的组号
	 * \param     CZUChar & pos 分配组中的位置
	 * \param     bool& bGroupChange 组是否变化
	 * \exception
	 * \return    CZInt 0添加成功 非0添加失败
	 * \remarks  
	 */
	virtual CZInt addEdToGroup(CZUShort ed_network_addr,CZUShort& groupId,CZUChar& pos,bool& bGroupChange,bool isUseDesignGroup = false);

	/*!
	 * \brief     通过位置信息查找对应的终端
	 *
	 * \param     CZUShort groupId
	 * \param     CZUChar pos
	 * \exception
	 * \return    CZUShort 0 表示未找到 非0表示对应的网络地址
	 * \remarks  
	 */
	virtual CZUShort findEdByPos(CZUShort groupId,CZUChar pos);

	/*!
	 * \brief     查找终端对应的R分组信息
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
	 * \brief     获取R基站当前分组数
	 *
	 * \exception
	 * \return    CZUShort
	 * \remarks  
	 */
	virtual inline CZUShort getRsiteCurrentGroupSize(){return currentGroupSize;}

private:
	
	/*!
	 * \brief     升唯  映射关系用
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
	 * \brief     降维 映射关系用
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
	 *	当前组数
	 */
	CZUShort currentGroupSize;

	/*!
	 *	记录基站分组信息
	 *  key:currentGroupId * 1000 + currentGroupMemberSize value:networkAddr
	 */
	std::unordered_map<CZInt, CZUShort> groupInfoMap;

	/*!
	 *	记录终端对应的位置信息
	 */
	std::unordered_map<CZUShort, CZInt> edInfoMap;
};

#endif //#ifndef CZ_RSITE_H
