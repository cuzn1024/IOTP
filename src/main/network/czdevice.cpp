#include <network/czdevice.h>

using namespace cz_types;
using namespace cz_defines;


CZDevice::CZDevice(CZUShort _network_addr, const CZPhysicalAddr &_phy_addr, cz_types::eNodeType node_type) : CZNode(_network_addr, _phy_addr, node_type)
{
}

CZDevice::~CZDevice()
{
	
}

CZInt CZDevice::setCanReachSites(cz_types::eNodeType nodeType, std::vector<CZUShort>& vec)
{
	reachSitesMap[nodeType] = vec;

	return 0;
}

CZInt CZDevice::getCanReachSites(cz_types::eNodeType nodeType, std::vector<CZUShort> *&vec)
{
    vec = 0;

	NodeMapIter it = reachSitesMap.find(nodeType);

	if (it == reachSitesMap.end())
    {
        reachSitesMap.insert(std::make_pair(nodeType, std::vector<CZUShort>()));
        it = reachSitesMap.find(nodeType);
    }

	vec = &(it->second);

	return 0;
}

CZInt CZDevice::delCanReachSites(cz_types::eNodeType nodeType)
{
	NodeMapIter it = reachSitesMap.find(nodeType);

	if (it == reachSitesMap.end())
		return 1;

	reachSitesMap.erase(it);

	return 0;
}

