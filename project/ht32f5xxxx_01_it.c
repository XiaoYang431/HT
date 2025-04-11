/*********************************************************************************************************//**
 * @file    IP/Example/ht32f5xxxx_01_it.c
 * @version $Rev:: 4398         $
 * @date    $Date:: 2019-11-14 #$
 * @brief   This file provides all interrupt service routine.
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
#include "ht32.h"
#include "GPS.h"
#include "Motor.h"
int16_t   encder_left;
int16_t   encder_right; 
/** @addtogroup Project_Template Project Template
  * @{
  */

/** @addtogroup IP_Examples IP
  * @{
  */

/** @addtogroup Example
  * @{
  */


/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   This function handles NMI exception.
 * @retval  None
 ************************************************************************************************************/
void NMI_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Hard Fault exception.
 * @retval  None
 ************************************************************************************************************/
void HardFault_Handler(void)
{
  #if 1

  static vu32 gIsContinue = 0;
  /*--------------------------------------------------------------------------------------------------------*/
  /* For development FW, MCU run into the while loop when the hardfault occurred.                           */
  /* 1. Stack Checking                                                                                      */
  /*    When a hard fault exception occurs, MCU push following register into the stack (main or process     */
  /*    stack). Confirm R13(SP) value in the Register Window and typing it to the Memory Windows, you can   */
  /*    check following register, especially the PC value (indicate the last instruction before hard fault).*/
  /*    SP + 0x00    0x04    0x08    0x0C    0x10    0x14    0x18    0x1C                                   */
  /*           R0      R1      R2      R3     R12      LR      PC    xPSR                                   */
  while (gIsContinue == 0)
  {
  }
  /* 2. Step Out to Find the Clue                                                                           */
  /*    Change the variable "gIsContinue" to any other value than zero in a Local or Watch Window, then     */
  /*    step out the HardFault_Handler to reach the first instruction after the instruction which caused    */
  /*    the hard fault.                                                                                     */
  /*--------------------------------------------------------------------------------------------------------*/

  #else

  /*--------------------------------------------------------------------------------------------------------*/
  /* For production FW, you shall consider to reboot the system when hardfault occurred.                    */
  /*--------------------------------------------------------------------------------------------------------*/
  NVIC_SystemReset();

  #endif
}

/*********************************************************************************************************//**
 * @brief   This function handles SVCall exception.
 * @retval  None
 ************************************************************************************************************/
void SVC_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles PendSVC exception.
 * @retval  None
 ************************************************************************************************************/
void PendSV_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles SysTick Handler.
 * @retval  None
 ************************************************************************************************************/
/*void SysTick_Handler(void)
{
}
*/
/*********************************************************************************************************//**
 * @brief   This function handles LVD & BOD interrupt.
 * @retval  None
 ************************************************************************************************************/
//void LVD_BOD_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles RTC interrupt.
 * @retval  None
 ************************************************************************************************************/
//void RTC_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles Flash interrupt.
 * @retval  None
 ************************************************************************************************************/
//void FLASH_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles Wake-up interrupt.
 * @retval  None
 ************************************************************************************************************/
//void EVWUP_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles EXTI interrupt.
 * @retval  None
 ************************************************************************************************************/
//void EXTI0_1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles EXTI interrupt.
 * @retval  None
 ************************************************************************************************************/
//void EXTI2_3_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles EXTI interrupt.
 * @retval  None
 ************************************************************************************************************/
//void EXTI4_15_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles CMP interrupt.
 * @retval  None
 ************************************************************************************************************/
//void COMP_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles ADC interrupt.
 * @retval  None
 ************************************************************************************************************/
//void ADC_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles MCTM interrupt.
 * @retval  None
 ************************************************************************************************************/
//void MCTM0_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles GPTM interrupt.
 * @retval  None
 ************************************************************************************************************/
//void GPTM0_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles GPTM interrupt.
 * @retval  None
 ************************************************************************************************************/
//void GPTM1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles SCTM interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SCTM0_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles SCTM interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SCTM1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles SCTM interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SCTM2_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles SCTM interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SCTM3_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles SLED0 interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SLED0_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles SLED1 interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SLED1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles BFTM interrupt.
 * @retval  None
 ************************************************************************************************************/
