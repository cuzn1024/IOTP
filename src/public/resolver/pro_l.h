/** - Start - Copyright (C), 2015-2025, Wumeng Tech. Co., Ltd. 2015 *****************************
 *
 *  File Name: pro_l.h
 *  Function:  
 *  Modify: 
 *  <Date>          <Author>       <version>        <Desc>
 * 2017-06-01                       V01.00   
 **************************************************************************************************/ 
#ifndef __PRO_L_H
#define __PRO_L_H

/** Include ***************************************************************************************/
#include "crc.h"
#include "pro_config.h"
#include "pro_payload.h"
#include <stddef.h>
#include <string.h>
/** Macro define **********************************************************************************/							  
/** Type define ***********************************************************************************/
typedef struct {
	uint16_t des_addr;
	uint16_t src_addr;
	uint8_t  frame_id;
	union {
		struct {
			uint8_t ack_req : 1;
			uint8_t ack_flg : 1;
			uint8_t         : 3;
			uint8_t pro_type: 3;
		}bit;
		uint8_t byte;
    }ctrl;
}LHead_t;

typedef struct {
    LHead_t   head;
	uint8_t   payload_flg;
	PayLoad_t payload;
}LFrame_t;

/** Constant **************************************************************************************/
/** Variable **************************************************************************************/
/** Public  Function ******************************************************************************/
void L_StructInit(LFrame_t *frame);
eStatusCode L_FindFrame(const uint8_t *buf, size_t buf_len, size_t *start_pos, size_t *frame_len);
eStatusCode L_FrameCheck(uint8_t* frame, size_t frame_len);
eStatusCode L_UnPackHead(PayBuf_t *pay, LHead_t *head, const uint8_t *buf, size_t buf_len);
eStatusCode L_UnPack(LFrame_t *frame, const uint8_t *buf, size_t buf_len);

eStatusCode L_PackHead(uint8_t *buf, size_t *buf_len, const LHead_t* head, const PayBuf_t* pay);
eStatusCode L_Pack(uint8_t* buf, size_t *buf_len, const LFrame_t* frame);
eStatusCode L_InitFrame(LFrame_t *frame);


#endif /* __PRO_L_H */

/** - End - Wumeng Tech. Co., Ltd. 2015 *********************************************************/

