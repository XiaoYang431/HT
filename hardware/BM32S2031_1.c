/*********************************************************************************************************//**
 * @file    BM32S2031_1_HT32/src/BM32S2031_1.c
 * @version V1.0.1
 * @date    2024-08-22
 * @brief   The function of BM32S2031_1 driver.
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

/* Includes ------------------------------------------------------------------------------------------------*/
#include "BM32S2031_1.h"

/* Settings ------------------------------------------------------------------------------------------------*/
/* Private types -------------------------------------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------------------------------------*/
/* Global variables ----------------------------------------------------------------------------------------*/

uint32_t gBM32S2031_1_SERIAL = BM32S2031_1_SERIAL; // Default serial number
/* Private variables ---------------------------------------------------------------------------------------*/

/* Global functions ----------------------------------------------------------------------------------------*/

/*********************************************************************************************************//**
 * @brief  module serial number select.
 * @param  serial_number: select serial number.
 * @retval BM32S2031_1_selStatus          
           @arg BM32S2031_1_SUCCESS
           @arg BM32S2031_1_FAILURE
 ************************************************************************************************************/
BM32S2031_1_selStatus BM32S2031_1_selSerial(uint32_t serial_number)
{
  if(CHECK_SERIALn(serial_number) == -1)
  {
    return BM32S2031_1_FAILURE;
  }

  gBM32S2031_1_SERIAL = serial_number;
  return BM32S2031_1_SUCCESS;
}

/*********************************************************************************************************//**
 * @brief  Module initialization using UART communication.
 * @param  void
 * @retval void
 ************************************************************************************************************/
void BM32S2031_1_Init(void)
{
	{
	USART_InitTypeDef USART_InitStructure = {0};
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStructure.USART_StopBits = USART_STOPBITS_1;
  USART_InitStructure.USART_Parity = USART_PARITY_NO;
  USART_InitStructure.USART_Mode = USART_MODE_NORMAL;
  UARTM_Init(gBM32S2031_1_SERIAL, &USART_InitStructure, BM32S2031_1_UART_TXTIMEOUT);
  }
	BM32S2031_1_getFWVer();
}

/*********************************************************************************************************//**
 * @brief  Get IR Induction state
 * @param  void
 * @retval BM32S2031_1_irStatus: 
            @arg 1:approach   
            @arg 0:not approach   
 ************************************************************************************************************/
uint8_t BM32S2031_1_getIRStatus(void)
{
    uint8_t BM32S2031_1_sendBuf[5] = {0x55, 0x80, 0x02, 0x01,0xD8};
    uint8_t BM32S2031_1_irStatus = 0;
    uint8_t BM32S2031_1_buff[6] = {0};
    _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,5);
    if(_BM32S2031_1_readBytes(BM32S2031_1_buff,6,20)== BM32S2031_1_CHECK_OK)
    {
     BM32S2031_1_irStatus = BM32S2031_1_buff[4];
    }
    _BM32S2031_1_delay(15);
    return BM32S2031_1_irStatus;
}

/*********************************************************************************************************//**
 * @brief  Read  signal value
 * @param  void
 * @retval BM32S2031_1_signalValue:Signal Value
 ************************************************************************************************************/
int16_t BM32S2031_1_readIRSignalValue(void)
{
    int16_t BM32S2031_1_signalValue = 0;
    uint8_t BM32S2031_1_irRef = 0;
    uint8_t BM32S2031_1_irEnv = 0;
    BM32S2031_1_irRef = _BM32S2031_1_readIRRef();
    BM32S2031_1_irEnv = _BM32S2031_1_readIREnv();
    BM32S2031_1_signalValue = BM32S2031_1_irEnv - BM32S2031_1_irRef;
    return BM32S2031_1_signalValue;
}

/*********************************************************************************************************//**
 * @brief  Enter distance learning mode
 * @param  void
 * @retval Distance learning situation: 
           @arg 0(BM32S2031_1_R_SUCCESS):distance Learn Success 
           @arg 1(BM32S2031_1_R_FAIL):distance Learn Fail
 * @Others Place the obstacle above BM32S2031_1 at the distance you want to learn. 
           The learning is  complete when the OUT pin level of BM32S2031_1 changes. 
 ************************************************************************************************************/
