/** - Start - Copyright (C), 2015-2025, Wumeng Tech. Co., Ltd. 2015 *****************************
 *
 *  File Name: pro_location.c
 *  Function:  
 *  Modify: 
 *  <Date>          <Author>       <version>        <Desc>
 * 2017-06-01                       V01.00   
 **************************************************************************************************/ 
 
/** Include ***************************************************************************************/
#include "pro_location.h"

/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
void WL_LocationInit(location  *locate)
{
   	if(locate == NULL)
		return;

     locate->start_start = 0;
     locate->start_len   = 2;
    
     locate->timestamp_start = 2;
     locate->timestamp_len   = 4;
    
     locate->rssi_start = 6;
     locate->rssi_len   = 1;
    
     locate->snr_start = 7;
     locate->snr_len   = 1;
    
     locate->total_start = 8;
     locate->total_len   = 1;
    
/********************** wireless frame start ************************/ 
     locate->framelen_start = 9;
     locate->framelen_len   = 1;

     locate->ctrl_start = 10;
     locate->ctrl_len   = 1;

     locate->sdg_start = 11;
     locate->sdg_len   = 2;
    
     locate->rdt_start = 11;
     locate->rdt_len   = 1;
    /****** payload start ******/    
     locate->pctrl_start = 0;
     locate->pctrl_len   = 1;

     locate->p2pdataid_start = 0;
     locate->p2pdataid_len   = 1;

     locate->p2pdata_start = 0;
     locate->p2pdata_len   = 0;

     locate->divpack_start = 0;
     locate->divpack_len   = 2;

     locate->dataid_start = 0;
     locate->dataid_len   = 1;

     locate->data_start = 0;
     locate->data_len   = 0;
    /****** payload end ******/
    
     locate->crc_start = 0;
     locate->crc_len   = 2;
/********************** wireless frame end ************************/
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
eStatusCode WL_GetLocation(location  *locate, const WLFrame_t *frame)
{
    uint8_t offset = 11; /* offset start from wireless frame ctrl field */

	if((frame == NULL) || (locate == NULL))
		return ERR_EMPTY;
    
    switch(frame->wl_head.pro_type){
        case M2E:
        case E2M:
        case R2E:
        case E2R:
            offset = 13;
            break;
        case C2E:
            if(1 == frame->wl_head.head.c2e.ctrl.bit.rdt_mode){
                offset = 12;
            }
			break;
        case E2C:
            if(1 == frame->wl_head.head.e2c.ctrl.bit.rdt_mode){
                offset = 12;
            }
			break;
        default: return ERR_TYPE;
    }

    if(1 == frame->payload_flg){
        locate->pctrl_start = offset;
        offset++;
    }
    if(1 == frame->payload.p2p_flg){
        locate->p2pdataid_start = offset;
        offset++;
    }
    if(frame->payload.p2p.data_len > 0){
        locate->p2pdata_start = offset;
        locate->p2pdata_len = frame->payload.p2p.data_len;
        offset += frame->payload.p2p.data_len;
    }
    if(1 == frame->payload.ctrl.bit.div_flg){
        locate->divpack_start = offset;
        offset += 2;
    }
    if(1 == frame->payload.data_flg){
        locate->dataid_start = offset;
        offset++;
    }
    if(frame->payload.pay_data.data_len > 0){
        locate->data_start = offset;
        locate->data_len = frame->payload.pay_data.data_len;
        offset += frame->payload.pay_data.data_len;
    }
    
    locate->crc_start = offset;

    return PRO_SUCCESS;
}

/** - End - Wumeng Tech. Co., Ltd. 2015 *********************************************************/


