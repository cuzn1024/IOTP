/** - Start - Copyright (C), 2015-2025, Wumeng Tech. Co., Ltd. 2015 *****************************
 *
 *  File Name: pro_r_mac.c
 *  Function:  
 *  Modify: 
 *  <Date>          <Author>       <version>        <Desc>
 * 2017-06-01                        V01.00            
 **************************************************************************************************/ 

/** Include ***************************************************************************************/
#include "pro_l.h"

/** Macro define **********************************************************************************/
/** Type define ***********************************************************************************/
/** Constant **************************************************************************************/
/** Variable **************************************************************************************/
/** Private Function ******************************************************************************/
/** Public  Function ******************************************************************************/
/**
 * Action: Clear the L-frame based on TH-NET(v0.6.0) protocol document.
 * Input : @frame : The pointer which point the L-frame.
 * Output: None.   
 * Return: None.
 */
void L_StructInit(LFrame_t *frame)
{
	if(frame == NULL)
		return;
    memset(frame, 0, sizeof(LFrame_t));
}

/*
 * Action: Find the L-frame in a buffer based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buf which contain the frame.
 *         @buf_len : The length of @buf.
 * Output: @start_pos : The L-frame's offset in the @buf.
           @frame_len : The length of L-frame.
 * Return: This return value can be one of the enum values:
 *         L_SUCCESS : Successful find the L-frame.
 *         ERR_EMPTY : the buf is NULL.
 *         ERR_BUFLEN : the buf_len is less than start field size and len field size.
 *         ERR_HEADLEN : the size after start field is less than len field size.
 *         ERR_NOSTART : can't find the start field.
 */
eStatusCode L_FindFrame(const uint8_t *buf, size_t buf_len, size_t *start_pos, size_t *frame_len)
{
	size_t i;
	size_t len;
	
	if(buf == NULL)
		return ERR_EMPTY;
	
	/* if the size of buf less than the sum of start field size and len field size,
 	 * then there must be no frame. */
    if(buf_len < L_START_LEN + L_HEADLEN_LEN)
		return ERR_BUFLEN;
	/* If there is just 3 bytes data left, there must be no complete frame. */
	for(i = 0; i < buf_len - 3; i++)
	{
		if(*(buf + i) == L_START1)
		{
			if(*(buf + i + 1) == L_START2)
			{
				/* after found the head, we should make sure there is enough size for len field */
				if(buf_len - i - L_START_LEN >= L_HEADLEN_LEN)
				{
					*start_pos = i;
                    /* Get the framelen field. */
					len = *(uint16_t*)(buf + i + L_START_LEN);
                    /* Calculate the whole LFrame length. */
					*frame_len = len + L_START_LEN + L_HEADLEN_LEN + L_CRC_LEN;                   
					return PRO_SUCCESS;
				}
				else
				{
					return ERR_HEADLEN;
				}
			}
		}
	}
	return ERR_NOSTART;
}

/**
 * Action: Unpack the L-frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buf which contain the L-frame.
 *         @buf_len : The length of the L-frame.
 * Output: @frame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode L_UnPackHead(PayBuf_t *pay, LHead_t *head, const uint8_t *buf, size_t buf_len)
{
	uint16_t crc;
	size_t crc_len;
    size_t frame_len;
	uint8_t* pbuf = (uint8_t*)buf;
	
	if((buf == NULL) || (head == NULL) || (buf_len == 0))
		return ERR_EMPTY;

    /* Check the crc. */
	crc_len = buf_len - L_START_LEN - L_HEADLEN_LEN - L_CRC_LEN;
    frame_len = *(uint16_t *)(pbuf + L_START_LEN);
	crc = cal_crc16(pbuf + L_START_LEN + L_HEADLEN_LEN, crc_len);
	if(*(uint16_t*)(pbuf + frame_len + 4)!= crc)
		return ERR_CRC;
    
	/* skip the length of start field. */
	pbuf += L_START_LEN;
    /* skip the length of framelen field. */
	pbuf += 2;
	head->frame_id  = *(uint8_t *)pbuf;
	pbuf += 1;
	head->ctrl.byte = *(uint8_t *)pbuf;
	pbuf += 1;
	head->des_addr  = *(uint16_t *)pbuf;
	pbuf += 2;
	head->src_addr  = *(uint16_t *)pbuf;
	pbuf += 2;
	pay->payload_len  = buf_len - L_CRC_LEN - (pbuf - buf);
	pay->payload = pbuf;
    
	return PRO_SUCCESS;
}
/**
 * Action: Unpack the L-frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buf which contain the L-frame.
 *         @buf_len : The size of the L-frame.
 * Output: @frame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @PRO_SUCCESS : Successful unpack the L-frame.
 *         @ERR_EMPTY : the buf/frame is NULL.
 *         @ERR_CRC : the crc of L-frame is error.
 *         @ERR_HEADLEN : the size behind start field is less than len field size.
 *         @ERR_NOSTART : can't find the start field.
 */