uint8_t BM32S2031_1_distanceLearning(void)
{
    uint8_t BM32S2031_1_sendBuf[3] = {0x55, 0x19, 0x6E};
    uint8_t BM32S2031_1_buff[3] = {0};
    _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,3); 
    if(_BM32S2031_1_readBytes(BM32S2031_1_buff,3,20)== BM32S2031_1_CHECK_OK)
    {
      if(BM32S2031_1_buff[1]== 0x7F)
      {
				if(_BM32S2031_1_readBytes(BM32S2031_1_buff,3,3500)== BM32S2031_1_CHECK_OK)
       {
        if(BM32S2031_1_buff[1]== 0x7D)
        {
         _BM32S2031_1_delay(15);
          return BM32S2031_1_R_SUCCESS ;
        }
       _BM32S2031_1_delay(15);
         return BM32S2031_1_R_FAIL ;
       }
      }
    }
     _BM32S2031_1_delay(15);
  return BM32S2031_1_R_FAIL ;
}

/*********************************************************************************************************//**
 * @brief  Module reset
 * @param  void
 * @retval Implementation status: 
              @arg 0(BM32S2031_1_R_SUCCESS):Success 
              @arg 1(BM32S2031_1_R_FAIL):Fail
 ************************************************************************************************************/
uint8_t BM32S2031_1_reset(void)
{
    uint8_t BM32S2031_1_sendBuf[3] = {0x55, 0x10, 0x65};
    uint8_t BM32S2031_1_buff[3] = {0};
     _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,3); 
    if( _BM32S2031_1_readBytes(BM32S2031_1_buff,3,20)== BM32S2031_1_CHECK_OK)
    {
     if(BM32S2031_1_buff[1]== 0x7f)
     {
      _BM32S2031_1_delay(100);
       return BM32S2031_1_R_SUCCESS;
     }
    }
      _BM32S2031_1_delay(100);
    return BM32S2031_1_R_FAIL ;
}

/*********************************************************************************************************//**
 * @brief  Save function parameters to EEPROM
 * @param  void
 * @retval Implementation status: 
              @arg 0(BM32S2031_1_R_SUCCESS):Success 
              @arg 1(BM32S2031_1_R_FAIL):Fail
 * @Others Writes the current parameter to EEPROM
 ************************************************************************************************************/
uint8_t BM32S2031_1_saveSettings(void)
{
  uint8_t BM32S2031_1_sendBuf[3] = {0x55, 0x1A, 0x6F}; 
  uint8_t BM32S2031_1_buff[3] = {0};
  _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,3);
  if(_BM32S2031_1_readBytes(BM32S2031_1_buff,5,50)== BM32S2031_1_CHECK_OK)
   {
    if(BM32S2031_1_buff[1]== 0x7f)
    {
     _BM32S2031_1_delay(15);
     return BM32S2031_1_R_SUCCESS;
    }
   }
   	 _BM32S2031_1_delay(15);
   return BM32S2031_1_R_FAIL ;
}

/*********************************************************************************************************//**
 * @brief  Assigns values to function parameters by reading values from EEPROM
 * @param  void
 * @retval Implementation status: 
              @arg 0(BM32S2031_1_R_SUCCESS):Success 
              @arg 1(BM32S2031_1_R_FAIL):Fail
 * @Others Read function parameters from EEPROM
 ************************************************************************************************************/
uint8_t BM32S2031_1_restoreSettings(void)
{
   uint8_t BM32S2031_1_sendBuf[3] = {0x55, 0x1C, 0x71}; 
   uint8_t BM32S2031_1_buff[3] = {0};
   _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,3);
   if( _BM32S2031_1_readBytes(BM32S2031_1_buff,3,20)== BM32S2031_1_CHECK_OK)
   {
    if(BM32S2031_1_buff[1]== 0x7f)
    {
     _BM32S2031_1_delay(15);
     return BM32S2031_1_R_SUCCESS;
    }
   }
    _BM32S2031_1_delay(15);
   return BM32S2031_1_R_FAIL ; 
}

/*********************************************************************************************************//**
 * @brief  Get the version information
 * @param  void
 * @retval BM32S2031_1_ver: FWVer
 * @Others EX.   //High byte     LOW byte
                 //0b00000 0001  0000 0000(version 1.0)
 ************************************************************************************************************/
