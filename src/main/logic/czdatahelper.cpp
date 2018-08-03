#include <logic/czdatahelper.h>

#include <iostream>
#include <algorithm>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/foreach.hpp>

#include <utils/czmiscs.h>
#include <czsingletonmanager.h>
#include <network/cznode.h>
#include <system/czlog.h>
#include <czconfiguration.h>


CZDataHelper::CZDataHelper()
{
	db = NULL;
}
CZDataHelper::~CZDataHelper()
{
	if(db != NULL)
	{
		sqlite3_close(db);
	}
}
CZUInt CZDataHelper::init()
{
	bool bCreate = false;

	dbPath = CZMiscs::getApplicationPath() + "server.db";

	if (!boost::filesystem::exists(dbPath))
	{
		bCreate = true;
	}

	if( sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK )
	{
		std::cout<<dbPath<<"数据库打开失败,"<<sqlite3_errmsg(db)<<std::endl;
		return 1;
	}

	std::string index ="";

	//创建表t_server_config
	sql = "CREATE TABLE IF NOT EXISTS t_server_config("  \
		"Id INTEGER PRIMARY KEY     NOT NULL," \
		"Key  VARCHAR(50) UNIQUE   NOT NULL," \
		"Value         VARCHAR(100)   NOT NULL);";
	if(exeSql(sql))
	{
		return 1;
	}

	//创建表t_node_info
	sql = "CREATE TABLE IF NOT EXISTS t_node_info("  \
		"Id INTEGER PRIMARY KEY     NOT NULL," \
		"NetworkAddr       INTEGER  DEFAULT 0," \
		"PhyAddr           VARCHAR(16)   NOT NULL, " \
		"Type              INTEGER       DEFAULT 0," \
		"Status            INTEGER       DEFAULT 0," \
		"Time              INTEGER       DEFAULT 0);";
	if(exeSql(sql))
	{
		return 1;
	}

	//创建表t_freid_info
	sql = "CREATE TABLE IF NOT EXISTS t_freid_info("  \
		"Id INTEGER PRIMARY KEY     NOT NULL," \
		"FreId			   INTEGER   UNIQUE NOT NULL," \
		"SiteId			   INTEGER," \
		"Time              INTEGER       DEFAULT 0,"   \
		"FOREIGN KEY(SiteId) REFERENCES t_node_info(Id));";
	if (exeSql(sql))
	{
		return 1;
	}

	//创建表t_msite_work
	sql = "CREATE TABLE IF NOT EXISTS t_msite_work("  \
		"Id INTEGER PRIMARY KEY     NOT NULL," \
		"SiteId			   INTEGER," \
		"EdId			   INTEGER," \
		"InService         INTEGER   DEFAULT 0," \
		"FOREIGN KEY(EdId) REFERENCES t_node_info(Id)," \
		"FOREIGN KEY(SiteId) REFERENCES t_node_info(Id));" ;

	if(exeSql(sql))
	{
		return 1;
	}

	//sql = "CREATE INDEX IF NOT EXISTS uni_t_msite_work on t_msite_work (NetworkAddr, EdNetworkAddr)";

	//if(exeSql(sql))
	//{
	//	return 1;
	//}

	//创建表t_rsite_work
	sql = "CREATE TABLE IF NOT EXISTS t_rsite_work("  \
		"Id INTEGER PRIMARY KEY     NOT NULL," \
		"SiteId			   INTEGER," \
		"EdId			   INTEGER," \
		"GroupId           INTEGER   DEFAULT 0," \
		"Postion           INTEGER   DEFAULT 0," \
		"InService         INTEGER   DEFAULT 0,"  \
		"FOREIGN KEY(SiteId) REFERENCES t_node_info(Id)," \
		"FOREIGN KEY(EdId) REFERENCES t_node_info(Id))";

	if(exeSql(sql))
	{
		return 1;
	}

	//sql = "CREATE INDEX IF NOT EXISTS uni_t_rsite_work on t_rsite_work (NetworkAddr, EdNetworkAddr)";

	//if(exeSql(sql))
	//{
	//	return 1;
	//}

	if(bCreate)
	{
		initData();
	}

	return 0;
}

