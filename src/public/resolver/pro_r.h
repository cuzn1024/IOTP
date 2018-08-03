/** - Start - Copyright (C), 2015-2025, Wumeng Tech. Co., Ltd. 2015 *****************************
 *
 *  File Name: pro_r_mac.h
 *  Function:  
 *  Modify: 
 *  <Date>          <Author>       <version>        <Description>
 * 2017-06-01                       V01.00   
 **************************************************************************************************/ 
#ifndef __PRO_R_H
#define __PRO_R_H

/** Include ***************************************************************************************/
#include "pro_config.h"
/** Macro define **********************************************************************************/
/** Type define ***********************************************************************************/

/* The structure define of R2E head based on TH-NET(v0.6.0) protocol document. */
typedef struct {
	union {
		struct {
			uint8_t ack_req  : 1;
			uint8_t          : 1;
			uint8_t listen_flg : 1;
			uint8_t pre_ok   : 1;
			uint8_t   dir    : 1;
			uint8_t pro_type : 3;		
		}bit;
		uint8_t byte;
	}ctrl;
	uint16_t  group_id;
}R2EHead_t;

/* The structure define of E2R head based on TH-NET(v0.6.0) protocol document. */
typedef struct {
	union {
		struct {
			uint8_t          : 4;
			uint8_t   dir    : 1;
			uint8_t pro_type : 3;	
		}bit;
		uint8_t byte;
	}ctrl;
	uint16_t  src_addr;
}E2RHead_t;
/** Constant **************************************************************************************/
/** Variable **************************************************************************************/
/** Private Function ******************************************************************************/
/** Public  Function ******************************************************************************/

/**
 * Action: Unpack the R2E frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void R2E_UnpackHead(R2EHead_t *head, const uint8_t *buf);

/**
 * Action: Unpack the E2R frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void E2R_UnpackHead(E2RHead_t *head, const uint8_t *buf);

/**
 * Action: Pack the R2E frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which will contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void R2E_PackHead(uint8_t *buf, const R2EHead_t *frame);

/**
 * Action: Pack the E2R frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which will contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void E2R_PackHead(uint8_t *buf, const E2RHead_t *frame);




#endif /* __PRO_R_H */


/** - End - Wumeng Tech. Co., Ltd. 2015 *********************************************************/

