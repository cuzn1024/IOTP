/** - Start - Copyright (C), 2015-2025, Wumeng Tech. Co., Ltd. 2015 *****************************
 *
 *  File Name: pro_r.c
 *  Function:  
 *  Modify: 
 *  <Date>          <Author>       <version>        <Description>
 * 2017-06-01                        V01.00            
 **************************************************************************************************/ 

/** Include ***************************************************************************************/
#include "pro_r.h"
/** Macro define **********************************************************************************/
/** Type define ***********************************************************************************/
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
void R2E_UnpackHead(R2EHead_t *head, const uint8_t *buf)
{
	/* skip the length of frameLen field. */
	buf++;
	head->ctrl.byte = *buf;
	/* skip the length of ctrl field. */
	buf++;
	head->group_id = *(uint16_t *)buf;
}

/**
 * Action: Unpack the E2R frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void E2R_UnpackHead(E2RHead_t *head, const uint8_t *buf)
{
	/* skip the length of frameLen field. */
	buf++;
	head->ctrl.byte = *buf;
	/* skip the length of ctrl field. */
	buf++;
	head->src_addr = *(uint16_t *)buf;
}
/**
 * Action: Pack the R2E frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which will contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void R2E_PackHead(uint8_t *buf, const R2EHead_t *head)
{
	/* skip the length of frame_len. */
	buf++;
	*buf = head->ctrl.byte;
	/* skip the length of ctrl. */
	buf++;
	*(uint16_t *)buf = head->group_id;
}

/**
 * Action: Pack the E2R frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which will contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void E2R_PackHead(uint8_t *buf, const E2RHead_t *head)
{
	/* skip the length of frame_len. */
	buf++;
	*buf = head->ctrl.byte;
	/* skip the length of ctrl. */
	buf++;
	*(uint16_t *)buf = head->src_addr;
}






/** - End - Wumeng Tech. Co., Ltd. 2015 *********************************************************/
