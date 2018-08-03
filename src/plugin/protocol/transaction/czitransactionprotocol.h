#ifndef CZ_ITRANSACTIONPROTOCOL_H
#define CZ_ITRANSACTIONPROTOCOL_H

#ifndef CZ_TYPES_H
#include <types.h>
#endif //#ifndef CZ_TYPES_H

#ifndef CZ_ISUITE_H
#include <czisuite.h>
#endif //#ifndef CZ_ISUITE_H

class CZITransactionProtocol : public CZISuite
{
public:
    struct ProcessHandle
    {
        CZUChar *frame_buffer;
        CZUInt frame_len;
        CZUInt frame_index;
    };

    struct ProcessData
    {
        CZUChar *data_buffer;
        CZUInt data_len;
    };

public:
    CZITransactionProtocol() {}
    virtual ~CZITransactionProtocol() {}

public:
    virtual CZInt beginProcess(CZUChar *&frame_buffer, CZUInt &frame_len,cz_types::Frame_Header_New &frame_header) = 0;
    virtual CZInt findNextUnit(ProcessHandle *processHandle, ProcessData *processData, bool multi_frame_flag, cz_types::Data_Header &data_header) = 0;
    virtual void endProcess(ProcessHandle *processHandle) = 0;

    virtual CZInt assemble(CZUChar *data_buffer, CZUInt dataLen, cz_types::Transaction_Header *transaction_header) = 0;
};

#endif // CZ_ITRANSACTIONPROTOCOL_H
