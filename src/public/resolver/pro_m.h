/** - Start - Copyright (C), 2015-2025, Wumeng Tech. Co., Ltd. 2015 *****************************
 *
 *  File Name: pro_m.h
 *  Function:  
 *  Modify: 
 *  <Date>          <Author>       <version>        <Description>
 * 2017-06-01                       V01.00   
 **************************************************************************************************/ 
#ifndef __PRO_M_H
#define __PRO_M_H

/** Include ***************************************************************************************/
#include "pro_config.h"
/** Macro define **********************************************************************************/
/** Type define ***********************************************************************************/

/* The structure define of M2E head based on TH-NET(v0.6.0) protocol document. */
typedef struct {
	union {
		struct {
			uint8_t ack_req : 1;
			uint8_t         : 1;
			uint8_t pend    : 1;
			uint8_t         : 1;
			uint8_t   dir   : 1;
			uint8_t pro_type: 3;
		}bit;
		uint8_t byte;
	}ctrl;
	uint16_t  des_addr;
}M2EHead_t;

/* The structure define of E2M head based on TH-NET(v0.6.0) protocol document. */
typedef struct {
	union {
		struct {
			uint8_t ack_req : 1;
			uint8_t         : 1;
			uint8_t  pend   : 1;
			uint8_t         : 1;
			uint8_t   dir   : 1;
			uint8_t pro_type: 3;
		}bit;
		uint8_t byte;
	}ctrl;
	uint16_t  src_addr;
}E2MHead_t;

/** Constant **************************************************************************************/
/** Variable **************************************************************************************/
/** Private Function ******************************************************************************/
/** Public  Function ******************************************************************************/
/**
 * Action: Unpack the M2E frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void M2E_UnpackHead(M2EHead_t *head, const uint8_t *buf);

/**
 * Action: Unpack the E2M frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void E2M_UnpackHead(E2MHead_t *head, const uint8_t *buf);

/**
 * Action: Pack the M2E frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which will contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void M2E_PackHead(uint8_t *buf, const M2EHead_t *head);

/**
 * Action: Pack the E2M frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which will contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void E2M_PackHead(uint8_t *buf, const E2MHead_t *head);



#endif /* __PRO_M_H */



/** - End - Wumeng Tech. Co., Ltd. 2015 *********************************************************/

