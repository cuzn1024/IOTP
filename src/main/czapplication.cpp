#include <czapplication.h>

#include <czsingletonmanager.h>
#include <thread/czthreadmanager.h>

#ifdef THRIFT_ENABLED
#include <thrift/czthriftmanager.h>
#endif //#ifdef THRIFT_ENABLED

CZApplication::CZApplication()
{
}


CZApplication::~CZApplication()
{
}

CZInt CZApplication::initializeSingleton()
{
    return CZSingletonManagerInstance.initialize();
}

CZInt CZApplication::initialize()
{
    return initializeSingleton();
}

CZInt CZApplication::start()
{
    return run();
}

CZInt CZApplication::run()
{
    CZInt ret = initialize();

    if (ret != 0) 
	{
		return ret;
	}

	startThreads();

#ifdef THRIFT_ENABLED
	CZSingletonManagerInstance.getThriftManager().startServer();
#else
    CZSingletonManagerInstance.getThreadManager().joinAllThread();
#endif //#ifdef THRIFT_ENABLED

    return 0;
}

CZInt CZApplication::startThreads()
{
    CZSingletonManagerInstance.getThreadManager().startThreads();

    return 0;
}