/** - Start - Copyright (C), 2015-2025, Huacheng Tech. Co., Ltd. 2015 *****************************
 *
 *  File Name: czthriftmanager.h
 *
 *  Function: thriftπ‹¿Ì
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

#ifndef __CZ_THRIFT_MANAGER_H
#define __CZ_THRIFT_MANAGER_H

#ifndef _THRIFT_SERVER_TSERVER_H_
#include <thrift/server/TServer.h>
#endif

#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TServer.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#ifndef __LOGIC_FUNCTION_SERVICE_H
#include "logicfunctionserviceimpl.h"
#endif

using boost::shared_ptr;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

class CZThriftManager
{
public:
	CZThriftManager();
	~CZThriftManager();

public:
	CZInt init();
	CZInt startServer();
	CZInt stopServer();
   
private:
	TServer* pServer;
};

#endif
/** - End - Huacheng Tech. Co., Ltd. 2015 *********************************************************/ 

#endif //#ifdef THRIFT_ENABLED
