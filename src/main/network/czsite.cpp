#include <network/czsite.h>

using namespace cz_types;
using namespace cz_defines;


CZSite::CZSite(CZUShort _network_addr, const CZPhysicalAddr &_phy_addr, cz_types::eNodeType node_type) : CZNode(_network_addr, _phy_addr, node_type), freId(0)
{
}

CZSite::~CZSite()
{
	
}