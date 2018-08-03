/*!
    \brief      Transaction data assembler
    \details    The upper tier is application data and the below tier is transmit protocol.
    \file       transaction/cztransactionprotocol.cpp
    \author     cuzn
    \version    0.1.0
    \date       2017/4/5
    \pre        Should be created by CZConnectionSuite
*/

#include <transaction/cztransactionprotocol.h>

#include <string.h>

using namespace cz_types;

CZTransactionProtocol::CZTransactionProtocol(void *param)
{
	receiveBufferArray.reset(new CZUChar[cz_defines::receive_buffer_size]);
}

CZTransactionProtocol::~CZTransactionProtocol()
{
}

PLUGIN_CREATE_HANDLER_ALIAS(CZTransactionProtocol)

CZInt CZTransactionProtocol::beginProcess(CZUChar *&frame_buffer, CZUInt &frame_len,cz_types::Frame_Header_New &frame_header)
{
	//分析本次接收到的数据内容位置
	return 0;
}

CZInt CZTransactionProtocol::findNextUnit(CZITransactionProtocol::ProcessHandle *processHandle, CZITransactionProtocol::ProcessData *processData, bool multi_frame_flag,
    cz_types::Data_Header &data_header)
{
    CZUChar *frame_buffer = processHandle->frame_buffer;
    CZUInt frameLen = processHandle->frame_len;
    CZUInt &frame_index = processHandle->frame_index;

    processData->data_buffer = frame_buffer;
    
    //inner
    CZUChar controlCode;
    CZUInt dataLen;

    //data length
    CZUInt &data_index = processData->data_len = 0;

    if (multi_frame_flag)
    {
        while (frame_index < frameLen)
        {
            controlCode = frame_buffer[frame_index];
            dataLen = controlCode & 0x1F;

            if (frame_index + dataLen > frameLen) //check if data length error
                break;

            //jump control code
            memmove(frame_buffer + data_index, frame_buffer + frame_index + 1, dataLen - 1);
            frame_index += dataLen + 1;
            data_index += dataLen;
        }

        if (frame_index == frameLen)
        {
            return 0;
        }
    }
    else
    {
        while (frame_index < frameLen)
        {
            if (data_header.protocol != RSITE + SITE_BASE)
            {
                controlCode = frame_buffer[frame_index];
                //add transaction type
                dataLen = (controlCode & 0x1F);
                bool more_data_flag = !(controlCode & 0x40);

                if (frame_index + dataLen + 1 > frameLen)
                    break;

                if (data_index == 0)
                {
                    ++data_index;
                    frame_buffer[0] = frame_buffer[frame_index + 1];
                }

                if (dataLen > 0)
                    memmove(frame_buffer + data_index, frame_buffer + ++frame_index + 1, dataLen); //jump control code

                data_index += dataLen;
                frame_index += dataLen + 1;
                if (!more_data_flag)
                {
                    return 0;
                }
            }
            else
            {
                memmove(frame_buffer, frame_buffer + frame_index, frameLen - frame_index);
                data_index = frameLen - frame_index;
                frame_index = frameLen;

                return 0;
            }
        }
    }

    return 1;
}

void CZTransactionProtocol::endProcess(CZITransactionProtocol::ProcessHandle *processHandle)
{
    delete processHandle;
}

/*!
  \internal
  Add transaction headers to original data.
  Copy from back to front in order to efficiency improvement.
  */
CZInt CZTransactionProtocol::assemble(CZUChar *data_buffer, CZUInt dataLen, Transaction_Header *transaction_header)
{
    enum {
        Data_Piece = 31
    };
    CZInt src_pos = dataLen / Data_Piece * Data_Piece;
    CZInt dst_pos = dataLen / Data_Piece * (Data_Piece + sizeof(Transaction_Header));

    memmove(data_buffer + dst_pos + sizeof(Transaction_Header), data_buffer + src_pos, dataLen - src_pos);
    memcpy(data_buffer + dst_pos, transaction_header, sizeof(Transaction_Header));
    ((Transaction_Header*)(data_buffer + dst_pos))->len = dataLen - src_pos;
    ((Transaction_Header*)(data_buffer + dst_pos))->finish = 1;
    dst_pos -= Data_Piece + sizeof(Transaction_Header);
    src_pos -= Data_Piece;
    dataLen += sizeof(Transaction_Header);

    while (src_pos > -1)
    {
        memmove(data_buffer + dst_pos + 1, data_buffer + src_pos, Data_Piece);
        memcpy(data_buffer + dst_pos, transaction_header, sizeof(Transaction_Header));
        ((Transaction_Header*)(data_buffer + dst_pos))->len = Data_Piece;
        ((Transaction_Header*)(data_buffer + dst_pos))->finish = 0;
        dst_pos -= Data_Piece;
        src_pos -= Data_Piece;
        dataLen++;
    }

    return dataLen;
}

PLUGIN_NAME("transaction")