CZInt CZDataHelper::initData()
{
	//直接语句速度有点慢 首次安装初始化无妨
	std::cout<<"清空原有数据"<<std::endl;

	sql = "DELETE from t_freid_info";
	exeSql(sql);

	sql = "DELETE from t_msite_work";
	exeSql(sql);

	sql = "DELETE from t_node_info";
	exeSql(sql);

	sql = "DELETE from t_rsite_work";
	exeSql(sql);

	sql = "DELETE from t_server_config";
	exeSql(sql);

	std::cout<<"开始导入配置"<<std::endl;

	sql = "insert into t_server_config(Key,Value) values('network_id',0)";
	exeSql(sql);

	sql = "insert into t_server_config(Key,Value) values('network_status',1)";
	exeSql(sql);

	sql = "insert into t_server_config(Key,Value) values('thrift_port',9090)";
	exeSql(sql);

	sql = "insert into t_server_config(Key,Value) values('thrift_thread_count',1)";
	exeSql(sql);

	sql = "insert into t_server_config(Key,Value) values('server_port',1024)";
	exeSql(sql);

	sql = "insert into t_server_config(Key,Value) values('site_tx_power',20)";
	exeSql(sql);

	sql = "insert into t_server_config(Key,Value) values('log_file_size',10)";
	exeSql(sql);

	sql = "insert into t_server_config(Key,Value) values('log_show_type',1)";
	exeSql(sql);

	sql = "insert into t_server_config(Key,Value) values('log_show_debug',1)";
	exeSql(sql);

	sql = "insert into t_server_config(Key,Value) values('msite_range',20)";
	exeSql(sql);

	sql = "insert into t_server_config(Key,Value) values('socket_timeout',60)";
	exeSql(sql);

	std::cout<<"导入配置完成"<<std::endl;

	sqlite3_exec(db,"begin;",0,0,0);  
	sqlite3_stmt *stmt;  
	sql = "insert into t_freid_info(FreId) values(?)";
	sqlite3_prepare_v2(db,sql.c_str(),sql.length(),&stmt,0);  

	for(int i=1;i<=200;++i)  
	{         
		sqlite3_reset(stmt);  
		sqlite3_bind_int(stmt,1,i);
		sqlite3_step(stmt);

		if(i%20 == 0)
		{
			std::cout<<"初始化频点进度:"<<(i / 2)<<"%"<<std::endl;
		}
	}

	sqlite3_finalize(stmt);  
	sqlite3_exec(db,"commit;",0,0,0); 

	return 0;
}

CZInt CZDataHelper::setServerConfig(const std::string &key, const std::string &value)
{
	if (key.length() == 0)
	{
		return 0;
	}

	std::string sql = "update t_server_config set Value='" + value + "' where Key = '" + key + "';";

	int rc = sqlite3_exec(db, sql.c_str(), NULL, 0, NULL);

	if( rc != SQLITE_OK )
	{
		std::cout << sql << " setServerConfig失败," << sqlite3_errmsg(db) << std::endl;
		return 1;
	}

	return 0;
}

int CZDataHelper::exeSql(const std::string &sql)
{
	if(sql.length()==0)
	{
		return 1;
	}

	if(NULL == db)
	{
		return 1;
	}

	int rc = sqlite3_exec(db, sql.c_str(), NULL, 0, NULL);

	if( rc != SQLITE_OK )
	{
		DebugLog<<"sql:"<<sql<<" 执行失败,"<<sqlite3_errmsg(db);
		return 1;
	}

	return 0;
}

std::string CZDataHelper::getServerConfig(const std::string &key)
{
	std::string retStr = "";

	if(key.length() == 0)
	{
		return retStr;
	}

	sqlite3_stmt  *stmt = NULL;

	std::string sql = "select Value from t_server_config where Key = '"+key+"' limit 1;";

	int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

	if(rc == SQLITE_OK)
	{
		if( sqlite3_step(stmt) ==SQLITE_ROW ) 
		{ 
			const unsigned char* pValue = sqlite3_column_text( stmt, 0);

			retStr = (const char*)pValue;
		} 
	}

	sqlite3_finalize(stmt);

	return retStr;
}

