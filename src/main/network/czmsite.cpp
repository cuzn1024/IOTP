#include <network/czmsite.h>

using namespace cz_types;
using namespace cz_defines;


CZMSite::CZMSite(CZUShort _network_addr, const CZPhysicalAddr &_phy_addr, cz_types::eNodeType node_type) : CZSite(_network_addr, _phy_addr, node_type)
{
}

CZMSite::~CZMSite()
{
	
}

CZInt CZMSite::addEdToManagerList(CZUShort ed_network_addr,bool inService)
{
	if(ed_network_addr <= 0)
	{
		return 1;
	}

	managerEdList[ed_network_addr] = inService;

	return 0;
}

CZInt CZMSite::delEdFromManagerList(CZUShort ed_network_addr)
{
	if(ed_network_addr <= 0)
	{
		return 1;
	}

	std::unordered_map<CZUShort,bool>::iterator itFind = managerEdList.find(ed_network_addr);

	if(itFind != managerEdList.end())
	{
		managerEdList.erase(itFind);
	}

	return 0;
}
CZInt CZMSite::getEdManagerList(std::vector<CZUShort>& edList,bool bInService)
{
	edList.clear();

	for(std::unordered_map<CZUShort,bool>::iterator it = managerEdList.begin();it != managerEdList.end();it++ )
	{
		if( ! it->second )
		{
			continue;	
		}

		edList.push_back(it->first);
	}

	return 0;
}