#include <network/cznode.h>

#include <bitset>

#include <system/czlog.h>

using namespace cz_types;
using namespace cz_defines;
using std::bitset;

class CZFrameBuffer
{
	friend class CZNode;

	CZFrameBuffer()
	{
		frameBuffer = new CZUChar[receive_buffer_size];
		reset();
	}

	~CZFrameBuffer()
	{
		delete[] frameBuffer;
		frameBuffer = 0;
	}

	/*!
	 * \brief     重置缓存
	 *
	 * \exception
	 * \return    void
	 * \remarks  
	 */
	void reset()
	{
		bExsitLastData = false;
		src_network_addr = 0;
		dst_network_addr = 0;
		data_id = 0;
		frame_total_size = 0;
		frame_last_recv_number = 0;
		current_data_len = 0;
		checkBit.reset();
	}

	/*!
	 * \brief     判断是否接收完一个完整包
	 *
	 * \exception
	 * \return    bool
	 * \remarks  
	 */
	bool isFullData()
	{
		if(frame_total_size == 0)
		{
			return false;
		}

		bool bRet = true;

		for(CZUChar i = 0;i<frame_total_size && i<checkBit.size();i++)
		{
			if(!checkBit.test(i))
			{
				bRet = false;
				break;
			}
		}

		return bRet;
	}

	/*!
	 *	是否存在上一次的数据
	 */
	bool bExsitLastData;

	/*!
	*	帧数据缓存
	*/
	CZUChar *frameBuffer;

	/*!
	 *	来源地址
	 */
	CZUShort src_network_addr;

	/*!
	 *	目的地址
	 */
	CZUShort dst_network_addr;

	/*!
	 *	业务id
	 */
	CZUChar data_id;

	/*!
	 *	分帧总数
	 */
	CZUChar frame_total_size;

	/*!
	 *	上一次接收到的分帧号
	 */
	CZUChar frame_last_recv_number;

	/*!
	 *	已经接收到的数据总长度
	 */
	CZUShort current_data_len ;

	/*!
	 *	记录是否没帧都收完毕
	 */
	bitset<MAX_FRAME_COUNT> checkBit;
};

CZNode::CZNode(CZUShort _network_addr, const CZPhysicalAddr &_phy_addr,cz_types::eNodeType node_type) : network_addr(_network_addr), phy_addr(_phy_addr),
    type(node_type), requestMessagePool(receive_buffer_size), requestMessagePoolQueue(rt_message_pool_max_size), timeoutMsgPool(receive_buffer_size),
    sessionId(0)
{
	dataBuffer = new CZUChar[receive_buffer_size];
	requestMessage = (RequestMessage*)dataBuffer;
	pFrameBuffer = new CZFrameBuffer();
	mIndex = 1;
	status = E_Node_Status_Ready;
}

CZNode::~CZNode()
{
	delete[] dataBuffer;
	dataBuffer = 0;

	delete pFrameBuffer;
	pFrameBuffer = 0;
}

CZUShort CZNode::addRequestMessageToQueue(cz_types::RequestMessage* requestMessage)
{
	if(NULL == requestMessage)
	{
		return 1;
	}

	while(requestMessagePoolQueue.full())
	{
		//队列已满 等待放入
		DebugLog<<"RequestMessage queue is full , throw it...";
        return 0;
	}

    void *bufferMalloced = requestMessagePool.ordered_malloc();
    RequestMessage *request = (RequestMessage*)bufferMalloced;

    memcpy(request, requestMessage, requestMessage->data_len + sizeof(RequestMessage) - 1);
	requestMessagePoolQueue.push_back((CZUChar*)bufferMalloced);

	request = NULL;

	return 0;
}

CZUShort CZNode::getMessageFromNode(cz_types::RequestMessage* requestMessage)
{
	if(NULL == requestMessage)
	{
		return 1;
	}

	if(requestMessagePoolQueue.empty())
	{
		return 1;
	}

	CZUChar *buffer = requestMessagePoolQueue.front();

	RequestMessage* req = (RequestMessage*)buffer;

    //拷贝值
    memcpy(requestMessage, req, req->data_len + sizeof(RequestMessage) - 1);

	//释放资源
	requestMessagePool.ordered_free(buffer);
	requestMessagePoolQueue.pop_front();

	req = NULL;

	return 0;
}

