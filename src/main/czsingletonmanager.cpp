#include <czsingletonmanager.h>

#include <czsuitmanager.h>
#include <czconfiguration.h>
#include <thread/czmessagepool.h>
#include <thread/czthreadmanager.h>
#include <network/czconnectionsuite.h>
#include <network/cznodemanager.h>
#include <network/czsessionmanager.h>
#include <thrift/czthriftmanager.h>
#include <logic/czdatahelper.h>
#include <system/czlogger.h>

#define NEW_SINGLETON(name) instance##name = new CZ##name;
#define DELETE_SINGLETON(name) delete instance##name; instance##name = 0;

CZSingletonManager::CZSingletonManager()
{
	NEW_SINGLETON(DataHelper);
	NEW_SINGLETON(Configuration);

#ifdef THRIFT_ENABLED
	NEW_SINGLETON(ThriftManager);
#endif //#ifdef THRIFT_ENABLED

    NEW_SINGLETON(SuitManager);
    NEW_SINGLETON(ConnectionSuite);
    NEW_SINGLETON(ThreadManager);    
    NEW_SINGLETON(MessagePool);
	NEW_SINGLETON(FrameLogger);
	NEW_SINGLETON(NodeManager);
	NEW_SINGLETON(SessionManager);	
}

CZSingletonManager::~CZSingletonManager()
{
	DELETE_SINGLETON(Configuration);
	DELETE_SINGLETON(DataHelper);

#ifdef THRIFT_ENABLED
	DELETE_SINGLETON(ThriftManager);
#endif //#ifdef THRIFT_ENABLED

    DELETE_SINGLETON(SuitManager);
    DELETE_SINGLETON(ConnectionSuite);
    DELETE_SINGLETON(ThreadManager);    
    DELETE_SINGLETON(MessagePool);
	DELETE_SINGLETON(FrameLogger);
	DELETE_SINGLETON(NodeManager);
	DELETE_SINGLETON(SessionManager);	
}

CZInt CZSingletonManager::initialize()
{
	CZInt iRet = instanceDataHelper->init();
	if(iRet) return iRet;

	iRet = instanceConfiguration->initialize();

	if(iRet) return iRet;	

	iRet = instanceFrameLogger->init() ? 0 : 1;

	if(iRet) return iRet;

#ifdef THRIFT_ENABLED
	iRet = instanceThriftManager->init();
	
	if(iRet) return iRet;
#endif //#ifdef THRIFT_ENABLED

	iRet = instanceConnectionSuite->initialize();

	if (iRet) return iRet;

    instanceSuitManager->lazyLoadSuit();

	if (iRet) return iRet;

	iRet = instanceConnectionSuite->establishNetworkListener();

	if (iRet) return iRet;

	instanceNodeManager->init();

	instanceSessionManager->init();

	return 0;
}
