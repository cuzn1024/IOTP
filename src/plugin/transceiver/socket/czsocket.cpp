#include <socket/czsocket.h>

#include <iostream>
#include <string>

#include <boost/exception/all.hpp>

CZSocket::CZSocket(void *param) :client(std::move(*(reinterpret_cast<boost::asio::ip::tcp::socket*>(param))))
{
    readBuffer = new CZUChar[BUFFER_LENGTH];
	client.non_blocking(true);
	read_pos = write_pos = data_len = 0;
}

CZSocket::~CZSocket(void)
{
	if (client.is_open())
	{
		client.close();
	}

    delete[]readBuffer;
    readBuffer = 0;

    read_pos = write_pos = data_len = 0;
}

CZInt CZSocket::initialize()
{
    return 0;
}

CZInt CZSocket::move(CZUInt length)
{
	if (length == 0)
	{
		//参数非法
		return 1;
	}

	if (data_len < length)
	{
		//说明当前缓冲器不足索取的长度
		return 2;
	}

	read_pos = (read_pos + length) % BUFFER_LENGTH;
	data_len -= length;

	return 0;
}

CZInt CZSocket::getData(CZUChar receiveBuffer[], CZUInt length)
{
	if (length == 0)
	{
		//参数非法
		return 1;
	}

	if (data_len < length)
	{
		//说明当前缓冲器不足索取的长度
		return 2;
	}

    if (read_pos + length > BUFFER_LENGTH)
    {//翻转
        memcpy(receiveBuffer, readBuffer + read_pos, BUFFER_LENGTH - read_pos);
        memcpy(receiveBuffer + BUFFER_LENGTH - read_pos, readBuffer, length - (BUFFER_LENGTH - read_pos));
    }
    else
    {
        memcpy(receiveBuffer, readBuffer + read_pos, length);
    }

    return 0;
}

CZInt CZSocket::write(CZUChar sendBuffer[], CZUInt length)
{
	int iRet = -1;
	
	try
	{
		iRet = client.send(boost::asio::buffer(sendBuffer, length));
	}
	catch(...)
	{
		iRet = -1;
	}
    
	return iRet;
}

CZInt CZSocket::collectData()
{
	//首先判断当前缓存是否已满
	if (data_len >= BUFFER_LENGTH)
	{
		return 0;
	}

	boost::system::error_code ignored_error;

    size_t lenToBeReceived = 0;

	if (write_pos >= read_pos)
	{
		lenToBeReceived = BUFFER_LENGTH - write_pos;
	}
	else
	{
		lenToBeReceived = read_pos - write_pos;
	}

	size_t lenActuallyReceived = client.read_some(boost::asio::buffer(readBuffer + write_pos, lenToBeReceived), ignored_error);

	if (ignored_error && 
        (ignored_error.value() == boost::asio::error::connection_reset || ignored_error.value() == boost::asio::error::eof ||
        ignored_error.value() == boost::asio::error::bad_descriptor || ignored_error.value() == boost::asio::error::operation_aborted))
	{
		return -1;
	}

    if (!ignored_error && lenActuallyReceived > 0)
    {
		data_len += lenActuallyReceived;
		write_pos = (write_pos + lenActuallyReceived) % BUFFER_LENGTH;
    }

    return 0;
}

CZInt CZSocket::close()
{
	if (client.is_open())
	{
		client.close();
	}

	return 0;
}

PLUGIN_CREATE_HANDLER_ALIAS(CZSocket)

PLUGIN_NAME("socket")
