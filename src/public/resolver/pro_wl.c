/** - Start - Copyright (C), 2015-2025, Wumeng Tech. Co., Ltd. 2015 *****************************
 *
 *  File Name: pro_wl.c
 *  Function:  
 *  Modify: 
 *  <Date>          <Author>       <version>        <Desc>
 * 2017-06-01                        V01.00            
 **************************************************************************************************/ 

/** Include ***************************************************************************************/
#include "pro_wl.h"
/** Macro define **********************************************************************************/
/** Type define ***********************************************************************************/
/** Constant **************************************************************************************/
/** Variable **************************************************************************************/
/** Private Function ******************************************************************************/
/** Public  Function ******************************************************************************/

/*
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode WL_FrameCheck(const uint8_t *buf, size_t buf_len)
{
	uint8_t frame_len;
	uint16_t crc;
	
	if((buf == NULL) || (buf_len <= 0))
		return ERR_EMPTY;
	
	frame_len = *buf;
    if(buf_len != frame_len + 3)
        return ERR_BUFLEN;
    
	crc = cal_crc16(buf + 1, frame_len);
	if(*(uint16_t *)(buf + frame_len + 1) != crc)
		return ERR_CRC;
	
	return PRO_SUCCESS;
}

/**
 * Action: Get the protocol type and direction based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
ProType WL_GetTypeDir(const uint8_t *buf)
{
	uint8_t type_dir;
	
	type_dir = (*(buf + 1) & 0xf0) >> 4;

	return (ProType)type_dir;
}

/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer which contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
static void PayFlg_Set(PayBuf_t *pay, WLFrame_t *frame, const uint8_t *buf, size_t buf_len)
{
    switch(frame->wl_head.pro_type){
    case M2E:
    case E2M:
    case R2E:
    case E2R:
        if(buf_len - 6 > 0){
            frame->payload_flg = 1;
            pay->payload_len = buf_len - 6;
            pay->payload = (uint8_t *)(buf + 4);
        }else{
            frame->payload_flg = 0;
            pay->payload_len = 0;
            pay->payload = NULL;
        }
		break;
    case C2E:
        if(frame->wl_head.head.c2e.ctrl.bit.rdt_mode){
            if(buf_len - 5 > 0){
                frame->payload_flg = 1;
                pay->payload_len = buf_len - 5;
                pay->payload = (uint8_t *)(buf + 3);
            }else{
                frame->payload_flg = 0;
                pay->payload_len = 0;
                pay->payload = NULL;
            }
        }else{
            if(buf_len - 4 > 0){
                frame->payload_flg = 1;
                pay->payload_len = buf_len - 4;
                pay->payload = (uint8_t *)(buf + 2);
            }else{
                frame->payload_flg = 0;
                pay->payload_len = 0;
                pay->payload = NULL;
            }
        }
		break;
    case E2C:
        if(frame->wl_head.head.e2c.ctrl.bit.rdt_mode){
            if(buf_len - 5 > 0){
                frame->payload_flg = 1;
                pay->payload_len = buf_len - 5;
                pay->payload = (uint8_t *)(buf + 3);
            }else{
                frame->payload_flg = 0;
                pay->payload_len = 0;
                pay->payload = NULL;
            }
        }else{
            if(buf_len - 4 > 0){
                frame->payload_flg = 1;
                pay->payload_len = buf_len - 4;
                pay->payload = (uint8_t *)(buf + 2);
            }else{
                frame->payload_flg = 0;
                pay->payload_len = 0;
                pay->payload = NULL;
            }
        }
		break;
		default: break;
    }
}

/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode WL_FrameUnpackHead(PayBuf_t *pay, WLFrame_t *frame, const uint8_t *buf, size_t buf_len)
{
	eStatusCode ret;
	
	if((frame == NULL) || (buf == NULL))
		return ERR_EMPTY;
	
	ret = WL_FrameCheck(buf, buf_len);
	if(PRO_SUCCESS != ret)
        return ret;
	
	frame->wl_head.pro_type = WL_GetTypeDir(buf);
	switch(frame->wl_head.pro_type)
	{
		case M2E: M2E_UnpackHead(&frame->wl_head.head.m2e, buf); break;
		case E2M: E2M_UnpackHead(&frame->wl_head.head.e2m, buf); break;
		case R2E: R2E_UnpackHead(&frame->wl_head.head.r2e, buf); break;
		case E2R: E2R_UnpackHead(&frame->wl_head.head.e2r, buf); break;
		case C2E: C2E_UnpackHead(&frame->wl_head.head.c2e, buf); break;
		case E2C: E2C_UnpackHead(&frame->wl_head.head.e2c, buf); break;
        default: return ERR_TYPE;
	}
	PayFlg_Set(pay, frame, buf, buf_len);
    
	return PRO_SUCCESS;
}

/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode WL_FrameUnpack(WLFrame_t *frame, const uint8_t *buf, size_t buf_len)
{
    PayBuf_t pay;
	eStatusCode ret;

	if((frame == NULL) || (buf == NULL))
		return ERR_EMPTY;
    
	ret = WL_FrameUnpackHead(&pay, frame, buf, buf_len);
	if(PRO_SUCCESS != ret)
        return ret;

    if(1 == frame->payload_flg)
    {
        ret = Payload_Unpack(&frame->payload, pay.payload, pay.payload_len);
    	if(PRO_SUCCESS != ret)
            return ret;
    }
    
    return PRO_SUCCESS;
}

/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode M2E_Pack(uint8_t *buf, size_t *buf_len, const WLFrame_t *frame)
{
    uint8_t *pbuf = buf;
    size_t payload_len;
    uint8_t frame_len;
    eStatusCode ret;
    
    M2E_PackHead(buf, &frame->wl_head.head.m2e);
    pbuf += 4;
    if(1 == frame->payload_flg)
    {
        ret = Payload_Pack(pbuf, &payload_len, &frame->payload);
        if(PRO_SUCCESS != ret)
            return ret;
        /* skip the payload size. */
		pbuf += payload_len;
	}
    /* calculate the frame length. */
    frame_len = (uint8_t)(pbuf - buf - 1);
    /* pack the frame length. */
    *(uint8_t *)buf = frame_len;
    /* calculate and pack the crc. */
    *(uint16_t *)pbuf = cal_crc16(buf + 1, frame_len);
    /* skip the crc size. */
    pbuf += 2;
    /* calculate the out buffer size. */
    *buf_len = pbuf - buf; 

    return PRO_SUCCESS;
}

