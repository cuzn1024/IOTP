#ifndef CZ_TRANSMITPROTOCOL_H
#define CZ_TRANSMITPROTOCOL_H

#ifndef BOOST_SHARED_PTR_HPP_INCLUDED
#include <boost/scoped_array.hpp>
#endif //#ifndef BOOST_SHARED_PTR_HPP_INCLUDED

#ifndef CZ_ITRANSMITPROTOCOL_H
#include <transmit/czitransmitprotocol.h>
#endif //#ifndef CZ_ITRANSMITPROTOCOL_H

class CZTransmitProtocol : public CZITransmitProtocol
{
    enum
    {
        single_frame_len = 255
    };

public:
    CZTransmitProtocol(void *param = 0);
    virtual ~CZTransmitProtocol(void);

public:
    PLUGIN_CREATE_HANDLER(CZTransmitProtocol)

protected:
    CZInt bind(CZITransceiverInterface *transceiverInterface);
    inline CZInt unbind() { transceiverInterface = 0; return 0; }

	/*!
	 * \brief     ��������ȡһ����������ȷ֡����
	 * 
	 * \param     CZUChar * & frame_buffer	֡����
	 * \param     CZUInt & frame_len		֡����
	 * \param     cz_types::Frame_Header_New & frame_header ֡ͷ
	 * \param     CZUInt& payload_pos		payload����λ��
	 * \return    CZInt 0 ��ȷ ��0�쳣
	 */
	CZInt getOneFrame(CZUChar *& frame_buffer,CZUInt &frame_len,cz_types::Frame_Header_New &frame_header,CZUInt& payload_pos);

    CZInt sendFrame(CZUChar *data_buffer, CZUInt dataLen, const cz_types::TransmitControlCode &transmitControlCode);
    CZInt assembleNSend(CZUChar *data_buffer, CZUInt dataLen, cz_types::Frame_Header *frame_header, cz_types::Data_Header *data_header);

    CZInt fragementing(CZUChar *dstBuffer, CZUInt dstDataLen, CZUChar *srcBuffer, CZUInt srcDataLen);

private:
    CZITransceiverInterface *transceiverInterface;
    boost::scoped_array<CZUChar> receiveBufferArray;
    boost::scoped_array<CZUChar> sendBufferArray;
    CZUChar *receiveBuffer, *sendBuffer;
};

#endif //#ifndef CZ_TRANSMITPROTOCOL_H
