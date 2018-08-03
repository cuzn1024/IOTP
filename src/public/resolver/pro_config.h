/** - Start - Copyright (C), 2015-2025, Wumeng Tech. Co., Ltd. 2015 *****************************
 *
 *  File Name: pro_config.h
 *  Function:  
 *  Modify: 
 *  <Date>          <Author>       <version>        <Desc>
 * 2017-06-01                       V01.00   
 **************************************************************************************************/ 
#ifndef __PRO_CONFIG_H
#define __PRO_CONFIG_H

/** Include ***************************************************************************************/
#include <stdint.h>
/** Macro define **********************************************************************************/
#define  PRO_M             (0x00>>5)
#define  PRO_R             (0x20>>5)
#define  PRO_C             (0x40>>5)
#define  PRO_L             (0x60>>5)

#define  DIR_UP             0x00  /* "UP" means from END to AP    */
#define  DIR_DOWN           0x01  /* "DOWN" means from AP to END */

#define  FRAMELEN_LEN          1
#define  FCTRL_LEN             1
#define  DNWKADDR_LEN          2
#define  SNWKADDR_LEN          2
#define  CRC_LEN               2
#define  GROUPID_LEN           2
#define  RDT_LEN               1

#define  L_START1              0x7e
#define  L_START2              0x7e
#define  L_START             ((L_START1)|(L_START2<<8))

#define  L_START_LEN           2
#define  L_CRC_LEN             2
#define  L_HEADLEN_LEN         2
#define  L_HEADFRAMEID_LEN     1
#define  L_HEADFCTRL_LEN       1
#define  L_HEADDNWKADDR_LEN    2
#define  L_HEADSNWKADDR_LEN    2
#define  L_HEAD_LEN           (L_HEADLEN_LEN+L_HEADFRAMEID_LEN+L_HEADFCTRL_LEN+\
							   L_HEADDNWKADDR_LEN+L_HEADSNWKADDR_LEN)

#define  P_PCTRL_LEN           1
#define  P_P2PDATAID_LEN       1
#define  P_DATAID_LEN          1

/* The real data length should less than a number, pelease check TH-NET(v0.6.0) document. */
#define  MAX_DATA_LEN       240
/* The p2p data length should not larger than 15, pelease check TH-NET(v0.6.0) document. */
#define  MAX_P2P_DATA_LEN   15

/** Type define ***********************************************************************************/
typedef enum {
	PRO_SUCCESS,
	ERR_CRC,
	ERR_NOSTART,            /* can not find the start.  */
	ERR_EMPTY,
	ERR_BUFLEN,
	ERR_HEADLEN,
	ERR_TYPE,               /* unknow protocol type.     */
	ERR_PRO_L,              /* this is not protocol l.   */
	ERR_MAX_DATA_LEN,       /* data length is too large. */
	ERR_MAX_P2P_DATA_LEN    /* p2p data length is too large. */
}eStatusCode;

typedef enum {
	M2E = (DIR_DOWN|(PRO_M<<1)),
	E2M = (DIR_UP  |(PRO_M<<1)),
	R2E = (DIR_DOWN|(PRO_R<<1)),
	E2R = (DIR_UP  |(PRO_R<<1)),
	C2E = (DIR_DOWN|(PRO_C<<1)),
	E2C = (DIR_UP  |(PRO_C<<1))
}ProType;

/** Constant **************************************************************************************/
/** Variable **************************************************************************************/
/** Private Function ******************************************************************************/
/** Public  Function ******************************************************************************/

#endif /* __PRO_CONFIG_H */

/** - End - Wumeng Tech. Co., Ltd. 2015 *********************************************************/

