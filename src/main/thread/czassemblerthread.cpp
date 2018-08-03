#include <thread/czassemblerthread.h>

#include <system/czlog.h>
#include <network/czsession.h>
#include <czsingletonmanager.h>
#include <thread/czthreadmanager.h>
#include <thread/czmessagepool.h>
#include <network/czconnectionsuite.h>
#include <network/cznodemanager.h>
#include <network/czsessionmanager.h>
#include <network/cznode.h>
#include <defines.h>

#include <utils/crc.h>

using namespace boost;
using namespace cz_types;
using namespace cz_defines;

CZAssemblerThread::CZAssemblerThread():sendFrameId(0)
{
	readFrameBuffer = new CZUChar[receive_buffer_size];
	requestFrame = (RequestFrame*)readFrameBuffer;
	writeFrameBuffer = new CZUChar[receive_buffer_size];
	responseFrame = (ResponseFrame*)writeFrameBuffer;

	readMessageBuffer = new CZUChar[receive_buffer_size];
	requestMessage = (RequestMessage*)readMessageBuffer;

	writeMessageBuffer = new CZUChar[receive_buffer_size];
	responseMessage = (ResponseMessage*)writeMessageBuffer;

	frameBuffer = new CZUChar[receive_buffer_size];
	frame = new LFrame_t();
	frame->payload.pay_data.data = (uint8_t *)frameBuffer;
	frame->payload.p2p.data = NULL;

	frameSendBuffer = new CZUChar[receive_buffer_size];
	frameSend = new LFrame_t();
	frameSend->payload.pay_data.data = (uint8_t *)frameSendBuffer;
	frameSend->payload.p2p.data = NULL;
}

CZAssemblerThread::~CZAssemblerThread()
{
	delete[]readFrameBuffer;
	readFrameBuffer = 0;

	delete[]writeFrameBuffer;
	writeFrameBuffer = 0;

	delete[] readMessageBuffer;
	readMessageBuffer = 0;

	delete[] writeMessageBuffer;
	writeMessageBuffer = 0;

	delete[] frameBuffer;
	frameBuffer = 0;

	delete frame;
	frame = 0;

	delete[] frameSendBuffer;
	frameSendBuffer = 0;

	delete frameSend;
	frameSend = 0;
}