CZInt CZDataHelper::addSiteAllow(const std::string &phyAddr,eNodeType type)
{
	if (phyAddr.length() != 16)
	{
		return 1;
	}

	//sql = "insert into t_site_allow(PhyAddr,Type) values('"+phyAddr+"',"+CZMiscs::i2s((int)type)+")";

	return exeSql(sql);
}

CZInt CZDataHelper::loadAllSiteAllow()
{
	sqlite3_stmt  *stmt = NULL;

	std::string sql = "select PhyAddr, Type from t_node_info";

	std::cout<<std::endl<<"加载允许入网信息"<<std::endl;

	int rc = sqlite3_prepare_v2(db,sql.c_str(),sql.length(),&stmt,NULL);

	if(rc == SQLITE_OK)
	{
		while( sqlite3_step(stmt) ==SQLITE_ROW ) 
		{ 
			const unsigned char* pAddr = sqlite3_column_text( stmt, 0);
            
            std::stringstream ss;
            ss.setf(std::ios::hex, std::ios::basefield);
            ss.str((const char*)pAddr);

            CZULongLong addr;
            ss>>addr;

            for (unsigned i = 0; i < 4; i++)
            {
                std::swap(*((CZUChar*)&addr + i), *((CZUChar*)&addr + 7 - i));
            }

			CZPhysicalAddr phy_addr((CZUChar*)&addr);

			int type = sqlite3_column_int( stmt, 1);

			if(!CZMiscs::isNodeTypeRight((CZUChar)type) || CZMiscs::isDevice(type))
			{
				continue;
			}

			std::cout<<"物理地址:"<<pAddr<<" 类型:"<<ByteToHexStr((CZUChar)type)<<std::endl;

			CZSingletonManagerInstance.getNodeManager().addSiteAllow(phy_addr,(eNodeType)type);
		} 
	}
	std::cout<<"加载允许入网信息结束"<<std::endl;
	sqlite3_finalize(stmt);

	return 0;
}

CZInt CZDataHelper::loadAllNodeInfo()
{
	sqlite3_stmt  *stmt = NULL;

	std::string sql = "select NetworkAddr,PhyAddr,Type from t_node_info";

	std::cout<<std::endl<<"加载节点信息"<<std::endl;

	int rc = sqlite3_prepare_v2(db,sql.c_str(),sql.length(),&stmt,NULL);

	if(rc == SQLITE_OK)
	{
		while( sqlite3_step(stmt) ==SQLITE_ROW ) 
		{ 
			int network_addr = sqlite3_column_int( stmt, 0);
            
			const unsigned char* pAddr = sqlite3_column_text( stmt, 1);
            
            std::stringstream ss;
            ss.setf(std::ios::hex, std::ios::basefield);
            ss.str((const char*)pAddr);

            CZULongLong addr;
            ss>>addr;

            for (unsigned i = 0; i < 4; i++)
            {
                std::swap(*((CZUChar*)&addr + i), *((CZUChar*)&addr + 7 - i));
            }

			CZPhysicalAddr phy_addr((CZUChar*)&addr);

			int type = sqlite3_column_int( stmt, 2);

			if (!CZMiscs::isNodeTypeRight((CZUChar)type) || network_addr == 0)
			{
				continue;
			}

			std::cout<<"物理地址:"<<(const char*)pAddr<<" 类型:"<<ByteToHexStr((CZUChar)type)<<" 网络地址:"<<ShortToHexStr((CZUShort)network_addr)<<std::endl;

			CZSingletonManagerInstance.getNodeManager().createNode(phy_addr,(eNodeType)type,(CZUShort)network_addr);
		} 
	}

	std::cout<<"加载节点信息结束"<<std::endl;

	sqlite3_finalize(stmt);

	return 0;
}

