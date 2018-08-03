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
     * \brief     ����payload����
     *
     *  ����֡��ȴ�����֡����
     * 
     * \param     CZUChar * & frame_buffer һ��֡����
     * \param     CZUInt & frame_len һ��֡����
     * \param     cz_types::Frame_Header_New & frame_header ֡ͷ�ṹ
     * \return    CZInt 0 ��ʾ�Ѿ�������һ���������� 1 ��ʾ��Ҫ�����ȴ�����֡
     */
    CZInt beginProcess(CZUChar *&frame_buffer, CZUInt &frame_len,cz_types::Frame_Header_New &frame_header);
    CZInt findNextUnit(CZITransactionProtocol::ProcessHandle *processHandle, CZITransactionProtocol::ProcessData *processData, bool multi_frame_flag, cz_types::Data_Header &data_header);
    void endProcess(CZITransactionProtocol::ProcessHandle *processHandle);

    CZInt assemble(CZUChar *data_buffer, CZUInt dataLen, cz_types::Transaction_Header *transaction_header);

private:
	boost::scoped_array<CZUChar> receiveBufferArray;
};

#endif // CZ_TRANSACTIONPROTOCOL_H