/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode E2M_Pack(uint8_t *buf, size_t *buf_len, const WLFrame_t *frame)
{
    uint8_t *pbuf = buf;
    size_t payload_len;
    uint8_t frame_len;
    eStatusCode ret;
    
    E2M_PackHead(buf, &frame->wl_head.head.e2m);
    pbuf += 4;
    if(1 == frame->payload_flg)
    {
        ret = Payload_Pack(pbuf, &payload_len, &frame->payload);
        if(PRO_SUCCESS != ret)
            return ret;
        /* skip the payload size. */
		pbuf += payload_len;
	}
    /* calculate the frame length. */
    frame_len = (uint8_t)(pbuf - buf - 1);
    /* pack the frame length. */
    *(uint8_t *)buf = frame_len;
    /* calculate and pack the crc. */
    *(uint16_t *)pbuf = cal_crc16(buf + 1, frame_len);
    /* skip the crc size. */
    pbuf += 2;
    /* calculate the out buffer size. */
    *buf_len = pbuf - buf; 
    
    return PRO_SUCCESS;
}
/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode R2E_Pack(uint8_t *buf, size_t *buf_len, const WLFrame_t *frame)
{
    uint8_t *pbuf = buf;
    size_t payload_len;
    uint8_t frame_len;
    eStatusCode ret;
    
    R2E_PackHead(buf, &frame->wl_head.head.r2e);
    pbuf += 4;
    if(1 == frame->payload_flg)
    {
        ret = Payload_Pack(pbuf, &payload_len, &frame->payload);
        if(PRO_SUCCESS != ret)
            return ret;
        /* skip the payload size. */
		pbuf += payload_len;
	}
    /* calculate the frame length. */
    frame_len = (uint8_t)(pbuf - buf - 1);
    /* pack the frame length. */
    *(uint8_t *)buf = frame_len;
    /* calculate and pack the crc. */
    *(uint16_t *)pbuf = cal_crc16(buf + 1, frame_len);
    /* skip the crc size. */
    pbuf += 2;
    /* calculate the out buffer size. */
    *buf_len = pbuf - buf; 

    return PRO_SUCCESS;
}