CZInt CZDataHelper::loadAllFreidInfo()
{
	sqlite3_stmt  *stmt = NULL;

	std::map<CZUShort,CZUShort> tmpMap;

	std::string sql = "select FreId,Type,NetworkAddr from t_freid_info left join t_node_info on  t_freid_info.SiteId=t_node_info.Id";

	std::cout<<std::endl<<"加载频点信息"<<std::endl;

	int rc = sqlite3_prepare_v2(db,sql.c_str(),sql.length(),&stmt,NULL);

	if(rc == SQLITE_OK)
	{
		while( sqlite3_step(stmt) ==SQLITE_ROW ) 
		{ 
			tmpMap.clear();

			CZUShort freId = (CZUShort)sqlite3_column_int( stmt, 0);

			CZUChar type = (CZUChar)sqlite3_column_int( stmt, 1);

			CZUShort networkAddr = (CZUShort)sqlite3_column_int( stmt, 2);

			if(networkAddr > 0)
			{
				std::cout<<"基站:"<<ShortToHexStr(networkAddr)<<" 工作在频点:"<<freId<<std::endl;
			}

			CZSingletonManagerInstance.getNodeManager().setFreidInfo(freId,networkAddr);
		} 
	}

	std::cout<<"加载频点信息结束"<<std::endl;

	sqlite3_finalize(stmt);

	return 0;
}

CZInt CZDataHelper::loadAllMSiteWork()
{
	sqlite3_stmt  *stmt = NULL;

	std::string sql = "select a.NetworkAddr, b.NetworkAddr, c.InService from t_msite_work c"
		" join t_node_info b on b.Id = c.EdId join t_node_info a on a.Id = c.SiteId";

	std::vector<CZUShort> vec;

	std::map<CZUShort,std::vector<CZUShort>> canReachSiteMap;

	std::map<CZUShort,std::vector<CZUShort>>::iterator it;

	std::cout<<std::endl<<"加载M基站管理信息"<<std::endl;

	int rc = sqlite3_prepare_v2(db,sql.c_str(),sql.length(),&stmt,NULL);

	CZUShort siteNetworkAddr = 0;

	CZUShort edNetworkAddr = 0;

	CZUShort inService = 0;

	if(rc == SQLITE_OK)
	{
		while( sqlite3_step(stmt) ==SQLITE_ROW ) 
		{ 
			vec.clear();

			siteNetworkAddr = (CZUShort)sqlite3_column_int( stmt, 0);

			edNetworkAddr = (CZUShort)sqlite3_column_int( stmt, 1);

			inService = (CZUShort)sqlite3_column_int( stmt, 2);

			if(siteNetworkAddr == 0 || edNetworkAddr == 0 )
			{
				continue;
			}
			
			it = canReachSiteMap.find(edNetworkAddr);

			if(it != canReachSiteMap.end())
			{
				vec = it->second;
			}

			vec.push_back(siteNetworkAddr);

			canReachSiteMap[edNetworkAddr] = vec;

			if(inService)
			{
				std::cout<<"M基站:"<<ShortToHexStr(siteNetworkAddr)<<"轮询 终端:"<<ShortToHexStr(edNetworkAddr)<<std::endl;
			}
			else
			{
				std::cout<<"M基站:"<<ShortToHexStr(siteNetworkAddr)<<"热备 终端:"<<ShortToHexStr(edNetworkAddr)<<std::endl;
			}
		} 

		for(it = canReachSiteMap.begin(); it != canReachSiteMap.end(); it++ )
		{
			CZSingletonManagerInstance.getNodeManager().setCanReachSites(it->first,M_AP,it->second);
		}
	}

	std::cout<<"加载M基站管理信息结束"<<std::endl;

	sqlite3_finalize(stmt);

	return 0;
}

