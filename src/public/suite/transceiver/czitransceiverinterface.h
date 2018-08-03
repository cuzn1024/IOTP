#ifndef CZ_ITRANSCEIVERINTERFACE_H
#define CZ_ITRANSCEIVERINTERFACE_H

#ifndef CZ_DEFINES_H
#include <defines.h>
#endif //#ifndef CZ_DEFINES_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#ifndef CZ_ISUITE_H
#include <suite/czisuite.h>
#endif //#ifndef CZ_ISUITE_H

class CZITransceiverInterface : public CZISuite
{
public:
    virtual ~CZITransceiverInterface() {}

public:
    virtual CZInt initialize() = 0;
	virtual CZInt getData(CZUChar receiveBuffer[], CZUInt length) = 0;
    virtual CZInt move(CZUInt length) = 0;
    virtual CZInt write(CZUChar sendBuffer[], CZUInt length) = 0;
    virtual CZInt collectData() { return 0; }
	virtual CZInt close() { return 1; }

protected:
    CZUChar buf[cz_defines::receive_buffer_size];
};

#endif // CZ_ITRANSCEIVERINTERFACE_H