int16_t BM32S2031_1_getFWVer(void)
{
    uint8_t BM32S2031_1_sendBuf1[5] = {0x55, 0x80, 0x00, 0x01, 0xD6};
    uint8_t BM32S2031_1_sendBuf2[5] = {0x55, 0x80, 0x01, 0x01, 0xD7};
    uint8_t BM32S2031_1_buff[6] = {0};
    int16_t BM32S2031_1_ver;
    uint8_t BM32S2031_1_ver_l = 0;
    uint8_t BM32S2031_1_ver_h = 0;
    _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf1,5);
    if(_BM32S2031_1_readBytes(BM32S2031_1_buff,6,20)== BM32S2031_1_CHECK_OK)
    {
     BM32S2031_1_ver_l = BM32S2031_1_buff[4];
    }
    _BM32S2031_1_delay(15);
    _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf2,5);
    if(_BM32S2031_1_readBytes(BM32S2031_1_buff,6,20)== BM32S2031_1_CHECK_OK)
    {
     BM32S2031_1_ver_h = BM32S2031_1_buff[4];
    }
    BM32S2031_1_ver = (BM32S2031_1_ver_h<<8)+BM32S2031_1_ver_l ;
    _BM32S2031_1_delay(15);
    return BM32S2031_1_ver;
}

/*********************************************************************************************************//**
 * @brief  Get IR trigger threshold
 * @param  void
 * @retval BM32S2031_1_threshold: Trigger threshold(16~180)
 ************************************************************************************************************/
uint8_t BM32S2031_1_getIRThreshold(void)
{
	  uint8_t BM32S2031_1_sendBuf[5] = {0x55, 0x80, 0x08, 0x01,0xDE};
    uint8_t BM32S2031_1_threshold = 0;
    uint8_t BM32S2031_1_buff[6] = {0};
    _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,5);
    if(_BM32S2031_1_readBytes(BM32S2031_1_buff,6,20)== BM32S2031_1_CHECK_OK)
    {
     BM32S2031_1_threshold = BM32S2031_1_buff[4];
    }
    _BM32S2031_1_delay(15);
    return BM32S2031_1_threshold;
}

/*********************************************************************************************************//**
 * @brief  Get IR debounce value
 * @param  void
 * @retval BM32S2031_1_debounce: Number of debounces(0~15)
 ************************************************************************************************************/
uint8_t BM32S2031_1_getIRDebounce(void)
{
  uint8_t BM32S2031_1_sendBuf[5] = {0x55, 0x80, 0x09, 0x01,0xDF};
  uint8_t BM32S2031_1_debounce = 0;
  uint8_t BM32S2031_1_buff[6] = {0};
  _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,5);
  if(_BM32S2031_1_readBytes(BM32S2031_1_buff,6,20)== BM32S2031_1_CHECK_OK)
   {
    BM32S2031_1_debounce = BM32S2031_1_buff[4];
   }
   _BM32S2031_1_delay(15);
  return BM32S2031_1_debounce;
}

/*********************************************************************************************************//**
 * @brief  Get IR response time 
 * @param  void
 * @retval BM32S2031_1_responseTime: 
                        @arg 0x00(BM32S2031_1_SPEED_8_MS):8MS       
                        @arg 0x01(BM32S2031_1_SPEED_16_MS):16MS      
                        @arg 0x02(BM32S2031_1_SPEED_32_MS):32MS       
                        @arg 0x03(BM32S2031_1_SPEED_64_MS):64MS      
                        @arg 0x04(BM32S2031_1_SPEED_128_MS):128MS      
                        @arg 0x05(BM32S2031_1_SPEED_256_MS):256MS      
                        @arg 0x06(BM32S2031_1_SPEED_500_MS):500MS
                        @arg 0x07(BM32S2031_1_SPEED_1_S ):1S  
                        @arg 0x08(BM32S2031_1_SPEED_FASTMODE):FASTMODE    
 ************************************************************************************************************/
uint8_t BM32S2031_1_getIRResponseTime(void)
{
    uint8_t BM32S2031_1_sendBuf[5] = {0x55, 0x80, 0x0A, 0x01,0xE0};
    uint8_t BM32S2031_1_responseTime = 0;
    uint8_t BM32S2031_1_buff[6] = {0};
    _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,5);
    if(_BM32S2031_1_readBytes(BM32S2031_1_buff,6,20)== BM32S2031_1_CHECK_OK)
    {
     BM32S2031_1_responseTime = BM32S2031_1_buff[4];
    }
    _BM32S2031_1_delay(15);
    return BM32S2031_1_responseTime;
}

