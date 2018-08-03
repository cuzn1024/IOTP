#include <serialport/czserialport.h>

#include <string>

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/basic_serial_port.hpp>

using std::string;
using namespace boost;
using namespace boost::asio;

class CZSerialPort_Private
{
    enum {
        read_len = 1024
    };
    friend class CZSerialPort;

    CZSerialPort_Private(void *param = 0);
    ~CZSerialPort_Private();

    CZSerialPort_Private(const CZSerialPort_Private &sp) {}

    //interfaces for CZSerialPort
    CZInt initialize();
	CZInt getFrame(CZUChar receiveBuffer[], CZUInt &length);
    CZInt sync_write(CZUChar sendBuffer[], CZUInt length);
    CZInt getData();

    //iterative callback
    void async_read(CZUChar[], boost::system::error_code errorCode, std::size_t bytes_transferred);

    serial_port *sp;
    io_service io;
    const string port;

    //data buffer, circular and over-writable
    CZUChar *readBuffer;

    //CZUInt consumer_total, producer_total, read_consumer, read_producer;
	/*!
	 *	可读取的位置
	 */
	CZUInt read_pos;

	/*!
	 *	可写入的位置
	 */
	CZUInt write_pos;

	/*!
	 *	接收的数据总长度
	 */
	CZUInt data_len;
};

CZSerialPort_Private::CZSerialPort_Private(void *param) :port(*(reinterpret_cast<string*>(param)))
{
    sp = new serial_port(io);
    readBuffer = new CZUChar[read_len];
	read_pos = write_pos = data_len = 0;
}

CZSerialPort_Private::~CZSerialPort_Private(void)
{
    sp->close();
    delete sp;
    sp = 0;

    delete []readBuffer;
    readBuffer = 0;
}

#include <Windows.h>
CZInt CZSerialPort_Private::initialize()
{
    boost::system::error_code errorcode;
    sp->open(port, errorcode);
    if (errorcode.value() != 0) return -1;

    sp->set_option(serial_port::baud_rate(115200));
    sp->set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
    sp->set_option(serial_port_base::character_size(8));
    sp->set_option(serial_port_base::parity(serial_port_base::parity::none));
    sp->set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));

    SetupComm(sp->lowest_layer().native_handle(), 1024, 1024);

    async_read(0, boost::system::error_code(), 0);

    return 0;
}

void CZSerialPort_Private::async_read(CZUChar[], boost::system::error_code, std::size_t bytes_transferred)
{

	/*!
	 *	

	 //首先判断当前缓存是否已满
	 if (data_len >= read_len)
	 {
	 return 0;
	 }

	 boost::system::error_code ignored_error;

	 size_t recv_len = client.read_some(boost::asio::buffer(readBuffer + write_pos, write_pos > read_pos ? read_len - write_pos : read_pos - write_pos ), ignored_error);

	 if ( !ignored_error && recv_len > 0 )
	 {
	 data_len += recv_len;

	 write_pos = (write_pos + recv_len) % read_len;
	 }

	 return 0;

	 */

	/*

    if (bytes_transferred > 0)
    {
        read_producer += bytes_transferred;
        producer_total += bytes_transferred;

        if (read_producer > read_len - 1)
            read_producer = 0;
    }

    sp->async_read_some(buffer(readBuffer + read_producer, read_len - read_producer), boost::bind(&CZSerialPort_Private::async_read, this, readBuffer, _1, _2));
	*/
}

CZInt CZSerialPort_Private::getData()
{
    return io.poll();
}

CZInt CZSerialPort_Private::getFrame(CZUChar receiveBuffer[], CZUInt &length)
{
	length = 0;

	if (data_len <= 0)
	{
		return 1;
	}

	//用于记录freame位置
	int begin_pos = -1;
	int end_pos = -1;

	for (CZUInt i = 0 ;i < data_len;i++)
	{

		if (readBuffer[(read_pos + i) % read_len] == 0x7e && readBuffer[(read_pos + i + 1) % read_len] == 0x7e)
		{
			if (begin_pos < 0)
			{
				begin_pos = (read_pos + i) % read_len;
			}
			else
			{
				end_pos = (read_pos + i) % read_len;
			}
		}
	}

	if (begin_pos < 0 || end_pos < 0)
	{
		return 1;
	}

	//循环队列前后位置接替
	if (end_pos > begin_pos)
	{
		length = end_pos - begin_pos;
		memcpy(receiveBuffer, readBuffer + begin_pos, length);
	}
	else
	{
		memcpy(receiveBuffer, readBuffer + begin_pos, read_len - begin_pos);

		memcpy(receiveBuffer + read_len - begin_pos, readBuffer, end_pos);
	}

	read_pos = end_pos;

	return 0;
}

CZInt CZSerialPort_Private::sync_write(CZUChar sendBuffer[], CZUInt length)
{
    boost::system::error_code error;
    return write(*sp, buffer(sendBuffer, length));
}

CZSerialPort::CZSerialPort(void *param) :p(new CZSerialPort_Private(param))
{
}

CZSerialPort::~CZSerialPort(void)
{
    delete p;
    p = 0;
}

CZInt CZSerialPort::initialize()
{
    return p->initialize();
}

CZInt CZSerialPort::getFrame(CZUChar receiveBuffer[], CZUInt &length)
{
	return p->getFrame(receiveBuffer,length);
}

CZInt CZSerialPort::write(CZUChar sendBuffer[], CZUInt length)
{
    return p->sync_write(sendBuffer, length);
}

CZInt CZSerialPort::getData(CZUChar receiveBuffer[], CZUInt length)
{
	return 0;
}

CZInt CZSerialPort::move(CZUInt length)
{
    return 0;
}

CZInt CZSerialPort::collectData()
{
    return p->getData();
}

PLUGIN_CREATE_HANDLER_ALIAS(CZSerialPort)

PLUGIN_NAME("serialport")
