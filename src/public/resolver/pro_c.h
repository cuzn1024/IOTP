/** - Start - Copyright (C), 2015-2025, Wumeng Tech. Co., Ltd. 2015 *****************************
 *
 *  File Name: pro_m.h
 *  Function:  
 *  Modify: 
 *  <Date>          <Author>       <version>        <Description>
 * 2017-06-01                       V01.00   
 **************************************************************************************************/ 
#ifndef __PRO_C_H
#define __PRO_C_H

/** Include ***************************************************************************************/
#include "pro_config.h"
/** Macro define **********************************************************************************/
/** Type define ***********************************************************************************/

/* The structure define of C2E head based on TH-NET(v0.6.0) protocol document. */
typedef struct {
	union {
		struct {
			uint8_t ack_req  : 1;
			uint8_t          : 1;
			uint8_t  bc_flg  : 1;
			uint8_t rdt_mode : 1;
			uint8_t   dir    : 1;
			uint8_t pro_type : 3;	
		}bit;
		uint8_t byte;
	}ctrl;
	union {
		struct {
			uint8_t re_tran : 4;
			uint8_t cnt     : 4;	
		}bit;
		uint8_t byte;
	}rdt;
}C2EHead_t;

/* The structure define of E2C head based on TH-NET(v0.6.0) protocol document. */
typedef struct {
	union {
		struct {
			uint8_t ack_req  : 1;
			uint8_t          : 1;
			uint8_t          : 1;
			uint8_t rdt_mode : 1;
			uint8_t   dir    : 1;
			uint8_t pro_type : 3;
		}bit;
		uint8_t byte;
	}ctrl;
	union {
		struct {
			uint8_t re_tran : 4;
			uint8_t cnt     : 4;	
		}bit;
		uint8_t byte;
	}rdt;
}E2CHead_t;
/** Constant **************************************************************************************/
/** Variable **************************************************************************************/
/** Private Function ******************************************************************************/
/** Public  Function ******************************************************************************/

/**
 * Action: Unpack the C2E frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void C2E_UnpackHead(C2EHead_t *head, const uint8_t *buf);

/**
 * Action: Unpack the E2C frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void E2C_UnpackHead(E2CHead_t *head, const uint8_t *buf);

/**
 * Action: Pack the C2E frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which will contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void C2E_PackHead(uint8_t *buf, const C2EHead_t *head);

/**
 * Action: Pack the E2C frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which will contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void E2C_PackHead(uint8_t *buf, const E2CHead_t *head);




#endif /* __PRO_C_H */


/** - End - Wumeng Tech. Co., Ltd. 2015 *********************************************************/