CZUShort CZNode::addFrame(LFrame_t* parseFrame,CZUInt sessionId, bool& bExistOkData,bool& bNeedAck)
{
	//是否存在完整数据
	bExistOkData = false;

	//是否需要ACK
	bNeedAck = false;
	
	if (NULL == parseFrame)
	{
		return 1;
	}

	setSessionId(sessionId);

	if(! parseFrame->payload_flg)
	{
		return 2;
	}

	//判断分帧总数和当前分帧 是否大于最大数
	if(parseFrame->payload.div.total > MAX_FRAME_COUNT || parseFrame->payload.div.cur > MAX_FRAME_COUNT)
	{
		DebugLog<<"MAX_FRAME_COUNT:"<<MAX_FRAME_COUNT<<" recv frame total frame size:"<<parseFrame->payload.div.total<<" current frame:"<<parseFrame->payload.div.cur;
		return 1;
	}

	//判断ACK
	if(parseFrame->head.ctrl.bit.ack_req)
	{
		bNeedAck = true;
	}
	
	//表示分帧发送数据
	if(parseFrame->payload.ctrl.bit.div_flg) 
	{
		//检测是否存在前帧数据
		if(pFrameBuffer->bExsitLastData)
		{
			//匹配是否是同一个帧 以及当前帧是否为上一次接收的下一帧
			if(parseFrame->head.src_addr != pFrameBuffer->src_network_addr ||
				parseFrame->head.des_addr != pFrameBuffer->dst_network_addr ||
				parseFrame->payload.pay_data.data_id != pFrameBuffer->data_id || 
				parseFrame->payload.div.total != pFrameBuffer->frame_total_size ||
				parseFrame->payload.div.cur != pFrameBuffer->frame_last_recv_number  + 1)
			{
				DebugLog<<"this frame not match buffer frame,so reset buffer frame,buffer info,src:"<<ShortToHexStr(pFrameBuffer->src_network_addr)<<" dst:"<<ShortToHexStr(pFrameBuffer->dst_network_addr)<<" dataId:"<<ByteToHexStr(pFrameBuffer->data_id)<<" totalSize:"<<ByteToHexStr(pFrameBuffer->frame_total_size)<<" last frame:"<<ByteToHexStr(pFrameBuffer->frame_last_recv_number)
					<<" this frame info,src:"<<ShortToHexStr(parseFrame->head.src_addr)<<" dst:"<<ShortToHexStr(parseFrame->head.des_addr)<<" dataId:"<<ByteToHexStr(parseFrame->payload.pay_data.data_id)<<" totalSize:"<<ByteToHexStr(parseFrame->payload.div.total)<<" current frame:"<<ByteToHexStr(parseFrame->payload.div.cur);
				//重置以往的信息
				pFrameBuffer->reset();
			}
		}

		pFrameBuffer->bExsitLastData = true;
		pFrameBuffer->src_network_addr = parseFrame->head.src_addr;
		pFrameBuffer->dst_network_addr = parseFrame->head.des_addr;
		pFrameBuffer->data_id = parseFrame->payload.pay_data.data_id;
		memcpy(pFrameBuffer->frameBuffer+pFrameBuffer->current_data_len,parseFrame->payload.pay_data.data,parseFrame->payload.pay_data.data_len);

		pFrameBuffer->current_data_len += parseFrame->payload.pay_data.data_len;

		//设置分帧号和总长度
		pFrameBuffer->frame_total_size = parseFrame->payload.div.total;
		pFrameBuffer->frame_last_recv_number = parseFrame->payload.div.cur;

		pFrameBuffer->checkBit.set(parseFrame->payload.div.cur);

		//判断是否所有帧都接收完毕
		if(pFrameBuffer->isFullData())
		{
			//组装放入队列
			requestMessage->msg_type = E_Msg_Type_Client_Request;
			requestMessage->session_id = sessionId;
			requestMessage->src_addr = pFrameBuffer->src_network_addr;
			requestMessage->dst_addr = pFrameBuffer->dst_network_addr;
			requestMessage->data_id  = pFrameBuffer->data_id;
			requestMessage->data_len = pFrameBuffer->current_data_len;
			memcpy(requestMessage->data,pFrameBuffer->frameBuffer,pFrameBuffer->current_data_len);

			bExistOkData = true;

			//放入队列
			addRequestMessageToQueue(requestMessage);

			pFrameBuffer->reset();
		}
	}
	else
	{
		//直接组装成message放到队列中去
		requestMessage->msg_type = E_Msg_Type_Client_Request;
		requestMessage->session_id = sessionId;
		requestMessage->src_addr = parseFrame->head.src_addr;
		requestMessage->dst_addr = parseFrame->head.des_addr;
		requestMessage->data_id  = parseFrame->payload.pay_data.data_id;
		requestMessage->data_len = parseFrame->payload.pay_data.data_len;
		memcpy(requestMessage->data,parseFrame->payload.pay_data.data,parseFrame->payload.pay_data.data_len);

		bExistOkData = true;

		//放入队列
		addRequestMessageToQueue(requestMessage);
	}

	return 0;
}

