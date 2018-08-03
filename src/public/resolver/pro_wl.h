/** - Start - Copyright (C), 2015-2025, Wumeng Tech. Co., Ltd. 2015 *****************************
 *
 *  File Name: pro_wl.h
 *  Function:  
 *  Modify: 
 *  <Date>          <Author>       <version>        <Desc>
 * 2017-06-01                       V01.00   
 **************************************************************************************************/ 
#ifndef __PRO_WL_H
#define __PRO_WL_H

/** Include ***************************************************************************************/
#include "crc.h"
#include "pro_m.h"
#include "pro_r.h"
#include "pro_c.h"
#include "pro_payload.h"
#include "pro_config.h"
#include <stddef.h>
#include <string.h>
/** Macro define **********************************************************************************/
/** Type define ***********************************************************************************/
typedef struct {
	ProType pro_type;
	union {
		M2EHead_t m2e;
		E2MHead_t e2m;
		R2EHead_t r2e;
		E2RHead_t e2r;
		C2EHead_t c2e;
		E2CHead_t e2c;
	}head;
}WLHead_t;

typedef struct {
	WLHead_t  wl_head;
	uint8_t   payload_flg;
	PayLoad_t payload;
}WLFrame_t;

/** Constant **************************************************************************************/
/** Variable **************************************************************************************/
/** Private Function ******************************************************************************/
/** Public  Function ******************************************************************************/
eStatusCode WL_FrameUnpackHead(PayBuf_t *pay, WLFrame_t *frame, const uint8_t *buf, size_t buf_len);
eStatusCode WL_FrameUnpack(WLFrame_t *frame, const uint8_t *buf, size_t buf_len);
eStatusCode WL_FramePack(uint8_t *buf, size_t *buf_len, const WLFrame_t *frame);
eStatusCode WL_FramePackHead(uint8_t *buf, size_t *buf_len, const WLHead_t *wl_head, const PayBuf_t *pay_buf);
eStatusCode WL_PayloadUnpack(PayLoad_t *payload, const uint8_t *buf, size_t buf_len);
eStatusCode WL_FrameCheck(const uint8_t *buf, size_t buf_len);
ProType     WL_GetTypeDir(const uint8_t *buf);
eStatusCode WL_InitFrame(ProType type, WLFrame_t *frame);



#endif /* __PRO_WL_H */

/** - End - Wumeng Tech. Co., Ltd. 2015 *********************************************************/

