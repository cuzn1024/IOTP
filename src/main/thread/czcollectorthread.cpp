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
	//��ʼ������session�ʹ�������session(�ػ�session �����ַΪ0xfffe)
    serialportConnectionHandler();

    while (CZSingletonManagerInstance.getThreadManager().stillRunning())
    {
		//����վ��ʱ
		checkTimeout();

		//����Ƿ���socket����
        establishRequestHandler();

		//1 ����frame���Ͷ������� 
		//2 �������ݷ���frame���ն�����
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