CZInt CZDataHelper::loadAllRSiteWork()
{
	sqlite3_stmt  *stmt = NULL;

	std::string sql = "select a.NetworkAddr, c.GroupId, c.Postion, b.NetworkAddr, c.InService from t_rsite_work c"
		" join t_node_info b on b.Id = c.EdId join t_node_info a on a.Id = c.SiteId";

	std::cout<<std::endl<<"加载R基站管理信息"<<std::endl;

	std::vector<CZUShort> vec;

	std::map<CZUShort,std::vector<CZUShort>> canReachSiteMap;

	std::map<CZUShort,std::vector<CZUShort>>::iterator it;

	int rc = sqlite3_prepare_v2(db,sql.c_str(),sql.length(),&stmt,NULL);

	if(rc == SQLITE_OK)
	{
		while( sqlite3_step(stmt) ==SQLITE_ROW ) 
		{ 
			CZUShort siteNetworkAddr = (CZUShort)sqlite3_column_int( stmt, 0);

			CZUShort groupId = (CZUShort)sqlite3_column_int( stmt, 1);

			CZUChar pos = (CZUChar)sqlite3_column_int( stmt, 2);

			CZUShort edNetworkAddr = (CZUShort)sqlite3_column_int( stmt, 3);

			CZUShort inService = (CZUShort)sqlite3_column_int( stmt, 4);

			it = canReachSiteMap.find(edNetworkAddr);

			if(it != canReachSiteMap.end())
			{
				vec = it->second;
			}

			vec.push_back(siteNetworkAddr);

			canReachSiteMap[edNetworkAddr] = vec;

			bool bGroupChange = false;

			CZSingletonManagerInstance.getNodeManager().addEdToGroup(siteNetworkAddr,edNetworkAddr,groupId,pos,bGroupChange,true);

			if(inService)
			{
				std::cout<<"R基站:"<<ShortToHexStr(siteNetworkAddr)<<"服务 终端:"<<ShortToHexStr(edNetworkAddr)<<" 组:"<<groupId<<" 位置:"<<(int)pos<<std::endl;
			}
			else
			{
				std::cout<<"R基站:"<<ShortToHexStr(siteNetworkAddr)<<"热备 终端:"<<ShortToHexStr(edNetworkAddr)<<" 组:"<<groupId<<" 位置:"<<(int)pos<<std::endl;
			}
		} 

		for(it = canReachSiteMap.begin(); it != canReachSiteMap.end(); it++ )
		{
			CZSingletonManagerInstance.getNodeManager().setCanReachSites(it->first,R_AP,it->second);
		}
	}

	std::cout<<"加载R基站管理信息结束"<<std::endl<<std::endl;

	sqlite3_finalize(stmt);

	return 0;
}

CZInt CZDataHelper::setFreidInfo(CZUShort freId,CZUShort networkAddr,eNodeType nodeType)
{
	if(freId == 0)
	{
		return 1;
	}

	sql = "update t_freid_info set SiteId=(select id from t_node_info where NetworkAddr="+CZMiscs::i2s(networkAddr)+") where FreId = "+CZMiscs::i2s(freId);

	return exeSql(sql);
}

CZInt CZDataHelper::createNode(CZUShort networkAddr, const std::string &phyAddr,eNodeType type)
{
	if (networkAddr == 0)
	{
		return 1;
	}
	std::string addr(phyAddr);
	while (addr.length() < 16)
	{
		addr = "0" + addr;
	}

	if (CZMiscs::isDevice(type))
		sql = "insert into t_node_info(NetworkAddr,PhyAddr,Type,Time) values(" + CZMiscs::i2s(networkAddr) + ",'" + addr + "'," + CZMiscs::i2s(type) + "," + CZMiscs::i2s(CZMiscs::getTimestamp()) + ")";
	else
		sql = "update t_node_info set networkaddr=" + CZMiscs::i2s(networkAddr) + ", time=" + CZMiscs::i2s(CZMiscs::getTimestamp()) + " where phyaddr='" + addr + "' and type='" + CZMiscs::i2s(type) + "'";

	return exeSql(sql);
}

CZInt CZDataHelper::addToMSiteList(CZUShort site_networkAddr, CZUShort ed_networkAddr, bool add)
{
	if(site_networkAddr == 0 || ed_networkAddr == 0 )
	{
		return 1;
	}

    if (add)
	    sql = "insert into t_msite_work(SiteId, EdId) values((select id from t_node_info where networkaddr=" + CZMiscs::i2s(site_networkAddr) +
			"), (select id from t_node_info where networkaddr=" + CZMiscs::i2s(ed_networkAddr) + "))";
    else
        sql = "delete from t_msite_work where SiteId=(select id from t_node_info where networkaddr=" + CZMiscs::i2s(site_networkAddr) + 
			") and EdId=(select id from t_node_info where networkaddr=" + CZMiscs::i2s(ed_networkAddr) + ")";

	return exeSql(sql);
}

