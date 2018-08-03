#ifndef CZ_ITRANSMITPROTOCOL_H
#define CZ_ITRANSMITPROTOCOL_H

#ifndef CZ_DEFINES_H
#include <defines.h>
#endif //#ifndef CZ_DEFINES_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#ifndef CZ_ISUITE_H
#include <czisuite.h>
#endif //#ifndef CZ_ISUITE_H

class CZITransceiverInterface;
class CZITransmitProtocol : public CZISuite
{
public:
    virtual ~CZITransmitProtocol() {}

public:
    virtual CZInt bind(CZITransceiverInterface *transceiverInterface) = 0;
    virtual CZInt unbind() = 0;

	virtual CZInt getOneFrame(CZUChar *& frame_buffer,CZUInt &frame_len,cz_types::Frame_Header_New &frame_header,CZUInt& payload_pos) = 0;
    virtual CZInt sendFrame(CZUChar *data_buffer, CZUInt dataLen, const cz_types::TransmitControlCode &transmitControlCode) = 0;
    virtual CZInt assembleNSend(CZUChar *data_buffer, CZUInt dataLen, cz_types::Frame_Header *frame_header, cz_types::Data_Header *data_header) = 0;
	virtual CZInt close() = 0;
};

#endif // CZ_ITRANSMITPROTOCOL_H