/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode E2R_Pack(uint8_t *buf, size_t *buf_len, const WLFrame_t *frame)
{
    uint8_t *pbuf = buf;
    size_t payload_len;
    uint8_t frame_len;
    eStatusCode ret;
    
    E2R_PackHead(buf, &frame->wl_head.head.e2r);
    pbuf += 4;
    if(1 == frame->payload_flg)
    {
        ret = Payload_Pack(pbuf, &payload_len, &frame->payload);
        if(PRO_SUCCESS != ret)
            return ret;
        /* skip the payload size. */
		pbuf += payload_len;
	}
    /* calculate the frame length. */
    frame_len = (uint8_t)(pbuf - buf - 1);
    /* pack the frame length. */
    *(uint8_t *)buf = frame_len;
    /* calculate and pack the crc. */
    *(uint16_t *)pbuf = cal_crc16(buf + 1, frame_len);
    /* skip the crc size. */
    pbuf += 2;
    /* calculate the out buffer size. */
    *buf_len = pbuf - buf; 

    return PRO_SUCCESS;
}

/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode C2E_Pack(uint8_t *buf, size_t *buf_len, const WLFrame_t *frame)
{
    uint8_t *pbuf = buf;
    size_t payload_len;
    uint8_t frame_len;
    eStatusCode ret;
    
    C2E_PackHead(pbuf, &frame->wl_head.head.c2e);
    pbuf += 2;
    if(frame->wl_head.head.c2e.ctrl.bit.rdt_mode)
    {
        *(uint8_t *)pbuf = frame->wl_head.head.c2e.rdt.byte;
        pbuf += 1;
    }
    if(1 == frame->payload_flg)
    {
        ret = Payload_Pack(pbuf, &payload_len, &frame->payload);
        if(PRO_SUCCESS != ret)
            return ret;
        /* skip the payload size. */
		pbuf += payload_len;
	}
    /* calculate the frame length. */
    frame_len = (uint8_t)(pbuf - buf - 1);
    /* pack the frame length. */
    *(uint8_t *)buf = frame_len;
    /* calculate and pack the crc. */
    *(uint16_t *)pbuf = cal_crc16(buf + 1, frame_len);
    /* skip the crc size. */
    pbuf += 2;
    /* calculate the out buffer size. */
    *buf_len = pbuf - buf;

    return PRO_SUCCESS;
}