/*********************************************************************************************************//**
 * @brief  Get IR output time
 * @param  void
 * @retval BM32S2031_1_outputTime: 
                       @arg 0x00: 0 seconds
                       @arg 0x01: 1 seconds
                       ...
                       @arg 0xFF: 255 seconds  
 * @Others  When object is detected, delay the output. (Only valid in general high and low level output mode)
 ************************************************************************************************************/
uint8_t BM32S2031_1_getIROutputTime(void)
{
    uint8_t BM32S2031_1_sendBuf[5] = {0x55, 0x80, 0x0B, 0x01,0xE1};
    uint8_t BM32S2031_1_outputTime;
    uint8_t BM32S2031_1_buff[6] = {0};
    _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,5);

    if(_BM32S2031_1_readBytes(BM32S2031_1_buff,6,20)== BM32S2031_1_CHECK_OK)
    {
     BM32S2031_1_outputTime = BM32S2031_1_buff[4];
    }
    _BM32S2031_1_delay(15);
    return BM32S2031_1_outputTime;
}

/*********************************************************************************************************//**
 * @brief  Get Mode function setting
 * @param  void
 * @retval BM32S2031_1_mode: 
                 @arg Bit4~0 Setting is invalid
                 @arg Bit5: Output mode selection 
                             0:High/Low(High and low level output mode)
                             1:Pulse/Toggle(Pulse switching mode) 
                 @arg Bit6: Pulse/Toggle mode selection(only valid if Bit 5=1) 
                             0:Pulse mode
                             1:Toogle mode 
                 @arg Bit7: Output level setting
                             0:Normal high level. Active low level
                             1:Normal low level. Active high level     
* @Others  Pulse mode: When the object is approaching, the output is 200us every 1 second
           Toogle mode: Object close, open output; When the object moves away and approaches again, turn off the output
 ************************************************************************************************************/
uint8_t BM32S2031_1_getMode(void)
{
    uint8_t BM32S2031_1_sendBuf[5] = {0x55, 0x80, 0x0C, 0x01,0xE2};
    uint8_t BM32S2031_1_mode;
    uint8_t BM32S2031_1_buff[6] = {0};
    _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,5);
    if(_BM32S2031_1_readBytes(BM32S2031_1_buff,6,20)== BM32S2031_1_CHECK_OK)
    {
     BM32S2031_1_mode = BM32S2031_1_buff[4];
    }
    _BM32S2031_1_delay(15);
    return BM32S2031_1_mode;
}

/*********************************************************************************************************//**
 * @brief  Get IR emission current value 
 * @param  void
 * @retval BM32S2031_1_current: Emission current parameters
              @arg current<= 63 (first order amplification), Emission current = current x 5 + 5 mA
              @arg current>= 63 (two-stage amplification), Emission current = (current - 64) x 5 + 5 mA  
 ************************************************************************************************************/
uint8_t BM32S2031_1_getIRCurrent(void)
{
  uint8_t BM32S2031_1_sendBuf[5] = {0x55, 0x80, 0x0D, 0x01,0xE3};
  uint8_t BM32S2031_1_current = 0;
  uint8_t BM32S2031_1_buff[6] = {0};
  _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,5);
  if(_BM32S2031_1_readBytes(BM32S2031_1_buff,6,20)== BM32S2031_1_CHECK_OK)
   {
    BM32S2031_1_current = BM32S2031_1_buff[4];
   }
  _BM32S2031_1_delay(15);
  return BM32S2031_1_current;
}

/*********************************************************************************************************//**
 * @brief  Set IR trigger threshold 
 * @param  threshold: Trigger threshold, parameter range:16~180(Default 16)  
 * @retval Implementation status: 
              @arg 0(BM32S2031_1_R_SUCCESS):Success 
              @arg 1(BM32S2031_1_R_FAIL):Fail
 ************************************************************************************************************/
