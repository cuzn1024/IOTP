#ifdef THRIFT_ENABLED

#include <logic/czapicenter.h>

#include <system/czlog.h>
#include <czsingletonmanager.h>
#include <czconfiguration.h>
#include <thread/czmessagepool.h>
#include <logic/czdatahelper.h>
#include <network/cznodemanager.h>
#include <chrono>
#include <utils/czmiscs.h>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace cz_defines;
using namespace cz_types;

CZApiCenter::CZApiCenter()
{
	responseBuffer = new CZUChar[receive_buffer_size];
	bufferLen = 0;
}

CZApiCenter::~CZApiCenter()
{
	delete[] responseBuffer;
	responseBuffer = 0;
}

void CZApiCenter::init()
{
	
}

LogicFunctionServiceImpl::LogicFunctionServiceImpl()
{

}
LogicFunctionServiceImpl::~LogicFunctionServiceImpl()
{

}
int32_t LogicFunctionServiceImpl::raiseSignal(const eIOTSignalType::type type)
{
	return 0;
}
void LogicFunctionServiceImpl::getAllowSites(std::map<std::string, eIOTNodeType::type> & _return)
{
	_return.clear();

	boost::unordered_map<std::string,eNodeType> siteMap = CZSingletonManagerInstance.getNodeManager().getAllowSites();

	if(siteMap.size() == 0)
	{
		return;
	}

	for(boost::unordered_map<std::string,eNodeType>::iterator it = siteMap.begin();it != siteMap.end();it++ )
	{
		_return[it->first] = (eIOTNodeType::type)(it->second);
	}
}
int32_t LogicFunctionServiceImpl::addSite(const std::string& phyAddr, const eIOTNodeType::type type)
{
	if(phyAddr.length()==0)
	{
		return 1;
	}

	eNodeType nodeType = (eNodeType)(type);

	if(!CZMiscs::isSite((CZUChar)type))
	{
		return 1;
	}

	int32_t iRet = CZSingletonManagerInstance.getNodeManager().addSiteAllow(phyAddr,nodeType);

	return iRet;
}
int32_t LogicFunctionServiceImpl::delSite(const std::string& phyAddr)
{
	return 0;
}
int32_t LogicFunctionServiceImpl::delDevice(const std::string& phyAddr)
{
	return 0;
}

int32_t LogicFunctionServiceImpl::getNetworkStatus()
{
	return CZSingletonManagerInstance.getConfiguration().isNetworkOpen()?1:0;
}

int32_t LogicFunctionServiceImpl::operateNetwork(const bool isOpen)
{
	return CZSingletonManagerInstance.getConfiguration().changeNetworkStatus(isOpen?false:true);
}
int32_t LogicFunctionServiceImpl::rebuildNetwork()
{
	return 0;
}

#endif //#ifdef THRIFT_ENABLED