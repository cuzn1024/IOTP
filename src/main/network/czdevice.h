#ifndef CZ_DEVICE_H
#define CZ_DEVICE_H

#ifndef CZ_NODE_H
#include <network/cznode.h>
#endif //#ifndef CZ_NODE_H

/*!
 * \brief 基站、终端等节点的抽象
 */
class CZDevice : public CZNode
{
    typedef std::unordered_map<CZUChar, std::vector<CZUShort>> NodeMap;
    typedef NodeMap::iterator NodeMapIter;
public:
   
    CZDevice(CZUShort _network_addr, const CZPhysicalAddr &_phy_addr, cz_types::eNodeType node_type);
    ~CZDevice();
	
public:

	/*!
	* \brief     设置终端优选基站结果
	*
	* \param     eNodeType nodeType 频点对应基站类型
	* \param     std::vector<CZUShort> & vec freid集合
	* \exception
	* \return    CZInt 0 成功 非0失败
	* \remarks
	*/
	virtual CZInt setCanReachSites(cz_types::eNodeType nodeType, std::vector<CZUShort>& vec);

	/*!
	* \brief     获取优选频点
	*
	* \exception
	* \return    std::map<CZUChar,std::vector<CZUShort>>
	* \remarks
	*/
	virtual CZInt getCanReachSites(cz_types::eNodeType nodeType, std::vector<CZUShort> *&vec);

	/*!
	 * \brief     删除某个类型的基站
	 *
	 * \param     cz_types::eNodeType nodeType
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	virtual CZInt delCanReachSites(cz_types::eNodeType nodeType);

private:

	/*!
	*	终端能扫描到的基站信息
	*/
	NodeMap reachSitesMap;
};

#endif //#ifndef CZ_DEVICE_H