eStatusCode L_UnPack(LFrame_t *frame, const uint8_t *buf, size_t buf_len)
{
	uint16_t crc;
	size_t crc_len;
    size_t frame_len;
    size_t payload_len;
    eStatusCode ret;
	uint8_t* pbuf = (uint8_t*)buf;
    
	if((buf == NULL) || (frame == NULL) || (buf_len == 0))
		return ERR_EMPTY;

    /* Check the crc. */
	crc_len = buf_len - L_START_LEN - L_HEADLEN_LEN - L_CRC_LEN;
    frame_len = *(uint16_t *)(pbuf + L_START_LEN);
	crc = cal_crc16(pbuf + L_START_LEN + L_HEADLEN_LEN, crc_len);
	if(*(uint16_t*)(pbuf + frame_len + 4)!= crc)
		return ERR_CRC;

    /* skip the length of start field. */
	pbuf += L_START_LEN;
    /* skip the length of framelen field. */
	pbuf += 2;
	frame->head.frame_id  = *(uint8_t *)pbuf;
	pbuf += 1;
	frame->head.ctrl.byte = *(uint8_t *)pbuf;
    if(frame->head.ctrl.bit.pro_type != PRO_L)
        return ERR_PRO_L;
	pbuf += 1;
	frame->head.des_addr  = *(uint16_t *)pbuf;
	pbuf += 2;
	frame->head.src_addr  = *(uint16_t *)pbuf;
	pbuf += 2;

    payload_len = buf_len - L_CRC_LEN - (pbuf - buf);
	if(payload_len > 0)
	{
        frame->payload_flg = 1;
        ret = Payload_Unpack(&frame->payload, pbuf, payload_len);
		if(PRO_SUCCESS != ret)
		    return ret;
	}
	else
	{
		frame->payload_flg = 0;
	}

	return PRO_SUCCESS;
}
/**
 * Action: pack the L-frame based on TH-NET(v0.6.0) protocol document.
 * Input : @frame : The frame which contain the L-frame each field.
 * Output: @buffer : The buffer where store the L-frame.
 *         @buffer_len : the size of buffer.
 * Return: This return value can be one of the enum values:
 *         @L_SUCCESS : Successful pack the L-frame.
 *         @ERR_EMPTY : the frame/buffer is NULL.
 */
eStatusCode L_PackHead(uint8_t *buf, size_t *buf_len, const LHead_t* head, const PayBuf_t* pay)
{
	size_t frame_len;
	uint8_t* pbuf = buf;
	
	if((head == NULL) || (buf == NULL))
		return ERR_EMPTY;
		
	*(uint16_t *)pbuf = (uint16_t)L_START;

	/* skip the length of start field and len field. */
	pbuf += 4;
	*(uint8_t *)pbuf = head->frame_id;
	/* skip the length of frameid field. */
	pbuf += 1;
	*(uint8_t *)pbuf = head->ctrl.byte;
	/* skip the length of ctrl field. */
	pbuf += 1;
	*(uint16_t *)pbuf = head->des_addr;
	/* skip the length of des_addr field. */
	pbuf += 2;	
	*(uint16_t *)pbuf= head->src_addr;
	/* skip the length of src_addr field. */
	pbuf += 2;

    if(pay->payload_len > 0)
    {
        if(NULL == pay->payload)
			return ERR_EMPTY;
		
	    memcpy(pbuf, pay->payload, pay->payload_len);
		pbuf += pay->payload_len;
	}
	frame_len = pbuf - buf - 4;
	*(uint16_t *)(buf + 2) = (uint16_t)frame_len;

    /* Calculate and pack the crc. */
	*(uint16_t *)pbuf = cal_crc16(buf + 4, frame_len);
	pbuf += 2;

	*buf_len = pbuf - buf;
	
	return PRO_SUCCESS;
}
/**
 * Action: pack the L-frame based on TH-NET(v0.6.0) protocol document.
 * Input : @frame : The frame which contain the L-frame each field.
 * Output: @buf : The buf where store the L-frame.
 *         @buf_len : the size of buf.
 * Return: This return value can be one of the enum values:
 *         @L_SUCCESS : Successful pack the L-frame.
 *         @ERR_EMPTY : the frame/buf is NULL.
 */
