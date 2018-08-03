/** - Start - Copyright (C), 2015-2025, Huacheng Tech. Co., Ltd. 2015 *****************************
*
*  File Name: czthriftmanager.cpp
*
*  Function: thrift管理
*
*  Created: 2017/5/17
*
*  Author: guxingxin
*
*  Modify: 
*
*  <Date>          <Author>       <version >        <Desc>
* 2017-5-17        guxingxin         V01.00            
*
**************************************************************************************************/ 
#ifdef THRIFT_ENABLED

#include "czthriftmanager.h"

#include <memory>
#include <iostream>

#include <thrift/processor/TMultiplexedProcessor.h>

#include <czsingletonmanager.h>
#include <czconfiguration.h>

CZThriftManager::CZThriftManager():pServer(NULL)
{

}

CZThriftManager::~CZThriftManager()
{
	delete pServer;
	pServer = NULL;
}

CZInt CZThriftManager::init()
{
	int port = CZSingletonManagerInstance.getConfiguration().getShortFromServerConfig("thrift_port");

	if(port <= 0)
	{
		return 1;
	}

	shared_ptr<LogicFunctionServiceImpl> handler(new LogicFunctionServiceImpl());
	
	//单个service
	//shared_ptr<TProcessor> processor(new CliDataServiceProcessor(handler));

	//多个service
	shared_ptr<TMultiplexedProcessor> mulProcessor(new TMultiplexedProcessor());
	
	//对应不同的service handler
	shared_ptr<TProcessor> processor1(new LogicFunctionServiceProcessor(handler));

	//注意客户端调用也和单个service不一样
	mulProcessor->registerProcessor("LogicFunctionService", processor1);

	shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	shared_ptr<TTransportFactory> transportFactory(new TFramedTransportFactory());
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	const int workerCount = CZSingletonManagerInstance.getConfiguration().getShortFromServerConfig("thrift_thread_count");

	if(workerCount<= 0)
	{
		return 1;
	}

	shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(workerCount);
	threadManager->threadFactory(boost::make_shared<PlatformThreadFactory>());
	threadManager->start();

	pServer = new TThreadPoolServer(mulProcessor, serverTransport, transportFactory, protocolFactory,threadManager);

	std::cout<<"服务器接口端口:"<<port<<std::endl;

	return 0;
}

CZInt CZThriftManager::startServer()
{
	if(NULL == pServer) return 1;
	std::cout<<"服务器程序启动完毕!!!"<<std::endl;
	
	try
	{
		pServer->serve();
	}
	catch (...)
	{
		return 1;
	}

	return 0;
}

CZInt CZThriftManager::stopServer()
{
	if(NULL == pServer) return 1;
	pServer->stop();
	return 0;
}

#endif //#ifdef THRIFT_ENABLED