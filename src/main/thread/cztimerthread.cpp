#include <thread/cztimerthread.h>

#include <czsingletonmanager.h>
#include <thread/czthreadmanager.h>
#include <thread/czmessagepool.h>
#include <network/czsessionmanager.h>
#include <network/czsession.h>
#include <utils/czmiscs.h>
#include <network/cznodemanager.h>
#include <system/czlog.h>

using namespace cz_types;
using namespace cz_defines;

CZTimerThread::CZTimerThread(void)
{
	taskBuffer = new CZUChar[receive_buffer_size];
	task = (TimerTask*)taskBuffer;

	reqBuffer = new CZUChar[receive_buffer_size];
	req = (RequestMessage*)reqBuffer;
}

CZTimerThread::~CZTimerThread(void)
{
	delete taskBuffer;
	taskBuffer = 0;

	delete reqBuffer;
	reqBuffer = 0;
}

void CZTimerThread::run()
{
	CZULongLong lNow = 0;

	CZInt iRet = 1;

	std::unordered_map<CZULongLong,CZUShort> tempTimeoutMap;

	std::unordered_map<std::string,CZULongLong>::iterator checkExsitIterator;

	std::unordered_map<CZULongLong,CZUShort>::iterator delIterator;

    while (CZSingletonManagerInstance.getThreadManager().stillRunning())
    {
		lNow = CZMiscs::getMillisecond();

		//从多线程队列中拿数据 ，解析组装放到每个对应的Node节点中
		iRet = CZSingletonManagerInstance.getMessagePool().getTimerTask(task);

		if(!iRet && task->dst_addr > 0)
		{
			if(task->isAddTask)
			{
				taskTimeMap[task->millisecond] = task->dst_addr;

				taskKeyMap[CZMiscs::i2s(task->dst_addr)+CZMiscs::i2s(task->data_id)] = task->millisecond;

				CZSingletonManagerInstance.getNodeManager().addTimerTask(task);
			}
			else
			{
				checkExsitIterator = taskKeyMap.find(CZMiscs::i2s(task->dst_addr)+CZMiscs::i2s(task->data_id));

				if(checkExsitIterator != taskKeyMap.end())
				{
					delIterator = taskTimeMap.find(checkExsitIterator->second);

					if(delIterator != taskTimeMap.end())
					{
						taskTimeMap.erase(delIterator);
					}

					taskKeyMap.erase(checkExsitIterator);
				}

				CZSingletonManagerInstance.getNodeManager().delTimerTask(task->dst_addr,task->data_id);
			}
		}

		//查看时间map中第一个位置的时间是否满足条件
		for(std::unordered_map<CZULongLong,CZUShort>::iterator it = taskTimeMap.begin();it != taskTimeMap.end();)
		{
			if(it->first > lNow)
			{
				break;
			}

			iRet = 0;

			do 
			{
				iRet = CZSingletonManagerInstance.getNodeManager().getTimerTask(it->second,task);

				if(!iRet)
				{
					if(task == NULL)
					{
						break;
					}

					if(task->type == E_Task_Timeout_Retry)
					{
						if(task->retryTimes < RETRY_TIMES)
						{
							task->millisecond = lNow+(RETRY_TIMEOUT * 1000);
							task->retryTimes++;

                            if (task->session_id == 0)
                            {
                                task->session_id = CZSingletonManagerInstance.getNodeManager().getNodeOverSessionRoad(task->dst_addr);
                            }

							DebugLog<<lNow<<" 开始重传任务network:"<<ShortToHexStr(task->dst_addr)<<" data_id:"<<ByteToHexStr(task->data_id)<<" 重传次数:"<<task->retryTimes<<" Time:"<<task->millisecond;
							CZSingletonManagerInstance.getMessagePool().addResponseMessage(task->session_id,task->dst_addr,task->data_id,task->data,task->data_len);

							tempTimeoutMap[task->millisecond] = task->dst_addr;
							CZSingletonManagerInstance.getNodeManager().addTimerTask(task);
						}
					}
					else if(task->type == E_Task_Normal_Timer)
					{
						req->src_addr = task->dst_addr;
						req->dst_addr = Server;
						req->msg_type = E_Msg_Type_TimerTask;
						req->session_id = task->session_id;
						req->data_id = task->data_id;
						req->data_len = task->data_len;

						if(task->data_len > 0)
						{
							memcpy(req->data,task->data,task->data_len);
						}

						if(!CZSingletonManagerInstance.getNodeManager().addRequestMessageToQueue(task->dst_addr,req))
						{
							DebugLog<<lNow<<" 开始定时任务network:"<<ShortToHexStr(task->dst_addr)<<" data_id:"<<ByteToHexStr(task->data_id)<<" Time:"<<task->millisecond;
							CZSingletonManagerInstance.getMessagePool().addRequestNode(task->dst_addr);
						}
					}
				}

			} while (iRet == 0);

			taskTimeMap.erase(it++);
		}

		for(std::unordered_map<CZULongLong,CZUShort>::iterator it = tempTimeoutMap.begin();it != tempTimeoutMap.end();it++)
		{
			taskTimeMap[it->first] = it->second;
		}

		tempTimeoutMap.clear();
    }
}
