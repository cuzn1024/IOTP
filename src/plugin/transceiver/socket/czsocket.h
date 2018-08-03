#ifndef CZ_SOCKET_H
#define CZ_SOCKET_H

#ifndef BOOST_ASIO_HPP
#include <boost/asio.hpp>
#endif //#ifndef BOOST_ASIO_HPP

#ifndef CZ_ITRANSCEIVERINTERFACE_H
#include <suite/transceiver/czitransceiverinterface.h>
#endif //#ifndef CZ_ITRANSCEIVERINTERFACE_H

class CZSocket : public CZITransceiverInterface
{
	/*!
     *   循环队列长度
     */
    enum {
        BUFFER_LENGTH = 1024
    };

public:
    CZSocket(void *param = 0);
    virtual ~CZSocket(void);

public:
    PLUGIN_CREATE_HANDLER(CZSocket)

protected:

	/*!
     * \brief     完成初始化操作
     *
     * \exception
     * \return    CZInt
     * \remarks  
     */
    CZInt initialize();
	
	/*!
	 * \brief     提取指定长度数据
	 *
	 * \param     CZUChar receiveBuffer[]
	 * \param     CZUInt length
	 * \exception
	 * \return    CZInt
	 * \remarks   仅提取不删除原数据
	 */
	CZInt getData(CZUChar receiveBuffer[], CZUInt length);
    
	/*!
	 * \brief     移动读指针
	 *
	 * \param     CZUInt length
	 * \exception
	 * \return    CZInt
	 * \remarks   仅提取不删除原数据
	 */
    CZInt move(CZUInt length);
	
    /*!
     * \brief     socket发数据
     *
     * \param     CZUChar sendBuffer[]
     * \param     CZUInt length
     * \exception
     * \return    CZInt
     * \remarks  
     */
    CZInt write(CZUChar sendBuffer[], CZUInt length);

    /*!
     * \brief     socket收数据
     *
     * \exception
     * \return    CZInt
     * \remarks  
     */
    CZInt collectData();

	/*!
	 * \brief     关闭链路
	 *
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt close();
private:
    boost::asio::ip::tcp::socket client;
    
	/*!
	 *	数据缓冲循环队列
	 */
    CZUChar *readBuffer;

	/*!
	 *	可读取的位置
	 */
	CZUInt read_pos;

	/*!
	 *	可写入的位置
	 */
	CZUInt write_pos;

	/*!
	 *	已接收到且未取走的数据长度
	 */
	CZUInt data_len;
};

#endif //#ifndef CZ_SOCKET_H
