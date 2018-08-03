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
     *   ѭ�����г���
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
     * \brief     ��ɳ�ʼ������
     *
     * \exception
     * \return    CZInt
     * \remarks  
     */
    CZInt initialize();
	
	/*!
	 * \brief     ��ȡָ����������
	 *
	 * \param     CZUChar receiveBuffer[]
	 * \param     CZUInt length
	 * \exception
	 * \return    CZInt
	 * \remarks   ����ȡ��ɾ��ԭ����
	 */
	CZInt getData(CZUChar receiveBuffer[], CZUInt length);
    
	/*!
	 * \brief     �ƶ���ָ��
	 *
	 * \param     CZUInt length
	 * \exception
	 * \return    CZInt
	 * \remarks   ����ȡ��ɾ��ԭ����
	 */
    CZInt move(CZUInt length);
	
    /*!
     * \brief     socket������
     *
     * \param     CZUChar sendBuffer[]
     * \param     CZUInt length
     * \exception
     * \return    CZInt
     * \remarks  
     */
    CZInt write(CZUChar sendBuffer[], CZUInt length);

    /*!
     * \brief     socket������
     *
     * \exception
     * \return    CZInt
     * \remarks  
     */
    CZInt collectData();

	/*!
	 * \brief     �ر���·
	 *
	 * \exception
	 * \return    CZInt
	 * \remarks  
	 */
	CZInt close();
private:
    boost::asio::ip::tcp::socket client;
    
	/*!
	 *	���ݻ���ѭ������
	 */
    CZUChar *readBuffer;

	/*!
	 *	�ɶ�ȡ��λ��
	 */
	CZUInt read_pos;

	/*!
	 *	��д���λ��
	 */
	CZUInt write_pos;

	/*!
	 *	�ѽ��յ���δȡ�ߵ����ݳ���
	 */
	CZUInt data_len;
};

#endif //#ifndef CZ_SOCKET_H
