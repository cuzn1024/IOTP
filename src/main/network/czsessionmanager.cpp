#include <network/czsessionmanager.h>

#include <network/czsession.h>
#include <system/czlog.h>
#include <suite/transceiver/czitransceiverinterface.h>

CZSessionManager::CZSessionManager()
{
	sessionIndex = 1;
}

CZSessionManager::~CZSessionManager()
{

}
CZInt CZSessionManager::init()
{
	return 0;
}

CZUInt CZSessionManager::createSession(CZITransceiverInterface *transceiverInterface)
{    
	CZSession *session = new CZSession(transceiverInterface,getSessionId());

	addSession(session);

	return session->getId();
}

CZInt CZSessionManager::addSession(CZSession* session)
{
	if(NULL == session)
	{

		return 1;
	}

	if(session->getId() == 0)
	{
		return 2;
	}
	sessionPool.insert(SessionPool::value_type(session->getId(), std::shared_ptr<CZSession>(session) ));

	return 0;
}

CZUInt CZSessionManager::getSessionId()
{
	CZUInt id = 0;

	do 
	{
		id = sessionIndex++;

		if( id == 0 )
		{
			continue;
		}

	} while ( ( ! sessionPool.empty()) && (sessionPool.find(id) != sessionPool.end()));

	return id;
}

CZSessionManager::SessionPool &CZSessionManager::getSessionPool()
{
	return sessionPool;
}

CZInt CZSessionManager::updateSiteInfo(CZUShort sessionId,CZUShort siteNetworkAddr)
{
	if (siteNetworkAddr == 0)
	{
		return 1;
	}

	if (sessionPool.find(sessionId) != sessionPool.end())
	{
	    sessionToSiteMap[sessionId] = siteNetworkAddr;
		return 0;
	}


	return 1;
}

CZUShort CZSessionManager::getSiteAddr(CZUShort sessionId)
{
	if (sessionId == 0)
	{
		return -1;
	}

	std::unordered_map<CZUShort, CZUShort>::iterator it = sessionToSiteMap.find(sessionId);

	if (it == sessionToSiteMap.end())
	{
		return 0;
	}

	return it->second;
}