/** - Start - Copyright (C), 2015-2025, Wumeng Tech. Co., Ltd. 2015 *****************************
 *
 *  File Name: pro_payload.c
 *  Function:  
 *  Modify: 
 *  <Date>          <Author>       <version>        <Desc>
 * 2017-06-01                        V01.00            
 **************************************************************************************************/ 

/** Include ***************************************************************************************/
#include "pro_payload.h"
/** Macro define **********************************************************************************/
/** Type define ***********************************************************************************/
/** Constant **************************************************************************************/
/** Variable **************************************************************************************/
/** Private Function ******************************************************************************/
/** Public  Function ******************************************************************************/

/**
 * Action: Clear the payload field based on TH-NET(v0.6.0) protocol document.
 * Input : @payload : The payload which will be cleared.
 * Output: none
 * Return: none
 */
void Payload_StructInit(PayLoad_t *payload)
{
	if(NULL == payload)
		return;
	memset(payload, 0, sizeof(PayLoad_t));
}

/**
 * Action: Unpack the payload field based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which contain the payload.
 *         @buf_len : The size of the real payload length.
 * Output: @payload : Will store the unpacked field.   
 * Return: This return value can be one of the enum values:
 *         @PRO_SUCCESS : Successful unpack the payload buffer.
 *         @ERR_EMPTY : the buf/payload pointer is NULL.
 */

eStatusCode Payload_Unpack(PayLoad_t *payload, const uint8_t *buf, size_t buf_len)
{
	size_t  p2p_len;
	size_t  div_len;
	size_t  dataid_len; /* the length of real data and its data id.*/

	if((buf == NULL) || (payload == NULL))
		return ERR_EMPTY;

	/* get pctrl field. */
	payload->ctrl.byte = *buf;
	/* skip the size of pctrl. */
	buf++;

	p2p_len = payload->ctrl.bit.p2p_len;
	/* if p2p total length is greater than zero, 
	 * that shows there must be p2p data id here. 
	 */
	if(p2p_len > 0)
	{
		payload->p2p_flg = 1;
		/* get p2p data id field. */
		payload->p2p.data_id = *buf;
		/* skip the size of p2p data id field field. */
		buf++;
		/* if p2p total length is greater than 1, 
		 * that shows there must be data here. 
		 */
		if(p2p_len > 1)
		{
			payload->p2p.data_len = p2p_len - 1;
            if(payload->p2p.data_len > MAX_P2P_DATA_LEN)
                return ERR_MAX_P2P_DATA_LEN;
			if(NULL != payload->p2p.data)
		    {                
				/* copy the p2p data to new buffer. */
				memcpy(payload->p2p.data, buf, payload->p2p.data_len);    
			}else{
                return ERR_EMPTY;
            }
			/* skip the size of p2p data size. */
			buf += payload->p2p.data_len;
		}
	}
	else
	{
		payload->p2p_flg = 0;
	}

	if(payload->ctrl.bit.div_flg)
	{
		payload->div.total = *(uint8_t *)buf;
		payload->div.cur   = *(uint8_t *)(buf + 1);
		/* skip the size of div size. */
		buf += 2;
		div_len = 2;
	}
	else
	{
		div_len = 0;
	}
	/* the payload does not contain the data and dataid size, 
	 * we should calculate it based on input buffer size. 
	 */
	dataid_len = buf_len - p2p_len - div_len - P_PCTRL_LEN;
	/* if dataid_len is greater than zero, that shows
	 * there must be data id here. 
	 */
	if(dataid_len > 0)
	{
		payload->data_flg = 1;
		payload->pay_data.data_id = *buf;
		/* skip the size of data id. */
		buf++;
		/* calculate the real data size. */
		payload->pay_data.data_len = dataid_len - P_DATAID_LEN;
		if(payload->pay_data.data_len > 0)
		{
   			if(payload->pay_data.data_len > MAX_DATA_LEN)
				return ERR_MAX_DATA_LEN;
			if(NULL != payload->pay_data.data)
			{
				/* copy the data to new buffer. */
				memcpy(payload->pay_data.data, buf, payload->pay_data.data_len);
			}else{
				return ERR_EMPTY;
			}
		}
	}
	else
	{
		payload->data_flg = 0;
	}

	return PRO_SUCCESS;
}


/**
 * Action: Pack the payload field based on TH-NET(v0.6.0) protocol document.
 * Input : @payload : The structure which contain the payload information.
 * Output: @buf : The buffer where store the packed field.  
 *         @buf_len : the size of packed payload buffer.
 * Return: This return value can be one of the enum values:
 *         @PRO_SUCCESS : Successful pack the payload.
 *         @ERR_EMPTY : the payload_t/buf is NULL.
 */

eStatusCode Payload_Pack(uint8_t *buf, size_t *buf_len, const PayLoad_t *payload)
{
	uint8_t *pbuf = buf;
	uint8_t ctrl;
	
	if((payload == NULL) || (buf == NULL) || (buf_len == NULL))
		return ERR_EMPTY;
	
	/* pack the control field and clear the p2p length, 
	 * we will pack it later(if have p2p). */
	*pbuf = payload->ctrl.byte & 0xf0;
	ctrl = *pbuf;
	/* skip the control size. */
	pbuf++;

	if(payload->p2p_flg == 1)
	{
		/* pack the p2p data id field to out buffer. */
		*(uint8_t *)pbuf = payload->p2p.data_id;
		/* skip the p2p data id size. */
		pbuf++;
		if(payload->p2p.data_len > 0)
		{   
            if(payload->p2p.data_len > MAX_P2P_DATA_LEN)
                return ERR_MAX_P2P_DATA_LEN;
			if(NULL == payload->p2p.data)
				return ERR_EMPTY;
			/* copy the p2p data to packed buffer. */
			memcpy(pbuf, payload->p2p.data, payload->p2p.data_len);	
			/* skip the p2p data size. */
			pbuf += payload->p2p.data_len;
		}
		/* calculate the p2p length and pack it to the control field. '1' is dataid length. */
		*buf = ctrl | ((payload->p2p.data_len + 1) & 0x0f);
	}

	if(payload->ctrl.bit.div_flg)
	{
		*(uint8_t *)pbuf = payload->div.total;
		*(uint8_t *)(pbuf + 1) = payload->div.cur;
		/* skip the div pack size. */
		pbuf += 2;
	}

	if(payload->data_flg == 1)
	{
		*(uint8_t *)pbuf = payload->pay_data.data_id;
		/* skip the dataid length. */
		pbuf++;
		if(payload->pay_data.data_len > 0)
		{
			if(payload->pay_data.data_len > MAX_DATA_LEN)
				return ERR_MAX_DATA_LEN;
			if(NULL == payload->pay_data.data)
				return ERR_EMPTY;
			/* copy the data to buffer. */
			memcpy(pbuf, payload->pay_data.data, payload->pay_data.data_len);
			pbuf += payload->pay_data.data_len;
		}
	}  
	/* calculate the packed buffer size(ie. the out buffer size). */
	*buf_len = pbuf - buf;

	return PRO_SUCCESS;
}

/** - End - Wumeng Tech. Co., Ltd. 2015 *********************************************************/ 