CZInt CZDataHelper::delFromMSiteList(CZUShort site_networkAddr,CZUShort ed_networkAddr)
{
	if(site_networkAddr == 0 || ed_networkAddr == 0 )
	{
		return 1;
	}

	sql = "DELETE from t_msite_work where SiteId=(select id from t_node_info where networkaddr="+CZMiscs::i2s(site_networkAddr)+
		") and EdId = (select id from t_node_info where networkaddr="+CZMiscs::i2s(ed_networkAddr) + ")";

	return exeSql(sql);
}

CZInt CZDataHelper::setMSiteInservice(CZUShort site_networkAddr,CZUShort ed_networkAddr,bool InService)
{
	if(site_networkAddr == 0 || ed_networkAddr == 0 )
	{
		return 1;
	}

	if (InService)
	{
		sql = "update t_msite_work set InService = 1 where SiteId = (select id from t_node_info where networkaddr="+CZMiscs::i2s(site_networkAddr)+
			") and EdId = (select id from t_node_info where networkaddr="+CZMiscs::i2s(ed_networkAddr) + ")";
	}
	else
	{
		sql = "update t_msite_work set InService = 0 where SiteId = (select id from t_node_info where networkaddr="+CZMiscs::i2s(site_networkAddr)+
			") and EdId = (select id from t_node_info where networkaddr="+CZMiscs::i2s(ed_networkAddr) + ")";
	}

	return exeSql(sql);
}

CZInt CZDataHelper::addToRSiteGroup(CZUShort site_networkAddr,CZUShort ed_networkAddr)
{
	if(site_networkAddr == 0 || ed_networkAddr == 0 )
	{
		return 1;
	}

	sql = "insert into t_rsite_work(SiteId, EdId) values((select id from t_node_info where networkaddr=" + CZMiscs::i2s(site_networkAddr) +
		"), (select id from t_node_info where networkaddr=" + CZMiscs::i2s(ed_networkAddr) + "))";

	return exeSql(sql);
}

CZInt CZDataHelper::delRSiteGroupNode(CZUShort site_networkAddr,CZUShort ed_networkAddr)
{
	if(site_networkAddr == 0 || ed_networkAddr == 0 )
	{
		return 1;
	}

	sql = "DELETE from t_rsite_work where SiteId=(select id from t_node_info where networkaddr=" + CZMiscs::i2s(site_networkAddr) +
		") and EdId = (select id from t_node_info where networkaddr=" + CZMiscs::i2s(ed_networkAddr) + ")";

	return exeSql(sql);
}
CZInt CZDataHelper::setRSiteInservice(CZUShort site_networkAddr,CZUShort ed_networkAddr,bool InService)
{
	if(site_networkAddr == 0 || ed_networkAddr == 0 )
	{
		return 1;
	}

	if(InService)
	{
		sql = "update t_rsite_work set InService = 1 where SiteId=(select id from t_node_info where networkaddr=" + CZMiscs::i2s(site_networkAddr) +
			") and EdId = (select id from t_node_info where networkaddr=" + CZMiscs::i2s(ed_networkAddr) + ")";
	}
	else
	{
		sql = "update t_rsite_work set InService = 0 where SiteId=(select id from t_node_info where networkaddr=" + CZMiscs::i2s(site_networkAddr) +
			") and EdId = (select id from t_node_info where networkaddr=" + CZMiscs::i2s(ed_networkAddr) + ")";
	}

	return exeSql(sql);
}

CZInt CZDataHelper::setRSiteGroupInfo(CZUShort site_networkAddr,CZUShort ed_networkAddr,CZUShort groupId,CZUChar pos)
{
	if(site_networkAddr == 0 || ed_networkAddr == 0 )
	{
		return 1;
	}

	sql = "update t_rsite_work set GroupId = "+CZMiscs::i2s(groupId)+", Postion = "+CZMiscs::i2s(pos)+
		" where SiteId=(select id from t_node_info where networkaddr=" + CZMiscs::i2s(site_networkAddr) +
		") and EdId = (select id from t_node_info where networkaddr=" + CZMiscs::i2s(ed_networkAddr) + ")";

	return exeSql(sql);
}