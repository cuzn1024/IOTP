/** - Start - Copyright (C), 2015-2025, Wumeng Tech. Co., Ltd. 2015 *****************************
 *
 *  File Name: crc.h
 *  Function:  
 *  Modify: 
 *  <Date>          <Author>       <version>        <Desc>
 * 2017-06-01                       V01.00   
 **************************************************************************************************/
#ifndef __CRC_H
#define __CRC_H

/** Include ***************************************************************************************/
#include <stdint.h>
#include <stddef.h>
/** Include ***************************************************************************************/
/** Macro define **********************************************************************************/
/** Type define ***********************************************************************************/
/** Constant **************************************************************************************/
/** Variable **************************************************************************************/
extern uint16_t crc16_update_table(uint16_t crc, const uint8_t *pData, size_t lenth);
extern uint32_t crc32_update_table(uint32_t crc, const uint8_t *pData, size_t lenth);

#define cal_crc16(x,y)    crc16_update_table(0xFFFF,x,y)
#define cal_crc32(x,y)    crc32_update_table(0xFFFF,x,y)


#endif	/* __CRC_H */

/** - End - Wumeng Tech. Co., Ltd. 2015 *********************************************************/ 

