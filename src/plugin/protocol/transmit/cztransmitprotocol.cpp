#include <transmit/cztransmitprotocol.h>

#include <string.h>
#include <algorithm>
#include <boost/crc.hpp>

#include <utils/crc.h>
#include <czitransceiverinterface.h>

using namespace boost;
using namespace cz_types;

CZTransmitProtocol::CZTransmitProtocol(void *param)
{
    receiveBufferArray.reset(new CZUChar[cz_defines::receive_buffer_size]);
    sendBufferArray.reset(new CZUChar[cz_defines::receive_buffer_size]);

    sendBuffer = &sendBufferArray[0];
    receiveBuffer = &receiveBufferArray[0];

    *(CZUShort*)sendBuffer = 0x7E7E;
}

CZTransmitProtocol::~CZTransmitProtocol(void)
{
}

CZInt CZTransmitProtocol::bind(CZITransceiverInterface *port)
{
    this->transceiverInterface = port;
    return 0;
}

CZInt CZTransmitProtocol::getOneFrame(CZUChar *& frame_buffer,CZUInt &frame_len,cz_types::Frame_Header_New &frame_header,CZUInt& payload_pos)
{
	return 0;
}

CZInt CZTransmitProtocol::sendFrame(CZUChar *data_buffer, CZUInt dataLen, const TransmitControlCode &transmitControlCode)
{
    memcpy(sendBuffer + 4, data_buffer, dataLen);

    *(CZUShort*)(sendBuffer + 2) = dataLen;

    *(CZUShort*)(sendBuffer + 2) |= transmitControlCode.dst_addr_type;
    *(CZUShort*)(sendBuffer + 2) |= transmitControlCode.src_addr_type;
    *(CZUShort*)(sendBuffer + 2) |= transmitControlCode.fw_flag;
    *(CZUChar*)(sendBuffer + 14) |= transmitControlCode.ack;

    *(CZUShort*)(sendBuffer + dataLen + 4) = CZCrc::getCRC16(sendBuffer + 4, dataLen);

    return transceiverInterface->write(sendBuffer, dataLen + 6);
}

/*!
  \internal
  Assemble packets and send. Maybe framing.
  */
CZInt CZTransmitProtocol::assembleNSend(CZUChar *data_buffer, CZUInt dataLen, Frame_Header *src_frame_header, Data_Header *src_data_header)
{
    //7e 7e Frame_Header Data_Header C_Control_Code data_buffer crc
    CZUInt frame_len = dataLen + sizeof(Data_Header) + sizeof(C_Control_Code);

    //jump 2 bytes for 0x7e7e
    CZUInt pos = 2;
    //First assign frame header
    *(Frame_Header*)(sendBuffer + pos) = *src_frame_header;
    //assign data header
    pos += sizeof(Frame_Header);
    *(Data_Header*)(sendBuffer + pos) = *src_data_header;
    //c control code, maybe modified someday
    pos += sizeof(Data_Header);
    sendBuffer[pos] = 0;
    //transaction data
    pos += sizeof(C_Control_Code);
    memcpy(sendBuffer + pos, data_buffer, dataLen);

    //frame length re-calculated
    ((Frame_Header*)(sendBuffer + 2))->data_len = frame_len;

    //crc calculation scope: Data_Header C_Control_Code data_buffer
    pos += dataLen;
    *(CZUShort*)(sendBuffer + pos) = CZCrc::getCRC16(sendBuffer + 4, frame_len);

    return transceiverInterface->write(sendBuffer, pos + 2);
}

CZInt CZTransmitProtocol::fragementing(CZUChar *dstBuffer, CZUInt dstDataLen, CZUChar *srcBuffer, CZUInt srcDataLen)
{
    if (srcDataLen <= 31)
    {
        memcpy(dstBuffer + 1, srcBuffer, srcDataLen);
        *dstBuffer |= srcDataLen; //data length adjust
        *dstBuffer |= 0x40; //data finished
        return 0;
    }

    memcpy(dstBuffer + 1, srcBuffer, 31);
    *dstBuffer |= 0x1F; //data length is 31
    *dstBuffer &= 0x5F; //data not finished
    
    dstBuffer[32] = dstBuffer[0];

    dstDataLen += 32;
    fragementing(dstBuffer + 32, dstDataLen, srcBuffer + 32, srcDataLen - 31);
    return 0;
}

PLUGIN_CREATE_HANDLER_ALIAS(CZTransmitProtocol)

PLUGIN_NAME("transmit")
