#ifndef CZ_TRANSACTIONPROTOCOL_H
#define CZ_TRANSACTIONPROTOCOL_H

#ifndef CZ_DEFINES_H
#include <defines.h>
#endif //#ifndef CZ_DEFINES_H

#ifndef CZ_ITRANSACTIONPROTOCOL_H
#include <transaction/czitransactionprotocol.h>
#endif //#ifndef CZ_ITRANSACTIONPROTOCOL_H

#ifndef BOOST_SHARED_PTR_HPP_INCLUDED
#include <boost/scoped_array.hpp>
#endif //#ifndef BOOST_SHARED_PTR_HPP_INCLUDED

class CZTransactionProtocol : public CZITransactionProtocol
{
public:
    CZTransactionProtocol(void *param = 0);
    virtual ~CZTransactionProtocol();

public:
    PLUGIN_CREATE_HANDLER(CZTransactionProtocol)

    /*!
     * \brief     解析payload数据
     *
     *  若分帧则等待后续帧数据
     * 
     * \param     CZUChar * & frame_buffer 一个帧数据
     * \param     CZUInt & frame_len 一个帧长度
     * \param     cz_types::Frame_Header_New & frame_header 帧头结构
     * \return    CZInt 0 表示已经解析完一个完整数据 1 表示需要继续等待后续帧
     */
    CZInt beginProcess(CZUChar *&frame_buffer, CZUInt &frame_len,cz_types::Frame_Header_New &frame_header);
    CZInt findNextUnit(CZITransactionProtocol::ProcessHandle *processHandle, CZITransactionProtocol::ProcessData *processData, bool multi_frame_flag, cz_types::Data_Header &data_header);
    void endProcess(CZITransactionProtocol::ProcessHandle *processHandle);

    CZInt assemble(CZUChar *data_buffer, CZUInt dataLen, cz_types::Transaction_Header *transaction_header);

private:
	boost::scoped_array<CZUChar> receiveBufferArray;
};

#endif // CZ_TRANSACTIONPROTOCOL_H
