#include <network/czconnectionsuite.h>

#include <iostream>
#include <algorithm>
#include <unordered_map>

#include <boost/asio.hpp>
#include <boost/pool/pool.hpp>
#include <boost/circular_buffer.hpp>

#include <defines.h>
#include <system/czlog.h>
#include <czsuitmanager.h>
#include <czconfiguration.h>
#include <network/czsession.h>
#include <network/czsessionmanager.h>
#include <network/cznodemanager.h>
#include <czsingletonmanager.h>
#include <suite/transceiver/czitransceiverinterface.h>

extern "C"
{
#include <resolver/pro_l.h>
};

using namespace std;
using namespace boost;
using namespace cz_types;
using namespace cz_defines;

class CZConnectionSuite_Private
{
    friend class CZConnectionSuite;

    typedef std::unordered_map<std::string, CZSuitManager::PluginCreator> TransceiverProtocolCreators;
    typedef std::unordered_map<std::string, CZITransceiverInterface*> SerialPortPool;


    CZConnectionSuite_Private() :acceptor(io_service), requestFramePool(receive_buffer_size), responseFramePool(receive_buffer_size), requestFramePoolQueue(rt_message_pool_max_size), responseFramePoolQueue(rt_message_pool_max_size)
    {
        readBuffer = new CZUChar[receive_buffer_size];
        readBufferLen = 0;
    }

    ~CZConnectionSuite_Private()
    {
    }

    CZInt initialize()
    {
        CZSingletonManagerInstance.getSuitManager().registerSuit("transceiver", boost::bind(&CZConnectionSuite_Private::transceiverInterfaceFound, this, _1, _2));

        CZUInt seconds = CZSingletonManagerInstance.getConfiguration().getShortFromServerConfig("socket_timeout");

        timeoutSeocnds = seconds > 0 ? seconds : 60;

        return 0;
    }

    void transceiverInterfaceFound(const std::string &name, CZSuitManager::PluginCreator creator)
    {
        transceiverProtocolCreators.insert(TransceiverProtocolCreators::value_type(name, creator));
    }

    void establishSerialPortPool()
    {
        /*
        StringSlist &serialportList = CZSingletonManagerInstance.getConfiguration().get_serialport_depot();

        if (serialportList.empty())
            return;

        CZSuitManager::PluginCreator creator = transceiverProtocolCreators["serialport"];

        BOOST_FOREACH(StringSlist::value_type &node, serialportList)
        {
            CZISuite *suite = creator(&node);

            if (suite != 0)
                serialPortPool.insert(SerialPortPool::value_type(node, reinterpret_cast<CZITransceiverInterface*>(suite)));
            else
                RunLog<<node<<" 打开失败！";
        }
        */
    }

