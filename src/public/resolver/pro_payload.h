/** - Start - Copyright (C), 2015-2025, Wumeng Tech. Co., Ltd. 2015 *****************************
 *
 *  File Name: pro_payload.h
 *  Function:  
 *  Modify: 
 *  <Date>          <Author>       <version>        <Desc>
 * 2017-06-01                       V01.00   
 **************************************************************************************************/ 

#ifndef __PRO_PAY_H
#define __PRO_PAY_H

/** Include ***************************************************************************************/
#include "pro_config.h"
#include <stddef.h>
#include <string.h>
/** Macro define **********************************************************************************/
/** Type define ***********************************************************************************/
typedef union {
	struct
	{
	    uint8_t p2p_len  : 4;
		uint8_t div_flg  : 1;
        uint8_t          : 1;
        uint8_t          : 1;
	    uint8_t security : 1;
	}bit;
	uint8_t byte;
}PCtrl_t;


typedef struct {
	uint8_t *data;
	uint8_t  data_len;
	uint8_t  data_id;
}Data_t;

typedef struct {
	uint8_t  total;
	uint8_t  cur;
}DivPack_t;

typedef struct {
	DivPack_t div;
	Data_t    p2p;
	Data_t    pay_data;
	PCtrl_t   ctrl;
	uint8_t   p2p_flg  : 1; /* Indicates whether p2p is available */
	uint8_t   data_flg : 1; /* Indicates whether pay_data and its id are available */
}PayLoad_t;

typedef struct {
	size_t   payload_len;
	uint8_t *payload;
}PayBuf_t;

/** Constant **************************************************************************************/
/** Variable **************************************************************************************/
/** Public  Function ******************************************************************************/
/**
 * Action: Clear the payload field based on TH-NET(v0.6.0) protocol document.
 * Input : @payload : The payload which will be cleared.
 * Output: none
 * Return: none
 */
void Payload_StructInit(PayLoad_t *payload);

/**
 * Action: Unpack the payload field based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which contain the payload.
 *         @buf_len : The size of the real payload length.
 * Output: @payload : Will store the unpacked field.   
 * Return: This return value can be one of the enum values:
 *         @PRO_SUCCESS : Successful unpack the payload buffer.
 *         @ERR_EMPTY : the buf/payload pointer is NULL.
 */
eStatusCode Payload_Unpack(PayLoad_t *payload, const uint8_t *buf, size_t buf_len);

/**
 * Action: Pack the payload field based on TH-NET(v0.6.0) protocol document.
 * Input : @payload : The structure which contain the payload information.
 * Output: @buf : The buffer where store the packed field.  
 *         @buf_len : the size of packed payload buffer.
 * Return: This return value can be one of the enum values:
 *         @PRO_SUCCESS : Successful pack the payload.
 *         @ERR_EMPTY : the payload/buf is NULL.
 */
eStatusCode Payload_Pack(uint8_t *buf, size_t *buf_len, const PayLoad_t *payload);








#endif /* __PRO_PAY_H */

/** - End - Wumeng Tech. Co., Ltd. 2015 *********************************************************/