void CZAssemblerThread::run()
{
	//用于记录返回值 判断是否存在数据
	CZInt iRet = 0;

	int index = 0;

	eStatusCode eRet = PRO_SUCCESS;

	CZUShort session_id  = 0;

	while (CZSingletonManagerInstance.getThreadManager().stillRunning())
	{
		//从接收frame队列中读取一帧数据
		iRet = CZSingletonManagerInstance.getConnectionSuite().getFrameFromRecvQueue(requestFrame);

		if(!iRet)
		{
			//为空判断
			if(NULL == requestFrame)
			{
				continue;
			}

			//解包
			eRet = L_UnPack(frame,requestFrame->data,requestFrame->message_len);

			if(eRet != PRO_SUCCESS)
			{
				DebugLog<<"Frame unpack fail,return:"<<eRet<<" Hex:"<<CharsToHexStr(requestFrame->data,requestFrame->message_len);
				continue;
			}

            if (frame->payload_flg != 0)
			    TraceLog<<"链路层收到"<<ShortToHexStr((CZUShort)(frame->head.src_addr))<<"发来的一帧数据,数据ID:$i:"<<ByteToHexStr((CZUChar)(frame->payload.pay_data.data_id))<<"$ 数据内容:"<<CharsToHexStr(frame->payload.pay_data.data,frame->payload.pay_data.data_len)<<" 整帧:"<<CharsToHexStr(requestFrame->data,requestFrame->message_len);
            else
			    TraceLog<<"链路层收到"<<ShortToHexStr((CZUShort)(frame->head.src_addr))<<"发来的一帧数据,无负载 整帧:"<<CharsToHexStr(requestFrame->data,requestFrame->message_len);

			//判断是否是回复帧 回复帧不需要进行逻辑处理
			if(frame->head.ctrl.bit.ack_req)
			{
				//TODO: 应答帧逻辑暂时不确定
				DebugLog<<"收到心跳帧："<<ByteToHexStr((CZUChar)(frame->head.frame_id));

				DebugLog<<"发送响应心跳帧"<<ShortToHexStr((CZUShort)(frame->head.src_addr));
                resetFrame(frameSend);

				frameSend->head.frame_id  = frame->head.frame_id;
				frameSend->head.src_addr = SERVER_NETWORK_ADDR;
				frameSend->head.des_addr = frame->head.src_addr;
				frameSend->head.ctrl.bit.ack_flg = 1;
				frameSend->payload.data_flg = 0;
				frameSend->payload.p2p_flg = 0;
				responseFrame->session_id = requestFrame->session_id;
				size_t message_len = 0;
				//组装
				eRet = L_Pack(responseFrame->data,&message_len,frameSend);
                
				if(eRet == PRO_SUCCESS)
				{
					responseFrame->message_len = (CZUShort)message_len;
                    if (frame->payload_flg != 0)
					    TraceLog<<"链路层发送"<<ShortToHexStr((CZUShort)(frame->head.src_addr))<<"发来的一帧数据,帧ID"<<(int)(frame->head.frame_id)<<"数据ID:$i:"<<ByteToHexStr((CZUChar)(frame->payload.pay_data.data_id))<<"$ 返回ACK";
                    else
					    TraceLog<<"链路层发送"<<ShortToHexStr((CZUShort)(frame->head.src_addr))<<"发来的一帧数据,无负载 返回ACK"<<" 整帧:" << CharsToHexStr(responseFrame->data, responseFrame->message_len);

					CZSingletonManagerInstance.getConnectionSuite().addMsgToSendQueue(responseFrame);
				}
				else
				{
					DebugLog<<"frame ack pack data fail,function return:"<<eRet;
				}

				continue;
			}

			//是否存在完整数据
			bool bExistOkData = false;
			//是否需要ack 帧ack和分帧最后一帧ack
			bool bNeedAck = false;

			//将解析得到的frame结构放入node中，通过返回值判断对应的node节点中是否存在完整包数据
			iRet = CZSingletonManagerInstance.getNodeManager().addFrameToNode(frame,requestFrame->session_id,bExistOkData, bNeedAck);

			if (iRet)
			{
				continue;
			}

			if (bNeedAck)
			{
				//组装frame数据 剔除data数据部分后 直接添加的sendFrame队列中
				resetFrame(frameSend);

				frameSend->head.frame_id  = frame->head.frame_id;
				frameSend->head.src_addr = SERVER_NETWORK_ADDR;
				frameSend->head.des_addr = frame->head.src_addr;
				frameSend->head.ctrl.bit.ack_flg = 1;
				frameSend->payload.data_flg = 0;
				frameSend->payload.p2p_flg = 0;
				responseFrame->session_id = requestFrame->session_id;
				size_t message_len = 0;
				//组装
				eRet = L_Pack(responseFrame->data,&message_len,frameSend);

				if(eRet == PRO_SUCCESS)
				{
					responseFrame->message_len = (CZUShort)message_len;
					TraceLog<<"链路层收到"<<ShortToHexStr((CZUShort)(frame->head.src_addr))<<"发来的一帧数据,帧ID"<<(int)(frame->head.frame_id)<<"数据ID:$i:"<<ByteToHexStr((CZUChar)(frame->payload.pay_data.data_id))<<"$ 返回ACK";
					CZSingletonManagerInstance.getConnectionSuite().addMsgToSendQueue(responseFrame);
				}
				else
				{
					DebugLog<<"frame ack pack data fail,function return:"<<eRet;
				}
			}

			//通过上述返回值 若有 则将该node节点放入RequestNode队列中
			if (bExistOkData)
			{
				//添加node节点到待处理队列中
				CZSingletonManagerInstance.getMessagePool().addRequestNode(frame->head.src_addr);
			}
		}		
		
		//从ResponseMessage队列中读取数据 通过结构中数据判断所属node，得到头部信息
		
		iRet = CZSingletonManagerInstance.getMessagePool().getResponseMessage(responseMessage);

		if (!iRet)
		{
			session_id = responseMessage->session_id;

			if (session_id == 0)
			{
				session_id = CZSingletonManagerInstance.getNodeManager().getNodeOverSessionRoad(responseMessage->dst_addr);

        //        if (session_id == 0)
        //        {
				    //DebugLog<<"网络地址:"<<ShortToHexStr(responseMessage->dst_addr)<<" 未发现链路";

				    //continue;
        //        }
			}

			CZUShort one_frame_max_len = frame_max_length;

			//判断是否需要分包
			if(responseMessage->data_len > one_frame_max_len)
			{
				DebugLog << "需分帧" << (responseMessage->data_len);
				//循环组包
				resetFrame(frameSend);

				frameSend->head.src_addr = SERVER_NETWORK_ADDR;
				frameSend->head.des_addr = responseMessage->dst_addr;
				frameSend->payload.pay_data.data_id = responseMessage->data_id;
				responseFrame->session_id = session_id;

				CZUShort total_size = (responseMessage->data_len % one_frame_max_len == 0)?(responseMessage->data_len / one_frame_max_len):( (responseMessage->data_len / one_frame_max_len) + 1 );

				CZUShort one_frame_len = 0;

				for(CZUShort index = 0;index < total_size ;index++)
				{
					if(index * one_frame_max_len + one_frame_max_len > responseMessage->data_len)
					{
						//最后一个包
						one_frame_len = responseMessage->data_len - index * one_frame_max_len;
					}
					else
					{
						one_frame_len = one_frame_max_len;
					}

					frameSend->head.frame_id  = (uint8_t)getFrameId();
					frameSend->payload.pay_data.data_len = (uint8_t)one_frame_len;
					frameSend->payload.data_flg = 1;
					frameSend->payload.p2p_flg = 0;
					frameSend->payload_flg = 1;
					memcpy(frameSend->payload.pay_data.data,responseMessage->data+(index * one_frame_max_len),one_frame_len);

					DebugLog<<"分帧发送:"<<index<<" ---> "<<index * one_frame_max_len<<" -----  "<<(index * one_frame_max_len + one_frame_len - 1);

					size_t message_len = 0;
					//组装
					eRet = L_Pack(responseFrame->data,&message_len,frameSend);

					if(eRet == PRO_SUCCESS)
					{
						responseFrame->message_len = message_len;
						TraceLog<<"链路层发给"<<"Session id "<<ShortToHexStr(responseFrame->session_id)<<" 目的地址 "<<ShortToHexStr(frameSend->head.des_addr)<<"一帧数据,数据ID:$i:"<<ByteToHexStr(frameSend->payload.pay_data.data_id)<<"$ 数据内容:"<<CharsToHexStr(frameSend->payload.pay_data.data,frameSend->payload.pay_data.data_len)<<" 整帧:"<<CharsToHexStr(responseFrame->data,responseFrame->message_len);

						CZSingletonManagerInstance.getConnectionSuite().addMsgToSendQueue(responseFrame);
					}
					else
					{
						DebugLog<<"frame loop response pack data fail,function return:"<<eRet;
					}
				}
			}
			else
			{
				resetFrame(frameSend);

				frameSend->head.frame_id  = getFrameId();
				frameSend->head.src_addr = SERVER_NETWORK_ADDR;
				frameSend->head.des_addr = responseMessage->dst_addr;
				frameSend->payload.pay_data.data_id = responseMessage->data_id;
				frameSend->payload.pay_data.data_len = (uint8_t)responseMessage->data_len;
				frameSend->payload.data_flg = 1;
				frameSend->payload.p2p_flg = 0;
				frameSend->payload_flg = 1;
				memcpy(frameSend->payload.pay_data.data,responseMessage->data,responseMessage->data_len);
				responseFrame->session_id = session_id;

				size_t message_len = 0;
				//组装
				eRet = L_Pack(responseFrame->data,&message_len,frameSend);
                
				if(eRet == PRO_SUCCESS)
				{
                    CZNodeManager::NodePool &nodePool = CZSingletonManagerInstance.getNodeManager().getNodePool();
                    CZUShort sessionId = responseFrame->session_id;
                    CZNodeManager::NodePool::iterator it = std::find_if(nodePool.begin(), nodePool.end(), [sessionId](const std::pair<CZUShort, std::shared_ptr<CZNode>> &node){return (node.second->getSessionId() == sessionId); });

					responseFrame->message_len = message_len;
					TraceLog<<"链路层发给"<<ShortToHexStr(it == nodePool.end() ? 0xffff : it->first)<<" Session id "<<ShortToHexStr(responseFrame->session_id)<<" 目的地址 "<<ShortToHexStr(frameSend->head.des_addr)<<"一帧数据,数据ID:$i:"<<ByteToHexStr(frameSend->payload.pay_data.data_id)<<"$ 数据内容:"<<CharsToHexStr(frameSend->payload.pay_data.data,frameSend->payload.pay_data.data_len)<<" 整帧:"<<CharsToHexStr(responseFrame->data,responseFrame->message_len);
					CZSingletonManagerInstance.getConnectionSuite().addMsgToSendQueue(responseFrame);
				}
				else
				{
					DebugLog<<"frame response pack data fail,function return:"<<eRet;
				}
			}
		}
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	}
}

CZUChar CZAssemblerThread::getFrameId()
{
	return sendFrameId++;
}

void CZAssemblerThread::resetFrame(LFrame_t* frame)
{
	if(NULL == frame)
	{
		return;
	}

	uint8_t* pData = frame->payload.pay_data.data;

	L_StructInit(frame);

	frame->payload.pay_data.data = pData;

	pData = NULL;
}
