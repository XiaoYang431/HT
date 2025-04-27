#ifndef __SIM900A_H__
#define __SIM900A_H__
#include "ht32f5xxxx_01.h"              // Device header

/****************************************************************/

//USE UART3
#define  SIM_RX_PINGroup  GPIO_PA
#define  SIM_RX_PIN       GPIO_PIN_5
#define  SIM_TX_PINGroup  GPIO_PA
#define  SIM_TX_PIN       GPIO_PIN_4
#define  SIM_UART         USART1
#define  SIM_UART_HT   HT_USART1
#define  SIM_RX_PINGroup_HT HT_GPIOA

/*用户必改 xxxxxxxxxxx 改成接收短信的电话号码*/
#define		PHONE_NUMBER		"17707592248"//接收短信的电话号码




#define RX_BUFFER_SIZE 20
void SIM_USART_Init(void);
void SIM_USART_SendString(const char *str);
void SIM900A_SendCommand(const char *cmd);
void SIM900A_SendSms(char *phone, char *message);
void SIM900A_Process(void);
void SIM900A_makeCall( char *phoneNumber);

#endif