uint8_t BM32S2031_1_setIRThreshold(uint8_t threshold)
{
    uint8_t BM32S2031_1_sendBuf[6] = {0x55, 0xC0, 0x08, 0x01, 0x00, 0x00};
    uint8_t BM32S2031_1_buff[3] = {0};
    BM32S2031_1_sendBuf[4] = threshold;
    BM32S2031_1_sendBuf[5] = 30+threshold;
    _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,6); 
    if(_BM32S2031_1_readBytes(BM32S2031_1_buff,3,20)== BM32S2031_1_CHECK_OK)
    {
     if(BM32S2031_1_buff[1]== 0x7f)
     {
       _BM32S2031_1_delay(15);
       return BM32S2031_1_R_SUCCESS;
     }
    }
    _BM32S2031_1_delay(15);
    return BM32S2031_1_R_FAIL ;
}

/*********************************************************************************************************//**
 * @brief  Set IR trigger threshold 
 * @param  threshold: Trigger threshold, parameter range:0~8(Default 6)  
 * @retval Implementation status: 
              @arg 0(BM32S2031_1_R_SUCCESS):Success 
              @arg 1(BM32S2031_1_R_FAIL):Fail
 ************************************************************************************************************/
uint8_t BM32S2031_1_setIRDebounce(uint8_t debounce)
{
  uint8_t BM32S2031_1_sendBuf[6] = {0x55, 0xC0, 0x09, 0x01, 0x00, 0x00};
  uint8_t BM32S2031_1_buff[3] = {0};
  BM32S2031_1_sendBuf[4] = debounce;
  BM32S2031_1_sendBuf[5] = 31+debounce;
  _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,6);
  if(_BM32S2031_1_readBytes(BM32S2031_1_buff,3,20)== BM32S2031_1_CHECK_OK)
   {
    if(BM32S2031_1_buff[1]== 0x7f)
    {
     _BM32S2031_1_delay(15);
     return BM32S2031_1_R_SUCCESS;
    }
   }
   _BM32S2031_1_delay(15);
   return BM32S2031_1_R_FAIL ;
}

/*********************************************************************************************************//**
 * @brief  Set IR response time
 * @param   responseTime: response time, parameter range:0x00~0x08 (Default 0x06)  
              @arg 0x00(BM32S2031_1_SPEED_8_MS):8MS       
              @arg 0x01(BM32S2031_1_SPEED_16_MS):16MS      
              @arg 0x02(BM32S2031_1_SPEED_32_MS):32MS       
              @arg 0x03(BM32S2031_1_SPEED_64_MS):64MS      
              @arg 0x04(BM32S2031_1_SPEED_128_MS):128MS      
              @arg 0x05(BM32S2031_1_SPEED_256_MS):256MS      
              @arg 0x06(BM32S2031_1_SPEED_500_MS):500MS
              @arg 0x07(BM32S2031_1_SPEED_1_S ):1S  
              @arg 0x08(BM32S2031_1_SPEED_FASTMODE):FASTMODE   
 * @retval Implementation status: 
              @arg 0(BM32S2031_1_R_SUCCESS):Success 
              @arg 1(BM32S2031_1_R_FAIL):Fail
 ************************************************************************************************************/
uint8_t BM32S2031_1_setIRResponseTime(uint8_t responseTime)
{
  uint8_t BM32S2031_1_sendBuf[6] = {0x55, 0xC0, 0x0A, 0x01, 0x00, 0x00};
  uint8_t BM32S2031_1_buff[3] = {0};
  BM32S2031_1_sendBuf[4] = responseTime;
  BM32S2031_1_sendBuf[5] = 32+responseTime;
  _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,6);
  if(_BM32S2031_1_readBytes(BM32S2031_1_buff,3,20)== BM32S2031_1_CHECK_OK)
  {
   if(BM32S2031_1_buff[1]== 0x7f)
   {
    _BM32S2031_1_delay(15);
     return BM32S2031_1_R_SUCCESS;
   }
  }
   _BM32S2031_1_delay(15);
  return BM32S2031_1_R_FAIL ;
}

/*********************************************************************************************************//**
 * @brief   Set IR output time
 * @param   outputTime: output time ,parameter range:0x00~0xFF (Default 0x00)  
              @arg 0X00:0 seconds       
              @arg 0x01:1 seconds     
              ***********
              @arg 0xff:255 seconds   
 * @retval Implementation status: 
              @arg 0(BM32S2031_1_R_SUCCESS):Success 
              @arg 1(BM32S2031_1_R_FAIL):Fail
* @Others   When object is detected, delay the output. (Only valid in general high and low level output mode)
 ************************************************************************************************************/
