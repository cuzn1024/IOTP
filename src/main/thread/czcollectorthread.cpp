#include <thread/czcollectorthread.h>

#include <system/czlog.h>
#include <czsingletonmanager.h>
#include <thread/czthreadmanager.h>
#include <network/czconnectionsuite.h>

using namespace cz_types;

CZCollectorThread::CZCollectorThread(void)
{
}

CZCollectorThread::~CZCollectorThread(void)
{
}

void CZCollectorThread::run()
{
	//初始化串口session和创建申请session(特化session 网络地址为0xfffe)
    serialportConnectionHandler();

    while (CZSingletonManagerInstance.getThreadManager().stillRunning())
    {
		//检查基站超时
		checkTimeout();

		//检测是否有socket接入
        establishRequestHandler();

		//1 发送frame发送队列数据 
		//2 接收数据放入frame接收队列中
        processDataHandler();

		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
    }
}

void CZCollectorThread::serialportConnectionHandler()
{
    CZSingletonManagerInstance.getConnectionSuite().initializeSessionPool();
}

void CZCollectorThread::checkTimeout()
{
	CZSingletonManagerInstance.getConnectionSuite().checkTimeout();
}

void CZCollectorThread::establishRequestHandler()
{
    CZSingletonManagerInstance.getConnectionSuite().acceptConnection();
}

void CZCollectorThread::processDataHandler()
{
	CZSingletonManagerInstance.getConnectionSuite().sendData();

    CZSingletonManagerInstance.getConnectionSuite().collectData();
}