/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode E2C_Pack(uint8_t *buf, size_t *buf_len, const WLFrame_t *frame)
{
    uint8_t *pbuf = buf;
    size_t payload_len;
    uint8_t frame_len;
    eStatusCode ret;
    
    E2C_PackHead(pbuf, &frame->wl_head.head.e2c);
    pbuf += 2;
    if(frame->wl_head.head.e2c.ctrl.bit.rdt_mode)
    {
        *(uint8_t *)pbuf = frame->wl_head.head.e2c.rdt.byte;
        pbuf += 1;
    }
    if(1 == frame->payload_flg)
    {
        ret = Payload_Pack(pbuf, &payload_len, &frame->payload);
        if(PRO_SUCCESS != ret)
            return ret;
        /* skip the payload size. */
		pbuf += payload_len;
	}
    /* calculate the frame length. */
    frame_len = (uint8_t)(pbuf - buf - 1);
    /* pack the frame length. */
    *(uint8_t *)pbuf = frame_len;
    /* calculate and pack the crc. */
    *(uint16_t *)pbuf = cal_crc16(buf + 1, frame_len);
    /* skip the crc size. */
    pbuf += 2;
    /* calculate the out buffer size. */
    *buf_len = pbuf - buf;

    return PRO_SUCCESS;
}
/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode WL_FramePack(uint8_t *buf, size_t *buf_len, const WLFrame_t *frame)
{
	if((buf == NULL) || (frame == NULL))
		return ERR_EMPTY; 
    
	switch(frame->wl_head.pro_type)
	{
		case M2E: return M2E_Pack(buf, buf_len, frame);
		case E2M: return E2M_Pack(buf, buf_len, frame);
		case R2E: return R2E_Pack(buf, buf_len, frame);
		case E2R: return E2R_Pack(buf, buf_len, frame);
		case C2E: return C2E_Pack(buf, buf_len, frame);
		case E2C: return E2C_Pack(buf, buf_len, frame);
		default:  return ERR_TYPE;
	}
}
/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode M2E_PackHeadWithPayLoad(uint8_t *buf, size_t *buf_len, const WLHead_t *wl_head, const PayBuf_t *pay_buf)
{
    uint8_t *pbuf = buf;
    uint8_t frame_len;
    
    M2E_PackHead(buf, &wl_head->head.m2e);
    pbuf += 4;
    if(pay_buf->payload_len > 0)
    {
        if(NULL == pay_buf->payload)
            return ERR_EMPTY;
		memcpy(pbuf, pay_buf->payload, pay_buf->payload_len);
        /* skip the payload size. */
		pbuf += pay_buf->payload_len;
	}
    /* calculate the frame length. */
    frame_len = (uint8_t)(pbuf - buf - 1);
    /* pack the frame length. */
    *(uint8_t *)buf = frame_len;
    /* calculate and pack the crc. */
    *(uint16_t *)pbuf = cal_crc16(buf + 1, frame_len);
    /* skip the crc size. */
    pbuf += 2;
    /* calculate the out buffer size. */
    *buf_len = pbuf - buf; 

    return PRO_SUCCESS;
}
/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode E2M_PackHeadWithPayLoad(uint8_t *buf, size_t *buf_len, const WLHead_t *wl_head, const PayBuf_t *pay_buf)
{
    uint8_t *pbuf = buf;
    uint8_t frame_len;
    
    E2M_PackHead(buf, &wl_head->head.e2m);
    pbuf += 4;
    if(pay_buf->payload_len > 0)
    {
        if(NULL == pay_buf->payload)
            return ERR_EMPTY;
		memcpy(pbuf, pay_buf->payload, pay_buf->payload_len);
        /* skip the payload size. */
		pbuf += pay_buf->payload_len;
	}
    /* calculate the frame length. */
    frame_len = (uint8_t)(pbuf - buf - 1);
    /* pack the frame length. */
    *(uint8_t *)buf = frame_len;
    /* calculate and pack the crc. */
    *(uint16_t *)pbuf = cal_crc16(buf + 1, frame_len);
    /* skip the crc size. */
    pbuf += 2;
    /* calculate the out buffer size. */
    *buf_len = pbuf - buf; 
    
    return PRO_SUCCESS;
}
/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode R2E_PackHeadWithPayLoad(uint8_t *buf, size_t *buf_len, const WLHead_t *wl_head, const PayBuf_t *pay_buf)
{
    uint8_t *pbuf = buf;
    uint8_t frame_len;
    
    R2E_PackHead(buf, &wl_head->head.r2e);
    pbuf += 4;
    if(pay_buf->payload_len > 0)
    {
        if(NULL == pay_buf->payload)
            return ERR_EMPTY;
		memcpy(pbuf, pay_buf->payload, pay_buf->payload_len);
        /* skip the payload size. */
		pbuf += pay_buf->payload_len;
	}
    /* calculate the frame length. */
    frame_len = (uint8_t)(pbuf - buf - 1);
    /* pack the frame length. */
    *(uint8_t *)buf = frame_len;
    /* calculate and pack the crc. */
    *(uint16_t *)pbuf = cal_crc16(buf + 1, frame_len);
    /* skip the crc size. */
    pbuf += 2;
    /* calculate the out buffer size. */
    *buf_len = pbuf - buf; 

    return PRO_SUCCESS;
}
/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode E2R_PackHeadWithPayLoad(uint8_t *buf, size_t *buf_len, const WLHead_t *wl_head, const PayBuf_t *pay_buf)
{
    uint8_t *pbuf = buf;
    uint8_t frame_len;
    
    E2R_PackHead(buf, &wl_head->head.e2r);
    pbuf += 4;
    if(pay_buf->payload_len > 0)
    {
        if(NULL == pay_buf->payload)
            return ERR_EMPTY;
		memcpy(pbuf, pay_buf->payload, pay_buf->payload_len);
        /* skip the payload size. */
		pbuf += pay_buf->payload_len;
	}
    /* calculate the frame length. */
    frame_len = (uint8_t)(pbuf - buf - 1);
    /* pack the frame length. */
    *(uint8_t *)buf = frame_len;
    /* calculate and pack the crc. */
    *(uint16_t *)pbuf = cal_crc16(buf + 1, frame_len);
    /* skip the crc size. */
    pbuf += 2;
    /* calculate the out buffer size. */
    *buf_len = pbuf - buf; 

    return PRO_SUCCESS;
}
/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode C2E_PackHeadWithPayLoad(uint8_t *buf, size_t *buf_len, const WLHead_t *wl_head, const PayBuf_t *pay_buf)
{
    uint8_t *pbuf = buf;
    uint8_t frame_len;
    
    C2E_PackHead(pbuf, &wl_head->head.c2e);
    pbuf += 2;
    if(wl_head->head.c2e.ctrl.bit.rdt_mode)
    {
        *(uint8_t *)pbuf = wl_head->head.c2e.rdt.byte;
        pbuf += 1;
    }
    if(pay_buf->payload_len > 0)
    {
        if(NULL == pay_buf->payload)
            return ERR_EMPTY;
		memcpy(pbuf, pay_buf->payload, pay_buf->payload_len);
        /* skip the payload size. */
		pbuf += pay_buf->payload_len;
	}
    /* calculate the frame length. */
    frame_len = (uint8_t)(pbuf - buf - 1);
    /* pack the frame length. */
    *(uint8_t *)buf = frame_len;
    /* calculate and pack the crc. */
    *(uint16_t *)pbuf = cal_crc16(buf + 1, frame_len);
    /* skip the crc size. */
    pbuf += 2;
    /* calculate the out buffer size. */
    *buf_len = pbuf - buf;

    return PRO_SUCCESS;
}
/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode E2C_PackHeadWithPayLoad(uint8_t *buf, size_t *buf_len, const WLHead_t *wl_head, const PayBuf_t *pay_buf)
{
    uint8_t *pbuf = buf;
    uint8_t frame_len;
    
    E2C_PackHead(pbuf, &wl_head->head.e2c);
    pbuf += 2;
    if(wl_head->head.e2c.ctrl.bit.rdt_mode)
    {
        *(uint8_t *)pbuf = wl_head->head.e2c.rdt.byte;
        pbuf += 1;
    }
    if(pay_buf->payload_len > 0)
    {
        if(NULL == pay_buf->payload)
            return ERR_EMPTY;
		memcpy(pbuf, pay_buf->payload, pay_buf->payload_len);
        /* skip the payload size. */
		pbuf += pay_buf->payload_len;
	}
    /* calculate the frame length. */
    frame_len = (uint8_t)(pbuf - buf - 1);
    /* pack the frame length. */
    *(uint8_t *)pbuf = frame_len;
    /* calculate and pack the crc. */
    *(uint16_t *)pbuf = cal_crc16(buf + 1, frame_len);
    /* skip the crc size. */
    pbuf += 2;
    /* calculate the out buffer size. */
    *buf_len = pbuf - buf;

    return PRO_SUCCESS;
}
/**
 * Action: Pack WL Frame head based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame.
 *         @buf_len : The size of the payload input buffer.
 * Output: @wl_head : The structure where store the head field.   
 * Return: This return value can be one of the enum values:
 *         @PRO_SUCCESS : Successful unpack the payload input buffer.
 *         @ERR_EMPTY   : the buffer/payload is NULL.
 *         @ERR_TYPE    :
 */
