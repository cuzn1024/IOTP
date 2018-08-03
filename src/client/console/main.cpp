#include<iostream>

#include <thrift/transport/TSocket.h>  
#include <thrift/transport/TBufferTransports.h>  
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/processor/TMultiplexedProcessor.h>
#include <thrift/protocol/TMultiplexedProtocol.h>

#include "logicfunctionservice.h"
#include <boost/xpressive/xpressive_dynamic.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <string>
#include <stdlib.h>
using namespace apache::thrift;  
using namespace apache::thrift::protocol;  
using namespace apache::thrift::transport;  

using boost::shared_ptr;
using namespace boost::xpressive;

bool isIPValid(const std::string &ip="localhost")
{
	if(boost::to_lower_copy(ip) == "localhost")
	{
		return true;
	}

	cregex reg_ip = cregex::compile("(25[0-4]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[1-9])[.](25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[.](25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[.](25[0-4]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[1-9])");
	return regex_match(ip.c_str(), reg_ip);
}

void startCliClient(const std::string &ip,int port, const std::string &key,std::vector<std::string>&vec )
{
	if(key.length() == 0 )
	{
		return ;
	}

	boost::shared_ptr<TSocket> socket(new TSocket(ip, port));  
	boost::shared_ptr<TTransport> transport(new TFramedTransport(socket)); 
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

	try
	{
		transport->open();
	}
	catch(TTransportException e)
	{
		std::cout<<"connect server failed,error:"<<e.what()<<std::endl;
		return;
	}

	//����service���÷�ʽ
	//CliDataServiceClient client(protocol);

	//���service���÷�ʽ
	shared_ptr<TMultiplexedProtocol> mulProcessor(new TMultiplexedProtocol(protocol,"LogicFunctionService"));
	LogicFunctionServiceClient client(mulProcessor);
	
	if(key == "seeSite")
	{
		std::map<std::string, eIOTNodeType::type> allowSiteMap;
		client.getAllowSites(allowSiteMap);

		for(std::map<std::string, eIOTNodeType::type>::iterator it = allowSiteMap.begin();it != allowSiteMap.end();it++)
		{
			std::cout<<"�����ַ:"<<it->first<<" ����:"<<(int)(it->second)<<std::endl;
		}
	}
	else if(key == "addSite")
	{
		if(vec.size() != 2)
		{
			return;
		}

		if(vec[0].length() != 16)
		{
			return;
		}

		int type = atoi(vec[1].c_str());

		if(type < 10 || type > 13)
		{
			return;
		}

		client.addSite(vec[0],(eIOTNodeType::type)type);
	}
	else if(key == "switch")
	{
		if(vec.size() == 0)
		{
			return ;
		}

		if(vec[0] == "on")
		{
			std::cout<<"׼��������"<<std::endl;
			client.operateNetwork(true);
			std::cout<<(client.getNetworkStatus()>0?"���紦�ڴ�":"���紦�ڹر�")<<std::endl;
		}
		else if(vec[0] == "off")
		{
			std::cout<<"׼���ر�����"<<std::endl;
			client.operateNetwork(false);
			std::cout<<(client.getNetworkStatus()>0?"���紦�ڴ�":"���紦�ڹر�")<<std::endl;
		}
	}

	transport->close();
}

std::map<std::string ,std::string> keyMap;

void showHelp()
{
	std::cout<<"������"<<std::endl;
	for(std::map<std::string ,std::string>::iterator it = keyMap.begin();it != keyMap.end();it++ )
	{
		std::cout<<it->first<<"\t\t"<<it->second<<std::endl;
	}
}

void init()
{
	keyMap["seeSite"] = "�鿴��վ,��:seeSite ,�·��Ѿ���ӵ�����������վ";
	keyMap["addSite"] = "��ӻ�վ,��:addSite �����ַ ��վ����,�� addSite 1122334455667788 10 .(10=R��վ 11=M��վ 12=C��վ )";
	keyMap["switch"]  = "��������,��:switch on/off";
}

int main(int argc, char *argv[])
{
	init();

	std::string ip="localhost";

	short port = 9090;

	std::string key = "";

	if(argc == 1 )
	{
		std::cout<<"�޷�ʶ��Ĳ���,�뿴������Ϣ������"<<std::endl;
		showHelp();
		system("pause");
		return 0;
	}

	key = argv[1];

	if(keyMap.find(key) == keyMap.end())
	{
		std::cout<<"�޷�ʶ��Ĳ���,�뿴������Ϣ������"<<std::endl;
		showHelp();
		system("pause");
		return 0;
	}

	std::vector<std::string> vec;

	if(key == "addSite")
	{
		if(argc < 4 )
		{
			return 0;
		}

		vec.push_back(argv[2]);
		vec.push_back(argv[3]);
	}
	else if(key == "switch")
	{
		vec.push_back(argv[2]);
	}

	startCliClient(ip,port,key,vec);

	return 0;
}