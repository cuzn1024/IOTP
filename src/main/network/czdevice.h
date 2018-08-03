#ifndef CZ_DEVICE_H
#define CZ_DEVICE_H

#ifndef CZ_NODE_H
#include <network/cznode.h>
#endif //#ifndef CZ_NODE_H

/*!
 * \brief ��վ���ն˵Ƚڵ�ĳ���
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
	* \brief     �����ն���ѡ��վ���
	*
	* \param     eNodeType nodeType Ƶ���Ӧ��վ����
	* \param     std::vector<CZUShort> & vec freid����
	* \exception
	* \return    CZInt 0 �ɹ� ��0ʧ��
	* \remarks
	*/
	virtual CZInt setCanReachSites(cz_types::eNodeType nodeType, std::vector<CZUShort>& vec);

	/*!
	* \brief     ��ȡ��ѡƵ��
	*
	* \exception
	* \return    std::map<CZUChar,std::vector<CZUShort>>
	* \remarks
	*/
	virtual CZInt getCanReachSites(cz_types::eNodeType nodeType, std::vector<CZUShort> *&vec);

	/*!
	 * \brief     ɾ��ĳ�����͵Ļ�վ
	 *
	 * \param     cz_types::eNodeType nodeType
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	virtual CZInt delCanReachSites(cz_types::eNodeType nodeType);

private:

	/*!
	*	�ն���ɨ�赽�Ļ�վ��Ϣ
	*/
	NodeMap reachSitesMap;
};

#endif //#ifndef CZ_DEVICE_H
