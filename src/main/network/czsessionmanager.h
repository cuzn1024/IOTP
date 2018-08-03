#ifndef CZ_SESSION_MANAGER_H
#define CZ_SESSION_MANAGER_H

#ifndef _MEMORY_
#include <memory>
#endif //#ifndef _MEMORY_

#ifndef _UNORDERED_MAP_
#include <unordered_map>
#endif //#ifndef _UNORDERED_MAP_

#ifndef CZ_DEFINES_H
#include <defines.h>
#endif //#ifndef CZ_DEFINES_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

using namespace cz_types;
using namespace cz_defines;

class CZSession;
class CZITransceiverInterface;

class CZSessionManager
{
public:
	typedef std::unordered_map<CZUShort, std::shared_ptr<CZSession>> SessionPool;

	CZSessionManager();
	~CZSessionManager();
	CZInt init();

	/*!
	 * \brief     ����session
	 *
	 * \param     CZITransceiverInterface * transceiverInterface
	 * \exception
	 * \return    CZUInt
	 * \remarks  
	 */
	CZUInt createSession(CZITransceiverInterface *transceiverInterface);

	/*!
	 * \brief     ��ȡsession�洢
	 *
	 * \exception
	 * \return    CZSessionManager::SessionPool&
	 * \remarks  
	 */
	SessionPool& getSessionPool();

	/*!
	 * \brief     ���»�վ��Ϣ
	 *
	 * \param     CZUShort sessionId
	 * \param     CZUShort siteNetworkAddr
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt updateSiteInfo(CZUShort sessionId,CZUShort siteNetworkAddr);

	/*!
	 * \brief     ��ȡ��վ��ַ
	 *
	 * \param     CZUShort sessionId
	 * \exception
	 * \return    CZUShort
	 * \remarks  
	 */
	CZUShort getSiteAddr(CZUShort sessionId);

private:	
	CZInt addSession(CZSession* session);

	/*!
	 * \brief     ����һ�����õ�session_id
	 * 
	 * \return    CZUInt ���õ�session_id
	 * \remarks   ��Ҫ����Ƿ��ظ�
	 */
	CZUInt getSessionId();

	/*!
	 *	�洢session
	 */
	SessionPool sessionPool;

	/*!
	 *	����session_id��
	 */
	CZUInt sessionIndex;

	/*!
	 *	��¼session�����ĸ���վ
	 */
	std::unordered_map<CZUShort,CZUShort> sessionToSiteMap;
};

#endif //#ifndef CZ_SESSION_MANAGER_H
