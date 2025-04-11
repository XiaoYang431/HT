/*********************************************************************************************************//**
 * @file    BM32S2031_1_HT32/src/BM32S2031_1.h
 * @version V1.0.1
 * @date    2024-08-22
 * @brief   The header file of BM32S2031_1 driver.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __BM32S2031_1_H
#define __BM32S2031_1_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "i2c_master_config.h"
#include "ht32_board_config.h"
#include "bmduino_check.h"

#include "middleware/i2c_master.h"
#include "middleware/uart_module.h"


/* Settings ------------------------------------------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------------------------------------*/
typedef enum {
  BM32S2031_1_SUCCESS,
  BM32S2031_1_FAILURE
} BM32S2031_1_selStatus;
/* Exported constants --------------------------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------------------------------------*/

#define   BM32S2031_1_UART_TXTIMEOUT     100
#define   BM32S2031_1_BAUD  9600
#define   BM32S2031_1_R_SUCCESS         0
#define   BM32S2031_1_R_FAIL            1
#define   BM32S2031_1_CHECK_OK        0
#define   BM32S2031_1_CHECK_ERROR     1
#define   BM32S2031_1_TIMEOUT_ERROR   2

/*IR detection of reaction speed*/
#define   BM32S2031_1_SPEED_8_MS        0x00
#define   BM32S2031_1_SPEED_16_MS       0x01
#define   BM32S2031_1_SPEED_32_MS       0x02
#define   BM32S2031_1_SPEED_64_MS       0x03
#define   BM32S2031_1_SPEED_128_MS      0x04
#define   BM32S2031_1_SPEED_256_MS      0x05
#define   BM32S2031_1_SPEED_500_MS      0x06
#define   BM32S2031_1_SPEED_1_S         0x07
#define   BM32S2031_1_SPEED_FASTMODE    0x08


/* Exported variables --------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/
BM32S2031_1_selStatus BM32S2031_1_selSerial(uint32_t serial_number);
void BM32S2031_1_Init(void);
uint8_t BM32S2031_1_getIRStatus(void);//是否靠近
int16_t BM32S2031_1_readIRSignalValue(void);//获得查值
uint8_t BM32S2031_1_distanceLearning(void);
uint8_t BM32S2031_1_reset(void);
uint8_t BM32S2031_1_saveSettings(void);
uint8_t BM32S2031_1_restoreSettings(void);
int16_t BM32S2031_1_getFWVer(void);
    
uint8_t BM32S2031_1_getIRThreshold(void);
uint8_t BM32S2031_1_getIRDebounce(void); 
uint8_t BM32S2031_1_getIRResponseTime(void); 
uint8_t BM32S2031_1_getIROutputTime(void);  
uint8_t BM32S2031_1_getMode(void); 
uint8_t BM32S2031_1_getIRCurrent(void);
uint8_t BM32S2031_1_setIRThreshold(uint8_t threshold);
uint8_t BM32S2031_1_setIRDebounce(uint8_t debounce);
uint8_t BM32S2031_1_setIRResponseTime(uint8_t responseTime);
uint8_t BM32S2031_1_setIROutputTime(uint8_t outputTime);
uint8_t BM32S2031_1_setMode(uint8_t mode);
uint8_t BM32S2031_1_setIRCurrent(uint8_t current);

uint8_t  _BM32S2031_1_readIRRef(void);//获得参考值
uint8_t  _BM32S2031_1_readIREnv(void);//获得环境值
void _BM32S2031_1_writeBytes(uint8_t wbuf[], uint8_t wlen);
uint8_t _BM32S2031_1_readBytes(uint8_t rbuf[], uint8_t rlen, uint16_t timOut);
void _BM32S2031_1_delay(vu32 count);

#ifdef __cplusplus
}
#endif

#endif

