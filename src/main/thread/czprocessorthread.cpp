#include <thread/czprocessorthread.h>

#include <system/czlog.h>
#include <czsingletonmanager.h>
#include <thread/czmessagepool.h>
#include <thread/czthreadmanager.h>
#include <network/cznodemanager.h>

using namespace cz_types;
using namespace cz_defines;

CZProcessorThread::CZProcessorThread()
{
	requestMessageBuffer = new CZUChar[receive_buffer_size];
	requestMessage = (RequestMessage*)requestMessageBuffer;
}

CZProcessorThread::~CZProcessorThread(void)
{
	delete[] requestMessageBuffer;
	requestMessageBuffer = 0;
}

void CZProcessorThread::run()
{
	//�߼�������ע���Ӧ����
	logic.init();

	CZUShort uRet = 0;

	CZUShort uNode_network_addr = 0;

    while (CZSingletonManagerInstance.getThreadManager().stillRunning())
    {
		//��RequestNode������ȡ������  ������ֵ��Ϊ���ʾ���ڴ������Node�ڵ�
		uNode_network_addr = CZSingletonManagerInstance.getMessagePool().getRequestNode();

		//�������ڴ�����Node
		if(uNode_network_addr == 0 )
		{
			continue;
		}

		//��������ȡ����Node�ڵ��ַ��ȡ����������
		if (! CZSingletonManagerInstance.getNodeManager().getMessageFromNode(uNode_network_addr,requestMessage))
		{
			logic.processMessage(requestMessage);
		}

		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
    }
}