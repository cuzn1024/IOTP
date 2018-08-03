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
	 * \brief     创建session
	 *
	 * \param     CZITransceiverInterface * transceiverInterface
	 * \exception
	 * \return    CZUInt
	 * \remarks  
	 */
	CZUInt createSession(CZITransceiverInterface *transceiverInterface);

	/*!
	 * \brief     获取session存储
	 *
	 * \exception
	 * \return    CZSessionManager::SessionPool&
	 * \remarks  
	 */
	SessionPool& getSessionPool();

	/*!
	 * \brief     更新基站信息
	 *
	 * \param     CZUShort sessionId
	 * \param     CZUShort siteNetworkAddr
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt updateSiteInfo(CZUShort sessionId,CZUShort siteNetworkAddr);

	/*!
	 * \brief     获取基站地址
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
	 * \brief     分配一个可用的session_id
	 * 
	 * \return    CZUInt 可用的session_id
	 * \remarks   需要检测是否重复
	 */
	CZUInt getSessionId();

	/*!
	 *	存储session
	 */
	SessionPool sessionPool;

	/*!
	 *	分配session_id用
	 */
	CZUInt sessionIndex;

	/*!
	 *	记录session服务哪个基站
	 */
	std::unordered_map<CZUShort,CZUShort> sessionToSiteMap;
};

#endif //#ifndef CZ_SESSION_MANAGER_H
