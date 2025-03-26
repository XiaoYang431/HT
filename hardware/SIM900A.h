#ifndef __SIM900A_H
#define __SIM900A_H
#include "ht32f5xxxx_01.h"              // Device header



/****************************************************************/

//USE UART3
#define  SIM_RX_PINGroup  GPIO_PA
#define  SIM_RX_PIN       GPIO_PIN_5
#define  SIM_TX_PINGroup  GPIO_PA
#define  SIM_TX_PIN       GPIO_PIN_4 
#define  SIM_UART         UART3
#define  SIM_UART_HT   HT_UART3
#define  SIM_RX_PINGroup_HT HT_GPIOA








/*用户必改 xxxxxxxxxxx 改成接收短信的电话号码*/
#define		PHONE_NUMBER		"xxxxxxxxxxx"//接收短信的电话号码


#define		SIM900A_SEND_AT							"AT\r\n"
//1.选择短消息格式 （0：PDU模式 1：文本模式）
#define		SIM900A_SELECT_SMS_FORMAT				"AT+CMGF=1\r\n"			
//2.选择TE字库集
#define		SIM900A_SELECT_TE_FONT					"AT+CSCS=\"UCS2\"\r\n"
//3.保存SMS设置
#define		SIM900A_SAVE_SMS_SET					"AT+CSCA?\r\n"
//4.设置短消息文本模式参数
#define		SIM900A_SET_SMS_TEST_MODE				"AT+CSMP=17,167,0,25\r\n"



extern uint8_t  sim900a_receive_data[128];
extern uint16_t  sim900a_receive_count;

extern uint8_t verification_code[7];		//存放验证码



uint8_t sim900a_send_cmd(uint8_t *str,uint8_t *ack,uint16_t time);	//SIM900A模块发送字符串指令 函数
uint8_t sim900a_send_end(uint8_t data,uint8_t *ack,uint16_t time);	//SIM900A模块发送单字节指令 函数
void generate_verification_code(char *data,uint8_t number);			// 随机生成验证码（'0'~'9'）
void ASCII_TO_Unicode(char *ASCII,char *Unicode);					// ASCII 转 unicode
uint8_t sim900a_init(void);
uint8_t sim900a_send_verification_code(void);
#endif