void BFTM0_IRQHandler(void)
{
  if(BFTM_GetFlagStatus(HT_BFTM0) == SET)
  {
    BFTM_ClearFlag(HT_BFTM0);
     encder_left = HTCFG_CAP_PORT_decode1A->CNTR;//获取编码器计数值
     encder_right = HTCFG_CAP_PORT_decode2A->CNTR;//获取编码器计数值
     HTCFG_CAP_PORT_decode1A->CNTR = 0;//获取编码器计数值
     HTCFG_CAP_PORT_decode2A->CNTR = 0;//获取编码器计数值
    // Do something
  }
}

/*********************************************************************************************************//**
 * @brief   This function handles BFTM interrupt.
 * @retval  None
 ************************************************************************************************************/
//void BFTM1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles I2C interrupt.
 * @retval  None
 ************************************************************************************************************/
//void I2C0_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles I2C interrupt.
 * @retval  None
 ************************************************************************************************************/
//void I2C1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles I2C interrupt.
 * @retval  None
 ************************************************************************************************************/
//void I2C2_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles SPI interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SPI0_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles SPI interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SPI1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles USART interrupt.
 * @retval  None
 ************************************************************************************************************/
 

void USART0_IRQHandler(void)
{
	extern uint16_t point1;
	extern char USART_RX_BUF[USART_REC_LEN];
	extern _SaveData Save_Data;
	// Check if UART0 receive interrupt is pending
    if(USART_GetFlagStatus(GPS_UART_HT, USART_FLAG_RXDR) == SET)
    {
    uint8_t Res = 0;
    USART_ClearFlag(GPS_UART_HT, USART_FLAG_RXDR);
        Res =USART_ReceiveData(GPS_UART_HT);//(USART1->DR);	//读取接收到的数据
	
	if(Res == '$')
	  {
		point1 = 0;	
	  }
		
	  USART_RX_BUF[point1++] = Res;

	if(USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' && USART_RX_BUF[5] == 'C')			//确定是否收到"GPRMC/GNRMC"这一帧数据
	  {
		if(Res == '\n')									   
		{
			memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
			memcpy(Save_Data.GPS_Buffer, USART_RX_BUF, point1); 	//保存数据
			Save_Data.isGetData = true;
			point1 = 0;
			memset(USART_RX_BUF, 0, USART_REC_LEN);      //清空				
		}			
	  }
	
	if(point1 >= USART_REC_LEN)
	  {
		point1 = USART_REC_LEN;
	  }	
    }
	
	
}

/*********************************************************************************************************//**
 * @brief   This function handles USART interrupt.
 * @retval  None
 ************************************************************************************************************/
//void USART1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles UART interrupt.
 * @retval  None
 ************************************************************************************************************/
//void UART0_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles UART interrupt.
 * @retval  None
 ************************************************************************************************************/
//void UART1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles UART interrupt.
 * @retval  None
 ************************************************************************************************************/
//void UART2_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles UART interrupt.
 * @retval  None
 ************************************************************************************************************/
//void UART3_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles UART interrupt.
 * @retval  None
 ************************************************************************************************************/
//void UART0_UART2_IRQHandler(void)
//{
//  #if (RETARGET_PORT == RETARGET_UART0) || (RETARGET_PORT == RETARGET_UART2)
//    #if (RETARGET_INT_MODE == 1)
//    RETARGET_UART_IRQHandler();
//    #endif
//  #endif
//}

/*********************************************************************************************************//**
 * @brief   This function handles UART interrupt.
 * @retval  None
 ************************************************************************************************************/
//void UART1_UART3_IRQHandler(void)
//{
//  #if (RETARGET_PORT == RETARGET_UART1) || (RETARGET_PORT == RETARGET_UART3)
//    #if (RETARGET_INT_MODE == 1)
//    RETARGET_UART_IRQHandler();
//    #endif
//  #endif
//}

/*********************************************************************************************************//**
 * @brief   This function handles SCI interrupt.
 * @retval  None
 ************************************************************************************************************/
//void SCI_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles I2S interrupt.
 * @retval  None
 ************************************************************************************************************/
//void I2S_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles USB interrupt.
 * @retval  None
 ************************************************************************************************************/
//void USB_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles PDMA interrupt.
 * @retval  None
 ************************************************************************************************************/
//void PDMA_CH0_1_IRQHandler(void)
//{
//}

/*********************************************************************************************************//**
 * @brief   This function handles PDMA interrupt.
 * @retval  None
 ************************************************************************************************************/
//void PDMA_CH2_5_IRQHandler(void)
//{
//}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