    CZInt establishNetworkListener()
    {
        boost::system::error_code ec;

        int iPort = CZSingletonManagerInstance.getConfiguration().getShortFromServerConfig("server_port");

        std::cout << "服务器TCP端口:" << iPort << std::endl;

        acceptor.open(boost::asio::ip::tcp::v4());
        acceptor.non_blocking(true);
        acceptor.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), iPort), ec);
        if (ec)
        {
            std::cout << "TCP端口绑定异常," << ec.message() << std::endl;
            return 1;
        }

        acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor.listen();

        return 0;
    }

    bool serialportAvailable(const string &port)
    {
        return serialPortPool.find(port) != serialPortPool.end();
    }

    CZISuite *getTransceiverInterface(const string &port)
    {
        return serialPortPool[port];
    }

    CZITransceiverInterface *acceptConnection()
    {
        boost::asio::ip::tcp::socket socket(io_service);
        boost::system::error_code ec;
        acceptor.accept(socket, ec);

        if (ec)
            return 0;

        return reinterpret_cast<CZITransceiverInterface*>(transceiverProtocolCreators["socket"](&socket));
    }

    void checkTimeout()
    {
        CZSessionManager::SessionPool& pool = CZSingletonManagerInstance.getSessionManager().getSessionPool();

        for (CZSessionManager::SessionPool::iterator it = pool.begin(); it != pool.end();)
        {
            if ((it->second)->getStatus() != E_Session_Status_Offline)
            {
                it++;
                continue;
            }

            //改变对应node节点状态
            //TODO: 
            CZUShort siteAddr = CZSingletonManagerInstance.getSessionManager().getSiteAddr(it->first);

            if (siteAddr > 0)
            {
                CZSingletonManagerInstance.getNodeManager().setNodeStatus(siteAddr, E_Node_Status_Ready);
            }

            pool.erase(it++);
        }
    }

    void collectData()
    {
        CZInt iRet = 0;

        CZUInt iNow = CZMiscs::getTimestamp();

        for_each(CZSingletonManagerInstance.getSessionManager().getSessionPool().begin(), CZSingletonManagerInstance.getSessionManager().getSessionPool().end(),
            [this, iNow, &iRet](CZSessionManager::SessionPool::value_type &node) {
            if ((node.second)->getStatus() == E_Session_Status_Offline)
            {
                return;
            }

            if ((node.second)->getLastRecvDataTime() + this->timeoutSeocnds <= iNow)
            {
                if ((node.second)->getStatus() == E_Session_Status_Online)
                    TraceLog << ((node.second)->getId());
                (node.second)->setStatus(E_Session_Status_Offline);
                (node.second)->close();
                return;
            }

            //先触发抽取socket数据
            iRet = (node.second)->collectData();

            if (iRet < 0)
            {
                //socket连接被对方关闭
                TraceLog << ((node.second)->getId());
                (node.second)->setStatus(E_Session_Status_Offline);
                return;
            }

            if (this->requestFramePoolQueue.full())
            {
                return;
            }

            iRet = (node.second)->getData(this->readBuffer, FIND_FRAME_FIRST_LEN);

            if (iRet != 0)
            {
                //不存在一个完整数据
                return;
            }

            node.second->setStatus(E_Session_Status_Online);

            size_t out_start_pos = 0;
            size_t out_frame_len = 0;

            eStatusCode eRet = L_FindFrame(this->readBuffer, FIND_FRAME_FIRST_LEN, &out_start_pos, &out_frame_len);

            if (eRet != PRO_SUCCESS || receive_buffer_size < out_frame_len)
            {
                DebugLog << "Find frame error,out_start_pos:" << out_start_pos << " out_frame_len:" << out_frame_len << " return:" << eRet << " Hex:" << CharsToHexStr(this->readBuffer, FIND_FRAME_FIRST_LEN);

                //未找到开始标志
                if (eRet == ERR_NOSTART)
                {
                    //清除错误数据
                    (node.second)->move(FIND_FRAME_FIRST_LEN);
                }
                else
                {
                    if (out_start_pos != 0)
                    {
                        //清除完整帧之前的数据
                        (node.second)->move(out_start_pos);
                    }
                }

                return;
            }

            this->readBufferLen = out_frame_len + out_start_pos;

            //从buffer中拿出数据
            iRet = (node.second)->getData(this->readBuffer, this->readBufferLen);

            if (iRet != 0)
            {
                //表明不存在完整的一个frame数据
                return;
            }

            eRet = L_FrameCheck(this->readBuffer + out_start_pos, out_frame_len);
            if (eRet != PRO_SUCCESS)
            {
                DebugLog << "帧校验出错！";
                (node.second)->move(this->readBufferLen);
                return;
            }

            (node.second)->move(this->readBufferLen);

            //刷新对应的基站时间
            (node.second)->refreshLastRecvDataTime(iNow);

            //将得到的frame组装放入收队列中
            void *bufferMalloced = this->requestFramePool.ordered_malloc();
            RequestFrame *request = (RequestFrame*)bufferMalloced;
            request->session_id = (node.second)->getId();
            request->message_type = 1;
            request->message_len = out_frame_len;
            memcpy(request->data, this->readBuffer + out_start_pos, out_frame_len);
            this->requestFramePoolQueue.push_back((CZUChar*)bufferMalloced);
        });
    }

    void sendData()
    {
        while (!responseFramePoolQueue.empty())
        {
            CZUChar *buffer = responseFramePoolQueue.front();

            ResponseFrame* rsp = (ResponseFrame*)buffer;

            //通过session_id找到对应的session 发送

            //TODO: 后续优化项
            CZSessionManager::SessionPool::iterator it = CZSingletonManagerInstance.getSessionManager().getSessionPool().find(rsp->session_id);

            if (it != CZSingletonManagerInstance.getSessionManager().getSessionPool().end())
            {
                (it->second)->sendFrame(rsp->data, rsp->message_len);
            }
            else
            {
                //暂时不处理不存在的session情况
            }

            //释放回收资源
            responseFramePool.ordered_free(buffer);
            responseFramePoolQueue.pop_front();

            rsp = NULL;
        }
    }



    CZInt getFrameFromRecvQueue(RequestFrame *&request)
    {
        if (requestFramePoolQueue.empty())
        {
            return 1;
        }

        CZUChar *buffer = requestFramePoolQueue.front();

        RequestFrame* req = (RequestFrame*)buffer;

        memcpy(request, req, req->message_len + sizeof(RequestFrame) - 1);

        //ÊÍ·Å×ÊÔ´
        requestFramePool.ordered_free(buffer);
        requestFramePoolQueue.pop_front();

        req = NULL;

        return 0;
    }

    CZInt addMsgToSendQueue(ResponseFrame* response)
    {
        if (NULL == response)
        {
            return 1;
        }
        while (responseFramePoolQueue.full())
        {
            //队列已满 等待放入
            DebugLog << "ResponseFrame queue is full ,waiting...";
        }

        void *bufferMalloced = responseFramePool.ordered_malloc();
        ResponseFrame *rsp = (ResponseFrame*)bufferMalloced;

        rsp->session_id = response->session_id;
        rsp->message_len = response->message_len;
        memcpy(rsp->data, response->data, response->message_len);
        responseFramePoolQueue.push_back((CZUChar*)bufferMalloced);

        return 0;
    }

    SerialPortPool serialPortPool;
    TransceiverProtocolCreators transceiverProtocolCreators;

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::acceptor acceptor;

    /*!
     *	读取数据缓存
     */
    CZUChar *readBuffer;

    /*!
     *	读取数据缓存长度
     */
    CZUInt readBufferLen;

    /*!
     *	接收数据池
     */
    boost::pool<> requestFramePool;
    boost::circular_buffer<CZUChar*> requestFramePoolQueue;

    /*!
     *	发送数据池
     */
    boost::pool<> responseFramePool;
    boost::circular_buffer<CZUChar*> responseFramePoolQueue;

    /*!
     *	超时时间
     */
    CZUInt timeoutSeocnds;
};

