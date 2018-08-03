#include <czconfiguration.h>

#include <iostream>

#include <utils/czmiscs.h>
#include <czsingletonmanager.h>
#include <logic/czdatahelper.h>
#include <system/czlog.h>

using namespace std;
using namespace boost;
using namespace cz_types;

CZConfiguration::CZConfiguration()
{
	networkId = networkStatus = 0;
}

CZConfiguration::~CZConfiguration()
{
   
}

CZInt CZConfiguration::initialize()
{
	//检测网络ID是否存在
	networkId = getShortFromServerConfig("network_id");

	if(networkId == 0)
	{
		CZSingletonManagerInstance.getDataHelper().setServerConfig("network_id",CZMiscs::i2s(CZMiscs::randNumber()));

		networkId = getShortFromServerConfig("network_id");
	}

	networkStatus = getShortFromServerConfig("network_status");

	std::cout<<"网络号:(十进制:"<<networkId<<" 十六进制:"<<ShortToHexStr(networkId)<<") 状态:"<<(networkStatus!=0?"开启":"关闭")<<std::endl;

	if(networkId == 0)
	{
		return 1;
	}

	return 0;
}

CZInt CZConfiguration::changeNetworkStatus(bool bCloseIt)
{
	CZSingletonManagerInstance.getDataHelper().setServerConfig("network_status",bCloseIt?"0":"1");

	networkStatus = getShortFromServerConfig("network_status");

	return 0;
}

CZUShort CZConfiguration::getShortFromServerConfig(const std::string &key)
{
	if(key.length() == 0)
	{
		return 0;
	}

	return (CZUShort)CZMiscs::s2i(CZSingletonManagerInstance.getDataHelper().getServerConfig(key));
}

std::string CZConfiguration::getStringFromServerConfig(const std::string &key)
{
	if(key.length() == 0)
	{
		return 0;
	}

	return CZSingletonManagerInstance.getDataHelper().getServerConfig(key);
}