eStatusCode WL_FramePackHead(uint8_t *buf, size_t *buf_len, const WLHead_t *wl_head, const PayBuf_t *pay_buf)
{    
	if(buf == NULL)
		return ERR_EMPTY;
    
	switch(wl_head->pro_type)
	{
		case M2E: return M2E_PackHeadWithPayLoad(buf, buf_len, wl_head, pay_buf);
		case E2M: return E2M_PackHeadWithPayLoad(buf, buf_len, wl_head, pay_buf);
		case R2E: return R2E_PackHeadWithPayLoad(buf, buf_len, wl_head, pay_buf);
		case E2R: return E2R_PackHeadWithPayLoad(buf, buf_len, wl_head, pay_buf);
		case C2E: return C2E_PackHeadWithPayLoad(buf, buf_len, wl_head, pay_buf);
		case E2C: return E2C_PackHeadWithPayLoad(buf, buf_len, wl_head, pay_buf);
		default:  return ERR_TYPE;
	}
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
eStatusCode WL_PayloadUnpack(PayLoad_t *payload, const uint8_t *buf, size_t buf_len)
{
    size_t payload_len;
	eStatusCode ret;
	ProType pro_type;
	
	if((buf == NULL) || (payload == NULL))
		return ERR_EMPTY;

	ret = WL_FrameCheck(buf, buf_len);
	if(PRO_SUCCESS != ret)
		return ret;
	
	pro_type = WL_GetTypeDir(buf);
    if((C2E == pro_type) || (E2C == pro_type))
	{	
        buf += 2;
        payload_len = buf_len - 4;
        /* about rdt */
		if((*buf >> 3) & 0x01)
		{
			buf++;
            payload_len--;
		}
	}
	else
	{
		buf += 4;
        payload_len = buf_len - 6;
	}

    ret = Payload_Unpack(payload, buf, payload_len);
    if(PRO_SUCCESS != ret)
        return ret;
    
	return PRO_SUCCESS;
}

/**
 * Action: Init WLFrame based on TH-NET(v0.6.0) protocol document.
 * Input : 
 *         
 * Output:   
 * Return: This return value can be one of the enum values:
 *         @PRO_SUCCESS : Successful unpack the payload input buffer.
 *         @ERR_EMPTY : the buffer/payload is NULL.
 */
eStatusCode WL_InitFrame(ProType type, WLFrame_t *frame)
{
	if(frame == NULL)
		return ERR_EMPTY;
	memset(frame, 0, sizeof(WLFrame_t));

    switch(type){
        case M2E: frame->wl_head.head.m2e.ctrl.byte = type << 4; break;
        case E2M: frame->wl_head.head.e2m.ctrl.byte = type << 4; break;
        case R2E: frame->wl_head.head.r2e.ctrl.byte = type << 4; break;
        case E2R: frame->wl_head.head.e2r.ctrl.byte = type << 4; break;
        case C2E: frame->wl_head.head.c2e.ctrl.byte = type << 4; break;
        case E2C: frame->wl_head.head.e2c.ctrl.byte = type << 4; break;
        default:  return ERR_TYPE;
    }
    frame->wl_head.pro_type = type;
	frame->payload.data_flg = 1;
	frame->payload_flg = 1;
#if 0
    frame->payload_flg = 0;
    frame->payload.ctrl.bit.p2p_len = 0;
    frame->payload.ctrl.bit.div_flg = 0;
    frame->payload.ctrl.bit.security = 0;
    frame->payload.div.total= 0;
    frame->payload.div.cur = 0;
    frame->payload.p2p_flg = 0;
    frame->payload.p2p.data_id = 0;
    frame->payload.p2p.data = NULL;
    frame->payload.p2p.data_len = 0;
    frame->payload.data_flg = 0;
    frame->payload.pay_data.data_id = 0;
    frame->payload.pay_data.data = NULL;
    frame->payload.pay_data.data_len = 0;
#endif
    return PRO_SUCCESS;
}

/** - End - Wumeng Tech. Co., Ltd. 2015 *********************************************************/ 