uint8_t BM32S2031_1_setIROutputTime(uint8_t outputTime)
{
  uint8_t BM32S2031_1_sendBuf[6] = {0x55, 0xC0, 0x0B, 0x01, 0x00, 0x00};
  uint8_t BM32S2031_1_buff[3] = {0};
  BM32S2031_1_sendBuf[4] = outputTime;
  BM32S2031_1_sendBuf[5] = 33+outputTime;
  _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,6);
 if( _BM32S2031_1_readBytes(BM32S2031_1_buff,3,20)== BM32S2031_1_CHECK_OK)
  {
   if(BM32S2031_1_buff[1]== 0x7f)
   {
   _BM32S2031_1_delay(15);
     return BM32S2031_1_R_SUCCESS;
   }
  }
  _BM32S2031_1_delay(15);
  return BM32S2031_1_R_FAIL ;
}

/*********************************************************************************************************//**
 * @brief   Set Mode function setting
 * @param   mode: parameter range:0b00000000~0b11111111 (Default 0b00010011)  
              @arg Bit4~0 Setting is invalid
              @arg Bit5: Output mode selection 
                             0:High/Low(High and low level output mode)
                             1:Pulse/Toggle(Pulse switching mode) 
              @arg Bit6: Pulse/Toggle mode selection(only valid if Bit 5=1) 
                             0:Pulse mode
                             1:Toogle mode 
              @arg Bit7: Output level setting
                             0:Normal high level. Active low level
                             1:Normal low level. Active high level     
 * @retval Implementation status: 
              @arg 0(BM32S2031_1_R_SUCCESS):Success 
              @arg 1(BM32S2031_1_R_FAIL):Fail
* @Others  Pulse mode: When the object is approaching, the output is 200us every 1 second
           Toogle mode: Object close, open output; When the object moves away and approaches again, turn off the output
 ************************************************************************************************************/
uint8_t BM32S2031_1_setMode(uint8_t mode)
{
  uint8_t BM32S2031_1_sendBuf[6] = {0x55, 0xC0, 0x0C, 0x01, 0x00, 0x00};
  uint8_t BM32S2031_1_buff[3] = {0};
  BM32S2031_1_sendBuf[4] = mode;
  BM32S2031_1_sendBuf[5] = 34+mode;
  _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,6);
 if(_BM32S2031_1_readBytes(BM32S2031_1_buff,3,20)== BM32S2031_1_CHECK_OK)
  {
   if(BM32S2031_1_buff[1]== 0x7f)
   {
     _BM32S2031_1_delay(15);
     return BM32S2031_1_R_SUCCESS;
   }
  }
    _BM32S2031_1_delay(15);
  return BM32S2031_1_R_FAIL ;
}

/*********************************************************************************************************//**
 * @brief   Set IR emission current value
 * @param   current: Emission current parameters, parameter range:0~127 (Default 78)  
              @arg current<= 63 (first order amplification), Emission current = current x 5 + 5 mA
              @arg current>= 63 (two-stage amplification), Emission current = (current - 64) x 5 + 5 mA   
 * @retval  Implementation status: 
              @arg 0(BM32S2031_1_R_SUCCESS):Success 
              @arg 1(BM32S2031_1_R_FAIL):Fail
* @Others  Pulse mode: When the object is approaching, the output is 200us every 1 second
           Toogle mode: Object close, open output; When the object moves away and approaches again, turn off the output
 ************************************************************************************************************/
uint8_t BM32S2031_1_setIRCurrent(uint8_t current)
{
  uint8_t BM32S2031_1_sendBuf[6] = {0x55, 0xC0, 0x0D, 0x01, 0x00, 0x00};
  uint8_t BM32S2031_1_buff[3] = {0};
  BM32S2031_1_sendBuf[4] = current;
  BM32S2031_1_sendBuf[5] = 35+current;
  _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,6);
  if(_BM32S2031_1_readBytes(BM32S2031_1_buff,3,20)== BM32S2031_1_CHECK_OK)
  {
   if(BM32S2031_1_buff[1]== 0x7f)
   {
     _BM32S2031_1_delay(15);
     return BM32S2031_1_R_SUCCESS;
   }
  }
   _BM32S2031_1_delay(15);
  return BM32S2031_1_R_FAIL ;
}

