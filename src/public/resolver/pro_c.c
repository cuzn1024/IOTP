/** - Start - Copyright (C), 2015-2025, Wumeng Tech. Co., Ltd. 2015 *****************************
 *
 *  File Name: pro_c.c
 *  Function:  
 *  Modify: 
 *  <Date>          <Author>       <version>        <Description>
 * 2017-06-01                        V01.00            
 **************************************************************************************************/ 

/** Include ***************************************************************************************/
#include "pro_c.h"
/** Macro define **********************************************************************************/
/** Type define ***********************************************************************************/
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
void C2E_UnpackHead(C2EHead_t *head, const uint8_t *buf)
{
	/* skip the length of frameLen field. */
	buf++;
	head->ctrl.byte = *buf;
	/* skip the length of ctrl field. */
	buf++;
	if(head->ctrl.bit.rdt_mode)
	{
		head->rdt.byte = *(uint8_t *)buf;
	}
}

/**
 * Action: Unpack the E2C frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void E2C_UnpackHead(E2CHead_t *head, const uint8_t *buf)
{
	/* skip the length of frameLen field. */
	buf++;
	head->ctrl.byte = *buf;
	/* skip the length of ctrl field. */
	buf++;
	if(head->ctrl.bit.rdt_mode)
	{
		head->rdt.byte = *(uint8_t *)buf;
	}
}
/**
 * Action: Pack the C2E frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which will contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void C2E_PackHead(uint8_t *buf, const C2EHead_t *head)
{
	/* skip the length of frame_len. */
	buf++;
	*buf = head->ctrl.byte;
	/* skip the length of ctrl. */
	buf++;
	if(head->ctrl.bit.rdt_mode)
	{
	   *buf = head->rdt.byte;
	}
}

/**
 * Action: Pack the E2C frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which will contain the frame head.
 * Output: @head : The structure where store the unpacked head.    
 * Return: none
 */
void E2C_PackHead(uint8_t *buf, const E2CHead_t *head)
{	
	/* skip the length of frame_len. */
	buf++;
	*buf = head->ctrl.byte;
	/* skip the length of ctrl. */
	buf++;
	if(head->ctrl.bit.rdt_mode)
	{
	   *buf = head->rdt.byte;
	}
}





/** - End - Wumeng Tech. Co., Ltd. 2015 *********************************************************/