CZConnectionSuite::CZConnectionSuite()
{
    p = new CZConnectionSuite_Private();
}

CZConnectionSuite::~CZConnectionSuite()
{
    delete p;
    p = 0;
}

CZInt CZConnectionSuite::initialize()
{
    return p->initialize();
}

void CZConnectionSuite::establishSerialPortPool()
{
    p->establishSerialPortPool();
}

CZInt CZConnectionSuite::establishNetworkListener()
{
    return p->establishNetworkListener();
}

bool CZConnectionSuite::serialportAvailable(const std::string &port)
{
    return p->serialportAvailable(port);
}

void CZConnectionSuite::initializeSessionPool()
{
    //根据配置初始化串口对应的基站session
    CZConnectionSuite_Private *pri = p;
    std::for_each(p->serialPortPool.begin(), p->serialPortPool.end(), [pri](const CZConnectionSuite_Private::SerialPortPool::value_type &node) {
        CZITransceiverInterface *transceiverInterface = reinterpret_cast<CZITransceiverInterface*>(pri->getTransceiverInterface(node.first));

        CZSingletonManagerInstance.getSessionManager().createSession(transceiverInterface);
    });
}

void CZConnectionSuite::acceptConnection()
{
    CZITransceiverInterface *transceiverInterface = p->acceptConnection();
    if (transceiverInterface == 0)
    {
        return;
    }

    CZUInt sessionId = CZSingletonManagerInstance.getSessionManager().createSession(transceiverInterface);

    TraceLog << "基站接入,分配在链路:" << sessionId;
}

void CZConnectionSuite::checkTimeout()
{
    p->checkTimeout();
}

void CZConnectionSuite::sendData()
{
    p->sendData();
}

void CZConnectionSuite::collectData()
{
    p->collectData();
}

CZInt CZConnectionSuite::getFrameFromRecvQueue(RequestFrame*& request)
{
    return p->getFrameFromRecvQueue(request);
}

CZInt CZConnectionSuite::addMsgToSendQueue(ResponseFrame* response)
{
    return p->addMsgToSendQueue(response);
}
