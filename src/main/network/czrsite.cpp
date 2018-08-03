#include <network/czrsite.h>
#include <system/czlog.h>

using namespace cz_types;
using namespace cz_defines;


CZRSite::CZRSite(CZUShort _network_addr, const CZPhysicalAddr &_phy_addr, cz_types::eNodeType node_type) : CZSite(_network_addr, _phy_addr, node_type)
{
	currentGroupSize = 0;
}

CZRSite::~CZRSite()
{
	
}

CZInt CZRSite::addEdToGroup(CZUShort ed_network_addr, CZUShort &groupId, CZUChar &pos, bool &bGroupChange, bool isUseDesignGroup)
{
	bGroupChange = false;

	if (ed_network_addr < 0)
	{
		return 1;
	}

	if (!isUseDesignGroup)
	{
		if(groupInfoMap.size() >= (R_AP_MAX_GROUP_MEMBER_SIZE * R_AP_MAX_GROUP))
		{
			DebugLog<<"该基站分组已满";
			return 1;
		}

		std::unordered_map<CZInt, CZUShort>::iterator it ;

		//清除以前的记录信息

		CZUShort oldGroupId = 0;
		CZUChar oldPos = 0;

		if (!findEdGroupInfo(ed_network_addr, oldGroupId, oldPos))
		{
			it = groupInfoMap.find(decreaseDimension(oldGroupId, oldPos));

			if (it != groupInfoMap.end())
			{
				groupInfoMap.erase(it);
			}
		}		

		bool bFind = false;

		//前面为空 挨个排放 暂时采用该方法处理
		for (CZUShort group = 0; group < R_AP_MAX_GROUP; group++)
		{
			for(CZUChar position = 0 ;position < R_AP_MAX_GROUP_MEMBER_SIZE;position++)
			{
				//检测是否被占用
				it = groupInfoMap.find(decreaseDimension(group, position));

				if(it == groupInfoMap.end())
				{
					bFind = true;
				}
				else
				{
					if(it->second == 0)
					{
						bFind = true;
					}
				}

				if(bFind)
				{
					groupId = group;
					pos = position;
					break;
				}
			}

			if(bFind)
			{
				break;
			}
		}

		if(!bFind)
		{
			return 1;
		}
	}

	if (groupId >= currentGroupSize)
	{
		currentGroupSize = groupId + 1;
		bGroupChange = true;
	}

	groupInfoMap[decreaseDimension(groupId, pos)] = ed_network_addr;

	edInfoMap[ed_network_addr] = decreaseDimension(groupId, pos);

	return 0;
}

CZUShort CZRSite::findEdByPos(CZUShort groupId,CZUChar pos)
{
	std::unordered_map<CZInt,CZUShort>::iterator it = groupInfoMap.find(decreaseDimension(groupId,pos));

	if(it == groupInfoMap.end())
	{
		return 0;
	}

	return it->second;
}

CZInt CZRSite::findEdGroupInfo(CZUShort networkAddr, CZUShort& groupId, CZUChar& pos)
{
	CZInt key = 0;

	std::unordered_map<CZUShort,CZInt>::iterator it = edInfoMap.find(networkAddr);

	if (it == edInfoMap.end())
	{
		return 1;
	}

	key = it->second;

	increaseDimension(key, groupId, pos);

	return 0;
}

void CZRSite::increaseDimension(CZInt key, CZUShort& groupId, CZUChar& pos)
{
	groupId = (key / 1000);

	pos = (CZUChar)(key % 1000);
}

CZInt CZRSite::decreaseDimension(CZUShort groupId, CZUChar pos)
{
	return (groupId * 1000 + pos);
}
