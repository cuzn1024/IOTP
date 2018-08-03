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
 * \brief �߼�����
 */
class CZDataHelper
{
public:

	/*!
	 * \brief     ���캯��
	 *
	 * \exception
	 * \return    
	 * \remarks  
	 */
	CZDataHelper();

	/*!
	 * \brief     ��������
	 *
	 * \exception
	 * \return    
	 * \remarks  
	 */
	~CZDataHelper();

	/*!
	 * \brief     ��ʼ��
	 *
	 * \exception
	 * \return    CZUInt
	 * \remarks  
	 */
	CZUInt init();

	/*!
	 * \brief     �״γ�ʼ������
	 *
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt initData();

	/*!
	 * \brief     ���÷���������
	 *
	 * \param     const std::string &key
	 * \param     const std::string &value
	 * \exception
	 * \return    CZInt
	 * \remarks  һ������²��������ã�ֻ������������
	 */
	CZInt setServerConfig(const std::string &key,const std::string &value);

	/*!
	 * \brief     ��ȡ����������
	 *
	 * \param     const std::string &key
	 * \exception
	 * \return    std::string
	 * \remarks  
	 */
	std::string getServerConfig(const std::string &key);
	
	/*!
	 * \brief     ��������������վ����
	 *
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt loadAllSiteAllow();

	/*!
	 * \brief     ���ؽڵ���Ϣ
	 *
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt loadAllNodeInfo();
	
	/*!
	 * \brief     ����Ƶ����Ϣ
	 *
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt loadAllFreidInfo();

	/*!
	 * \brief     ����m��վ���������
	 *
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt loadAllMSiteWork();

	/*!
	 * \brief     ����r��վ���������
	 *
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt loadAllRSiteWork();

	/*!
	* \brief     ��ӻ�վ��������
	*
	* \param     const std::string &phyAddr
	* \param     eNodeType type
	* \exception
	* \return    CZInt
	* \remarks
	*/
	CZInt addSiteAllow(const std::string &phyAddr, eNodeType type);

	/*!
	 * \brief     ���һ���ڵ�
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
	* \brief     ����Ƶ����Ϣ
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
	 * \brief     ��ӵ�M��վ�����б� 
	 *
	 * \param     CZUShort site_networkAddr
	 * \param     CZUShort ed_networkAddr
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt addToMSiteList(CZUShort site_networkAddr, CZUShort ed_networkAddr, bool add);

	/*!
	 * \brief     ��M��վ�����б���ɾ��ĳ���ն�
	 *
	 * \param     CZUShort site_networkAddr
	 * \param     CZUShort ed_networkAddr
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt delFromMSiteList(CZUShort site_networkAddr,CZUShort ed_networkAddr);

	/*!
	 * \brief     �����ĸ�Ϊ���ڷ���
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
	 * \brief     ��ӵ�R��վ����
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
	 * \brief     ��R��վ��ɾ��һ���ն�
	 *
	 * \param     CZUShort site_networkAddr
	 * \param     CZUShort ed_networkAddr
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt delRSiteGroupNode(CZUShort site_networkAddr,CZUShort ed_networkAddr);

	/*!
	 * \brief     ���ķ���״̬
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
	 * \brief     ����ĳ���ն��ڷ����е�λ����Ϣ
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
	 * \brief     ִ��sql���
	 *
	 * \param     const std::string &sql
	 * \exception
	 * \return    int
	 * \remarks  
	 */
	int exeSql(const std::string &sql);

private:

	//��Ϊ��̬��洢
	/*!
	 *	����ID
	 */
	CZUShort network_id;

	/*!
	 *	����״̬
	 */
	CZUChar network_status;

	/*!
	 *	��վĬ�Ϸ��͹���
	 */
	CZUChar siteTxPower;

	/*!
	 *	db�洢·��
	 */
	std::string dbPath;

	/*!
	 *	���ݿ����
	 */
	sqlite3 *db;

	/*!
	 *	sql���
	 */
	std::string sql;
};

#endif //#ifndef CZ_DATA_HELPER_H
