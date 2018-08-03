#include <thread/czmessagepool.h>

#include <defines.h>
#include <system/czlog.h>

#include <czsingletonmanager.h>

using namespace cz_types;
using namespace cz_defines;

CZMessagePool::CZMessagePool(void) :requestNodePoolQueue(rt_message_pool_max_size),responseMessagePool(receive_buffer_size), 
	responseMessagePoolQueue(rt_message_pool_max_size),timeoutMsgPool(receive_buffer_size),timeoutMsgPoolQueue(rt_message_pool_max_size)
{
}

CZInt CZMessagePool::addRequestNode(CZUShort node_network_addr)
{
	if(0 == node_network_addr)
	{
		return 1;
	}

	while (requestNodePoolQueue.full())
	{
		//队列已满 等待放入
		DebugLog<<"RequestMessage queue is full ,throw it...";
        return 0;
	}

	requestNodePoolQueue.push_back(node_network_addr);

	return 0;
}

CZUShort CZMessagePool::getRequestNode()
{
	if(requestNodePoolQueue.empty())
	{
		return 0;
	}

	CZUShort network_addr = requestNodePoolQueue.front();

	requestNodePoolQueue.pop_front();

	return network_addr;
}

CZInt CZMessagePool::addResponseMessage(CZUShort session_id,CZUShort dst_network_addr,CZUChar data_id,CZUChar* data_buffer,CZUShort data_len,bool bNeedTimeoutRetry)
{
	if (dst_network_addr == 0)
	{
		return 1;
	}

	while (responseMessagePoolQueue.full())
	{
		//队列已满 等待放入
		DebugLog<<"ResponseMessage queue is full ,waiting...";
	}

	//将得到的frame组装放入收队列中
	void *bufferMalloced = responseMessagePool.ordered_malloc();
	ResponseMessage *rsp = (ResponseMessage*)bufferMalloced;
	
	rsp->session_id = session_id;
	rsp->dst_addr = dst_network_addr;
	rsp->data_id = data_id;
	rsp->data_len = data_len;
	memcpy(rsp->data,data_buffer,data_len);
	responseMessagePoolQueue.push_back((CZUChar*)bufferMalloced);

	if(bNeedTimeoutRetry)
	{
		void *bufferMallocedTimeout = timeoutMsgPool.ordered_malloc();

		TimerTask* task = (TimerTask*)bufferMallocedTimeout;

		task->isAddTask = true;
		task->type = cz_types::E_Task_Timeout_Retry;
		task->session_id = session_id;
		task->dst_addr = dst_network_addr;
		task->data_id = data_id;
		task->data_len = data_len;
		task->retryTimes = 0;
		task->millisecond = CZMiscs::getMillisecond()+(RETRY_TIMEOUT * 1000);
		memcpy(task->data,data_buffer,data_len);
		timeoutMsgPoolQueue.push_back((CZUChar*)bufferMallocedTimeout);
	}

	return 0;
}

CZInt CZMessagePool::getResponseMessage(cz_types::ResponseMessage* responseMessage)
{
	if(NULL == responseMessage || responseMessagePoolQueue.empty())
	{
		return 1;
	}

	CZUChar *buffer = responseMessagePoolQueue.front();

	ResponseMessage* rsp = (ResponseMessage*)buffer;

    memcpy(responseMessage, rsp, rsp->data_len + sizeof(ResponseMessage) - 1);

    //释放回收资源
    responseMessagePool.ordered_free(buffer);
    responseMessagePoolQueue.pop_front();

	return 0;
}

CZInt CZMessagePool::addTimerTask(cz_types::TimerTask* task)
{
	if(task == NULL || ! task->isAddTask)
	{
		return 1;
	}

	while (timeoutMsgPoolQueue.full())
	{
		//队列已满 等待放入
		DebugLog<<"timeoutRetryPoolQueue queue is full ,waiting...";
	}

	void *bufferMallocedTimeout = timeoutMsgPool.ordered_malloc();

	TimerTask* pTask = (TimerTask*)bufferMallocedTimeout;

    memcpy(pTask, task, task->data_len + sizeof(TimerTask) - 1);
    pTask->isAddTask = true;
    timeoutMsgPoolQueue.push_back((CZUChar*)bufferMallocedTimeout);
    return 0;
}

CZInt CZMessagePool::delTimerTask(CZUShort network_addr,CZUChar data_id)
{
	while (timeoutMsgPoolQueue.full())
	{
		//队列已满 等待放入
		DebugLog<<"timeoutRetryPoolQueue queue is full ,waiting...";
	}

	void *bufferMallocedTimeout = timeoutMsgPool.ordered_malloc();

	TimerTask* pTask = (TimerTask*)bufferMallocedTimeout;

	pTask->isAddTask = false;
	pTask->type = cz_types::E_Task_Normal_Timer;
	pTask->dst_addr = network_addr;
	pTask->data_id = data_id;
	pTask->data_len = 0;
	timeoutMsgPoolQueue.push_back((CZUChar*)bufferMallocedTimeout);

	//DebugLog<<"队列删除超时重传任务,network:"<<ShortToHexStr(network_addr)<<" data_id:"<<ByteToHexStr(data_id);

	return 0;
}

CZInt CZMessagePool::getTimerTask(cz_types::TimerTask* task)
{
	if(timeoutMsgPoolQueue.empty())
	{
		return 1;
	}

	CZUChar *buffer = timeoutMsgPoolQueue.front();

	TimerTask* pTask = (TimerTask*)buffer;

    memcpy(task, pTask, pTask->data_len + sizeof(TimerTask) - 1);

	//释放回收资源
	timeoutMsgPool.ordered_free(buffer);
	timeoutMsgPoolQueue.pop_front();

	return 0;
}