eStatusCode L_Pack(uint8_t* buf, size_t *buf_len, const LFrame_t* frame)
{
	size_t frame_len;
    size_t payload_len;
	uint8_t* pbuf = buf;
    eStatusCode ret;
	
	if((frame == NULL) || (buf == NULL))
		return ERR_EMPTY;
		
	*(uint16_t *)pbuf = (uint16_t)L_START;
	/* skip the length of start field and len field. */
	pbuf += 4;
	
	*(uint8_t *)pbuf = frame->head.frame_id;
	/* skip the length of frameid field. */
	pbuf += 1;
	
	*(uint8_t *)pbuf = frame->head.ctrl.byte;
	/* skip the length of ctrl field. */
	pbuf += 1;
	
	*(uint16_t *)pbuf = frame->head.des_addr;
	/* skip the length of des_addr field. */
	pbuf += 2;	
	
	*(uint16_t *)pbuf= frame->head.src_addr;
	/* skip the length of src_addr field. */
	pbuf += 2;

	if(1 == frame->payload_flg){
        ret = Payload_Pack(pbuf, &payload_len, &frame->payload);
		if(PRO_SUCCESS != ret){
            return ret;
        }else{
            pbuf += payload_len;
        }
	}else{
		payload_len = 0;
	}
    /* Calculate the framelen field value. */
    /* '6' = L_HEADFRAMEID_LEN + L_HEADFCTRL_LEN + L_HEADDNWKADDR_LEN + L_HEADSNWKADDR_LEN */
	frame_len = payload_len + 6;
    /* Pack the framelen field. */
	*(uint16_t *)(buf + 2) = frame_len;
    /* Calculate and pack the crc. */
	*(uint16_t *)pbuf = cal_crc16(buf + 4, frame_len);
    /* Pack the whole LFrame length. */
	*buf_len = L_START_LEN + L_HEAD_LEN + payload_len + L_CRC_LEN;
	
	return PRO_SUCCESS;
}

/**
 * Action: Unpack the payload field based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame.
 *         @buf_len : The size of the payload input buffer.
 * Output: @payload : The structure where store the unpacked field.   
 * Return: This return value can be one of the enum values:
 *         @PRO_SUCCESS : Successful unpack the payload input buffer.
 *         @ERR_EMPTY : the buffer/payload is NULL.
 */
eStatusCode L_InitFrame(LFrame_t *frame)
{
	if(frame == NULL)
		return ERR_EMPTY;
	memset(frame, 0, sizeof(LFrame_t));
    frame->head.ctrl.bit.pro_type = PRO_L;

    return PRO_SUCCESS;
}

/**
 * Action: Unpack the payload field based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame.
 *         @buf_len : The size of the payload input buffer.
 * Output: @payload : The structure where store the unpacked field.   
 * Return: This return value can be one of the enum values:
 *         @PRO_SUCCESS : Successful unpack the payload input buffer.
 *         @ERR_EMPTY : the buffer/payload is NULL.
 */
 eStatusCode L_FrameCheck(uint8_t* frame, size_t frame_len)
 {
	size_t len;
    uint16_t crc;
	
	if(frame == NULL)
		return ERR_EMPTY;
    
    /* Check the frame length. */
	len = *(uint16_t*)(frame + L_START_LEN);
	if(len + L_START_LEN + L_HEADLEN_LEN + L_CRC_LEN != frame_len)
		return ERR_BUFLEN;

    /* Check the crc. */
	crc = cal_crc16(frame + L_START_LEN + L_HEADLEN_LEN, len);
	if(*(uint16_t*)(frame + L_START_LEN + L_HEADLEN_LEN + len)!= crc)
		return ERR_CRC;
		
	return PRO_SUCCESS;
 }
 
 
/** - End - Wumeng Tech. Co., Ltd. 2015 *********************************************************/ 

