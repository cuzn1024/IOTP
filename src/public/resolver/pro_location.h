/** - Start - Copyright (C), 2015-2025, Wumeng Tech. Co., Ltd. 2015 *****************************
 *
 *  File Name: pro_location.h
 *  Function:  
 *  Modify: 
 *  <Date>          <Author>       <version>        <Desc>
 * 2017-06-01                       V01.00   
 **************************************************************************************************/ 
#ifndef _PRO_LOCATION_H_
#define _PRO_LOCATION_H_

/** Include ***************************************************************************************/
#include "pro_config.h"
#include "pro_wl.h"

typedef struct {
    uint8_t start_start;
    uint8_t start_len;
    
    uint8_t timestamp_start;
    uint8_t timestamp_len;
    
    uint8_t rssi_start;
    uint8_t rssi_len;
    
    uint8_t snr_start;
    uint8_t snr_len;
    
    uint8_t total_start;
    uint8_t total_len;
    
/********************** wireless frame start ************************/ 
    uint8_t framelen_start;
    uint8_t framelen_len;

    uint8_t ctrl_start;
    uint8_t ctrl_len;

    uint8_t sdg_start;
    uint8_t sdg_len;
    
    uint8_t rdt_start;
    uint8_t rdt_len;   
    /****** payload start ******/    
    uint8_t pctrl_start;
    uint8_t pctrl_len;

    uint8_t p2pdataid_start;
    uint8_t p2pdataid_len;

    uint8_t p2pdata_start;
    uint8_t p2pdata_len;

    uint8_t divpack_start;
    uint8_t divpack_len;

    uint8_t dataid_start;
    uint8_t dataid_len;

    uint8_t data_start;
    uint8_t data_len;
    /****** payload end ******/
    
    uint8_t crc_start;
    uint8_t crc_len;
/********************** wireless frame end ************************/
}location;

/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
void WL_LocationInit(location  *locate);

/**
 * Action: Unpack the M2E frame based on TH-NET(v0.6.0) protocol document.
 * Input : @buf : The buffer witch contain the frame data.
 *         @len : The length of the frame.
 * Output: @pM2EFrame : The structure where store the unpacked field.    
 * Return: This return value can be one of the enum values:
 *         @CRC_ERR : 
 *         @LEN_ERR : 
 */
eStatusCode WL_GetLocation(location  *locate, const WLFrame_t *frame);




#endif  /* _PRO_LOCATION_H_ */


/** - End - Wumeng Tech. Co., Ltd. 2015 *********************************************************/
