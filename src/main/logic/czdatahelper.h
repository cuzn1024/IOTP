#ifndef CZ_DATA_HELPER_H
#define CZ_DATA_HELPER_H

#ifndef CZ_DEFINES_H
#include <defines.h>
#endif //#ifndef CZ_DEFINES_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#ifndef BOOST_PROPERTY_TREE_PTREE_HPP_INCLUDED
#include <boost/property_tree/ptree.hpp>
#endif //#ifndef BOOST_PROPERTY_TREE_PTREE_HPP_INCLUDED

#ifndef BOOST_UNORDERED_MAP_HPP_INCLUDED
#include <boost/unordered_map.hpp>
#endif //#ifndef BOOST_UNORDERED_MAP_HPP_INCLUDED

#include <network/cznodemanager.h>
#include <sqlite3.h>

using namespace cz_types;
using namespace cz_defines;

#define SQLITE_THREADSAFE 2

/*!
 * \brief 逻辑处理
 */
class CZDataHelper
{
public:

	/*!
	 * \brief     构造函数
	 *
	 * \exception
	 * \return    
	 * \remarks  
	 */
	CZDataHelper();

	/*!
	 * \brief     析构函数
	 *
	 * \exception
	 * \return    
	 * \remarks  
	 */
	~CZDataHelper();

	/*!
	 * \brief     初始化
	 *
	 * \exception
	 * \return    CZUInt
	 * \remarks  
	 */
	CZUInt init();

	/*!
	 * \brief     首次初始化数据
	 *
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt initData();

	/*!
	 * \brief     设置服务器配置
	 *
	 * \param     const std::string &key
	 * \param     const std::string &value
	 * \exception
	 * \return    CZInt
	 * \remarks  一般情况下不设置配置，只有少数项设置
	 */
	CZInt setServerConfig(const std::string &key,const std::string &value);

	/*!
	 * \brief     获取服务器配置
	 *
	 * \param     const std::string &key
	 * \exception
	 * \return    std::string
	 * \remarks  
	 */
	std::string getServerConfig(const std::string &key);
	
	/*!
	 * \brief     加载允许入网基站配置
	 *
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt loadAllSiteAllow();

	/*!
	 * \brief     加载节点信息
	 *
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt loadAllNodeInfo();
	
	/*!
	 * \brief     加载频点信息
	 *
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt loadAllFreidInfo();

	/*!
	 * \brief     加载m基站工作任务表
	 *
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt loadAllMSiteWork();

	/*!
	 * \brief     加载r基站工作任务表
	 *
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt loadAllRSiteWork();

	/*!
	* \brief     添加基站允许入网
	*
	* \param     const std::string &phyAddr
	* \param     eNodeType type
	* \exception
	* \return    CZInt
	* \remarks
	*/
	CZInt addSiteAllow(const std::string &phyAddr, eNodeType type);

	/*!
	 * \brief     添加一个节点
	 *
	 * \param     CZUShort networkAddr
	 * \param     const std::string &phyAddr
	 * \param     eNodeType type
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt createNode(CZUShort networkAddr,const std::string &phyAddr,eNodeType type);

	/*!
	* \brief     设置频点信息
	*
	* \param     CZUShort freId
	* \param     CZUShort networkAddr
	* \param     ,eNodeType nodeType
	* \exception
	* \return    CZInt
	* \remarks
	*/
	CZInt setFreidInfo(CZUShort freId, CZUShort networkAddr, eNodeType nodeType);

	/*!
	 * \brief     添加到M基站管理列表 
	 *
	 * \param     CZUShort site_networkAddr
	 * \param     CZUShort ed_networkAddr
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt addToMSiteList(CZUShort site_networkAddr, CZUShort ed_networkAddr, bool add);

	/*!
	 * \brief     从M基站管理列表中删除某个终端
	 *
	 * \param     CZUShort site_networkAddr
	 * \param     CZUShort ed_networkAddr
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt delFromMSiteList(CZUShort site_networkAddr,CZUShort ed_networkAddr);

	/*!
	 * \brief     设置哪个为正在服务
	 *
	 * \param     CZUShort site_networkAddr
	 * \param     CZUShort ed_networkAddr
	 * \param     bool InService
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt setMSiteInservice(CZUShort site_networkAddr,CZUShort ed_networkAddr,bool InService);
	
	/*!
	 * \brief     添加到R基站分组
	 *
	 * \param     CZUShort site_networkAddr
	 * \param     CZUShort groupId
	 * \param     CZUChar postion
	 * \param     CZUShort ed_networkAddr
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt addToRSiteGroup(CZUShort site_networkAddr,CZUShort ed_networkAddr);

	/*!
	 * \brief     从R基站中删除一个终端
	 *
	 * \param     CZUShort site_networkAddr
	 * \param     CZUShort ed_networkAddr
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt delRSiteGroupNode(CZUShort site_networkAddr,CZUShort ed_networkAddr);

	/*!
	 * \brief     更改服务状态
	 *
	 * \param     CZUShort site_networkAddr
	 * \param     CZUShort ed_networkAddr
	 * \param     bool InService
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt setRSiteInservice(CZUShort site_networkAddr,CZUShort ed_networkAddr,bool InService);

	/*!
	 * \brief     设置某个终端在分组中的位置信息
	 *
	 * \param     CZUShort site_networkAddr
	 * \param     CZUShort ed_networkAddr
	 * \param     CZUShort groupId
	 * \param     CZUChar pos
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt setRSiteGroupInfo(CZUShort site_networkAddr,CZUShort ed_networkAddr,CZUShort groupId,CZUChar pos);

private:

	/*!
	 * \brief     执行sql语句
	 *
	 * \param     const std::string &sql
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int exeSql(const std::string &sql);

private:

	//作为静态项存储
	/*!
	 *	网络ID
	 */
	CZUShort network_id;

	/*!
	 *	网络状态
	 */
	CZUChar network_status;

	/*!
	 *	基站默认发送功率
	 */
	CZUChar siteTxPower;

	/*!
	 *	db存储路径
	 */
	std::string dbPath;

	/*!
	 *	数据库对象
	 */
	sqlite3 *db;

	/*!
	 *	sql语句
	 */
	std::string sql;
};

#endif //#ifndef CZ_DATA_HELPER_H
