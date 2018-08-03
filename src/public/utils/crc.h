#ifndef CZ_CRC_H
#define CZ_CRC_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

class CZCrc
{
public:
    static CZUShort getCRC16(CZUChar *pData, CZUInt lenth);
};

#endif // CZ_CRC_H