CZInt CZNode::addTimerTask(cz_types::TimerTask* task)
{
	if(NULL == task)
	{
		return 1;
	}

	if(task->dst_addr <= 0)
	{
		return 1;
	}

	if(timeoutTaskMap.find(task->data_id) != timeoutTaskMap.end())
	{
		DebugLog<<"存在data_id："<<ByteToHexStr(task->data_id)<<" 超时重传任务,先删除再添加";
		delTimerTask(task->data_id);
	}

	void *bufferMalloced = timeoutMsgPool.ordered_malloc();
	TimerTask *pTask = (TimerTask*)bufferMalloced;

    memcpy(pTask, task, task->data_len + sizeof(TimerTask) - 1);

	timeoutTaskMap[task->data_id] = (CZUChar*)bufferMalloced;

	//DebugLog<<"节点添加超时重传任务 network:"<<ShortToHexStr(task->dst_addr)<<" data_id:"<<ByteToHexStr(task->data_id) /*<<" time:"<<task->timestamp*/<<" Task size:"<<timeoutTaskMap.size()+" Pool size:"<<timeoutMsgPool.get_next_size();

	return 0;
}

CZInt CZNode::delTimerTask(CZUChar data_id)
{
	if(data_id <= 0)
	{
		return 1;
	}

	std::unordered_map<CZUChar,CZUChar*>::iterator it = timeoutTaskMap.find(data_id);

	if(it != timeoutTaskMap.end())
	{
		DebugLog<<"节点删除超时重传任务 network:"<<ShortToHexStr(network_addr)<<" data_id:"<<ByteToHexStr(data_id);

		timeoutMsgPool.ordered_free((void*)(it->second));
		timeoutTaskMap.erase(it);
	}

	return 0;
}

bool CZNode::checkIsHaveCase(CZUChar data_id)
{
	if(data_id <= 0)
	{
		return 1;
	}

	std::unordered_map<CZUChar,CZUChar*>::iterator it = timeoutTaskMap.find(data_id);

	if(it != timeoutTaskMap.end())
	{
		return true;
	}

	return false;
}

CZInt CZNode::getTimerTask(cz_types::TimerTask* task)
{
	CZInt iRet = 1;

	CZULongLong lNow = CZMiscs::getMillisecond();

	for(std::unordered_map<CZUChar,CZUChar*>::iterator it = timeoutTaskMap.begin();it != timeoutTaskMap.end();)
	{
		TimerTask* pTask = (TimerTask*)(it->second);

		if(pTask == NULL)
		{
			timeoutTaskMap.erase(it++);
			continue;
		}

        if (pTask->millisecond <= lNow)
        {
            memcpy(task, pTask, pTask->data_len + sizeof(TimerTask) - 1);

			timeoutMsgPool.ordered_free((void*)(it->second));
			timeoutTaskMap.erase(it++);
			iRet = 0;
			break;
		}
		else
		{
			it++;
		}
	}

	return iRet;
}

CZInt CZNode::setCache(const std::string &key, const std::string &value)
{
	if(key.length() == 0 || value.length() == 0)
	{
		return 1;
	}

	cacheMap[key] = value;

	return 0;
}

CZInt CZNode::getCache(const std::string &key, std::string &value, bool isDeleteThisKey)
{
	value = "";

	if(key.length() == 0)
	{
		return 1;
	}

	std::unordered_map<std::string,std::string>::iterator it = cacheMap.find(key);

	if(it == cacheMap.end())
	{
		return 1;
	}

	value = it->second;

	if(isDeleteThisKey)
	{
		cacheMap.erase(it);
	}

	return 0;
}

CZInt CZNode::setFaultSite(CZUShort site)
{
    faultSite = site;

    return 0;
}

CZInt CZNode::getFaultSite(CZUShort &site)
{
    site = faultSite;

    return 0;
}