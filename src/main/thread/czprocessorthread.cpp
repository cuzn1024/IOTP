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
	//逻辑处理器注册对应函数
	logic.init();

	CZUShort uRet = 0;

	CZUShort uNode_network_addr = 0;

    while (CZSingletonManagerInstance.getThreadManager().stillRunning())
    {
		//从RequestNode队列中取出数据  若返回值不为零表示存在待处理的Node节点
		uNode_network_addr = CZSingletonManagerInstance.getMessagePool().getRequestNode();

		//并不存在待处理Node
		if(uNode_network_addr == 0 )
		{
			continue;
		}

		//从上述获取到的Node节点地址获取待处理数据
		if (! CZSingletonManagerInstance.getNodeManager().getMessageFromNode(uNode_network_addr,requestMessage))
		{
			logic.processMessage(requestMessage);
		}

		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
    }
}