/*********************************************************************************************************//**
 * @brief   Get reference value 
 * @param   void
 * @retval  IrRef:Proximity sensing reference value. (Sensing value when IR LED is not on)  
 ************************************************************************************************************/
uint8_t  _BM32S2031_1_readIRRef(void)
{
  uint8_t BM32S2031_1_sendBuf[5] = {0x55, 0x80, 0x03, 0x01,0xD9};
  uint8_t BM32S2031_1_IrRef = 0;
  uint8_t BM32S2031_1_buff[6] = {0};
  _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,5);
  if(_BM32S2031_1_readBytes(BM32S2031_1_buff,6,20)== BM32S2031_1_CHECK_OK)
  {
   BM32S2031_1_IrRef = BM32S2031_1_buff[4];
  }
  _BM32S2031_1_delay(15);
  return BM32S2031_1_IrRef;
}

/*********************************************************************************************************//**
 * @brief   Get environmental value
 * @param  
 * @retval  IREnv:Proximity sensing environmental values. (Sensing value when IR LED is turned on ) 
 ************************************************************************************************************/
uint8_t  _BM32S2031_1_readIREnv(void)
{
  uint8_t BM32S2031_1_sendBuf[5] = {0x55, 0x80, 0x04, 0x01,0xDA};
  uint8_t BM32S2031_1_IREnv =  0;
  uint8_t BM32S2031_1_buff[6] = {0};
  _BM32S2031_1_writeBytes(BM32S2031_1_sendBuf,5);
  if( _BM32S2031_1_readBytes(BM32S2031_1_buff,6,20)== BM32S2031_1_CHECK_OK)
   {
    BM32S2031_1_IREnv = BM32S2031_1_buff[4];
   }
   _BM32S2031_1_delay(15);
  return BM32S2031_1_IREnv;
}

/*********************************************************************************************************//**
 * @brief  write data through UART.
 * @param  wbuf:Variables for storing Data to be sent
 * @param  wlen:Length of data sent
 * @retval void  
 ************************************************************************************************************/
void _BM32S2031_1_writeBytes(uint8_t wbuf[], uint8_t wlen)
{
	 UARTM_DiscardReadBuffer(gBM32S2031_1_SERIAL);
	 UARTM_Write(gBM32S2031_1_SERIAL,wbuf,wlen);
}

/*********************************************************************************************************//**
 * @brief  read data through UART.
 * @param  rbuf:Variables for storing Data to be obtained
 * @param  rlen:Length of data to be obtained
 * @param  timOut:The timeout between reading the first byte and each byte,Unit (ms)
 * @retval BM32S2031_1_TIMEOUT_ERROR/BM32S2031_1_CHECK_OK/BM32S2031_1_CHECK_ERROR
 ************************************************************************************************************/
uint8_t _BM32S2031_1_readBytes(uint8_t rbuf[], uint8_t rlen, uint16_t timOut)
{
	  uint8_t _lenCount = 0;
	  uint8_t _delayCount = 0;
	  uint8_t _BM32S2031_1_checkSum = 0;
		 for(_lenCount = 0; _lenCount < rlen; _lenCount++)
		 {
			 _delayCount = 0;
			 while(UARTM_GetReadBufferLength(gBM32S2031_1_SERIAL) == 0)
			 {
				 if(_delayCount > timOut)
				 {
					 return BM32S2031_1_TIMEOUT_ERROR;
				 }
				 
				 _BM32S2031_1_delay(1);
				 _delayCount++;
			 }
			 UARTM_ReadByte(gBM32S2031_1_SERIAL,rbuf+_lenCount);		 
		 }

	 //checkSum
  for (_lenCount = 0; _lenCount < (rlen - 1); _lenCount++)
 {
    _BM32S2031_1_checkSum += rbuf[_lenCount];
  }
  if (_BM32S2031_1_checkSum == rbuf[rlen - 1])
  {
    return BM32S2031_1_CHECK_OK; // Check correct
  }
  else
 {
   return BM32S2031_1_CHECK_ERROR; // Check error
  }
}


/*********************************************************************************************************//**
 * @brief  delay ms.
 * @param  count: Delay time,Unit(ms)
 * @retval void
 ************************************************************************************************************/
void _BM32S2031_1_delay(vu32 count)
{
  count = SystemCoreClock / 8000 * count;
  while(count--);
}

/* Private functions ---------------------------------------------------------------------------------------*/
