#include "SIM900A.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Delay.h"



uint8_t verification_code[7];		//存放验证码 




uint8_t sim900a_receive_data[128] = {0};
uint16_t sim900a_receive_count = 0;

void SIM_Usart_sendString(char *str)
{
	while(*str)
	{
		USART_SendData(SIM_UART_HT, *(str++));
		while(USART_GetFlagStatus(SIM_UART_HT, USART_FLAG_TXDE) == RESET);
	}
}


/**********************************************************************
描述:   SIM900A模块发送指令 函数
参数: *str:指令内容	ack:正常返回结果的数据 time:允许时间(1次代表 10ms)
返回: 0:指令得到正确回应 1: 指令没得到回应
***********************************************************************/
uint8_t sim900a_send_cmd(uint8_t *str,uint8_t *ack,uint16_t time)
{
	memset(sim900a_receive_data,'\0',sizeof(sim900a_receive_data));
	sim900a_receive_count = 0;
	
	SIM_Usart_sendString((char *)str);		//发送指令
	if(ack == NULL)	return 0;
	while(time--)
	{
		Delay_ms(10);
		//寻找要找的关键词 找到关中断接收 并且返回0
		if(strstr((char *)sim900a_receive_data,(char *)ack) != NULL)
		{
				//关闭中断接收
			return 0;
		}	
	}
	
	//关闭中断接收
	return 1;				//超时还没收到想要得到的关键词 关串口中断 并且返回1			
}


/**********************************************************************
描述:   SIM900A模块发送指令 函数
参数: *str:指令内容	ack:正常返回结果的数据 time:允许时间(1次代表 10ms)
返回: 0:指令得到正确回应 1: 指令没得到回应
***********************************************************************/
uint8_t sim900a_send_end(uint8_t data,uint8_t *ack,uint16_t time)
{
	sim900a_receive_count = 0;	
	memset(sim900a_receive_data,'\0',sizeof(sim900a_receive_data));
	

	USART_SendData(SIM_UART_HT,data);		//发送结束指令
	if(ack == NULL)	return 0;
	
	//开启中断接收
	
	
	while(time--)
	{
		Delay_ms(10);
		//寻找要找的关键词 找到关中断接收 并且返回0
		if(strstr((char *)sim900a_receive_data,(char *)ack) != NULL)
		{
				//关闭中断接收
			return 0;
		}	
	}
			//关闭中断接收
	return 1;					//超时还没收到想要得到的关键词 关串口中断 并且返回1			
}



/**********************************************************************
描述:   随机生成验证码（'0'~'9'）
        产生字符 '0' ~ '9'  十进制也就是 48 ~ 57
***********************************************************************/
void generate_verification_code(char *data,uint8_t number)
{
    uint8_t i;
//    srand(time(NULL));

    for(i=0;i<number;i++)
    {
        data[i] = 48 + (rand() % 10);
    }
}

/**********************************************************************
描述: ASCII 转 unicode      比如 '1'  转成 "0031"
***********************************************************************/
void ASCII_TO_Unicode(char *ASCII,char *Unicode)
{
    int length;
    int i = 0;
    int j = 0;
	memset(Unicode,'\0',sizeof(Unicode)/sizeof(char));
    length = strlen(ASCII);

    for(i=0;i<length;i++)
    {
        Unicode[j++] = '0';
        Unicode[j++] = '0';

        Unicode[j++] = (ASCII[i] / 16) + 0x30;
        Unicode[j++] = (ASCII[i] % 16) + 0x30;
    }

}

/**********************************************************************
描述: SIM900A 发送验证码 函数
返回: 0:成功 1:失败
***********************************************************************/
uint8_t sim900a_send_verification_code(void)
{
	uint8_t ack;		//存放状态
	uint8_t err = 3;	//设置可错误次数 
	uint8_t send_count = 3;	//发送次数 发送次数达到还是没成功则放弃发送
	uint8_t sim900a_send_buffer[96] = {0};

SEND_AT:
	strcpy((char *)sim900a_send_buffer,"AT+CMGS=\"");
	
	//发送指令AT 检测是否返回OK 给300ms时间检测
	ack = sim900a_send_cmd(SIM900A_SEND_AT,"OK",30);	
	while(ack)
	{
		if(err-- == 0)
		{
			//printf("AT FAIL\r\n");
			return 1;	
		}
		
	strcpy((char *)sim900a_send_buffer,"AT+CMGS=\"");
	
	//发送指令AT 检测是否返回OK 给300ms时间检测
	ack = sim900a_send_cmd(SIM900A_SEND_AT,"OK",30);
	}
	//printf("AT OK\r\n");
	err = 3;
		
	//发送 短消息格式 指令 检测是否返回OK 给300ms时间检测

	ack = sim900a_send_cmd(SIM900A_SELECT_SMS_FORMAT,"OK",30);	
	while(ack)
	{
		if(err-- == 0)
		{
			//printf("SMS FORMAT FAIL\r\n");	
			return 1;	
		}
		ack = sim900a_send_cmd(SIM900A_SELECT_SMS_FORMAT,"OK",30);	

	}
	//printf("SMS FORMAT SUCCESS\r\n");
	err = 3;



	//发送 选择TE字库集 指令 检测是否返回OK 给300ms时间检测

	ack = sim900a_send_cmd(SIM900A_SELECT_TE_FONT,"OK",30);	
	while(ack)
	{
		if(err-- == 0)
		{
			//printf("SELECT_TE_FONT FAIL\r\n");
			return 1;	
		}
	ack = sim900a_send_cmd(SIM900A_SELECT_TE_FONT,"OK",30);	
	
	}
	//printf("SELECT_TE_FONT SUCCESS\r\n");
	err = 3;



	//发送 选择TE字库集 指令 不检测返回值

	ack = sim900a_send_cmd(SIM900A_SAVE_SMS_SET,NULL,0);	
	while(ack)
	{
		if(err-- == 0)
		{
			//printf("SAVE_SMS_SET FAIL\r\n");
			return 1;	
		}
	ack = sim900a_send_cmd(SIM900A_SAVE_SMS_SET,NULL,0);	
	}
	//printf("SAVE_SMS_SET SUCCESS\r\n");
	err = 3;


	//发送 设置短消息文本模式参数 指令 检测是否返回OK 给300ms时间检测

	ack = sim900a_send_cmd(SIM900A_SET_SMS_TEST_MODE,"OK",30);	
		while(ack)
		{
			if(err-- == 0)
			{
				//printf("SMS TEST MODE FAIL\r\n");
				return 1;	
			}
	ack = sim900a_send_cmd(SIM900A_SET_SMS_TEST_MODE,"OK",30);	
		}
		//printf("SMS TEST MODE SUCCESS\r\n");
		err = 3;


	//让我们的设置接收短信方电话号码生成unicode编码	
	ASCII_TO_Unicode(PHONE_NUMBER,&sim900a_send_buffer[strlen(sim900a_send_buffer)]);
	strcat((char *)sim900a_send_buffer,"\"\r\n"); 		
	//printf("%s\r\n",sim900a_send_buffer);
	//发送 接收短信方电话号码 指令 检测是否返回OK 给1000ms时间检测

	ack = sim900a_send_cmd(sim900a_send_buffer,">",100);	
		while(ack)
		{
			if(err-- == 0)
			{
				//printf("SET PHONE NUMBER FAIL\r\n");
				return 1;	
			}
	ack = sim900a_send_cmd(sim900a_send_buffer,">",100);	
		}
		//printf("SET PHONE NUMBER SUCCESS\r\n");
		err = 3;		
		
		//清空缓冲区 准备为发送验证码做准备
		memset(sim900a_send_buffer,'\0',sizeof(sim900a_send_buffer));
		
		

		
		//在这里开始就是开始发送内容了
		//1-生成验证码
		strcpy((char *)sim900a_send_buffer,"9A8C8BC17801003A");	//添加unicode码 "验证码:"
		generate_verification_code((char *)verification_code,6);//生成验证码
		//printf("验证码生成结果:%s\r\n",verification_code);		
		
		//2-将随机产生的6位验证码转Unicode编码 并且拼接一起
		ASCII_TO_Unicode((char *)verification_code,&sim900a_send_buffer[strlen(sim900a_send_buffer)]);
		strcat((char *)sim900a_send_buffer,"\r\n");	//加上回车换行 
		//printf("%s\r\n",sim900a_send_buffer);
		//3-发送数据

	ack = sim900a_send_cmd(sim900a_send_buffer,">",300);	
		while(ack)
		{
			if(err-- == 0)
			{
				//printf("SEND DATA FAIL\r\n");
				return 1;	
			}
	ack = sim900a_send_cmd(sim900a_send_buffer,">",300);	
		}
		//printf("SEND DATA SUCCESS\r\n");
		
		//4-发送 0X1A 标志
		ack = sim900a_send_end(0x1a,"OK",800);
		if(ack)
		{
			if(send_count-- == 0)	return 1;
			memset(sim900a_send_buffer,'\0',sizeof(sim900a_send_buffer));
			err = 3;
			goto SEND_AT;		//重发短信
		}
		
		//printf("SEND MESSAGE SUCCESS\r\n");
	return 0;
}

void SIM_USART_Init(void)  
{
 // Step 1: Enable peripheral clocks for GPIO and UART
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.PA = 1; // Assuming UART is on GPIO Port A
    CKCUClock.Bit.SIM_UART = 1; // Assuming using UART3
    CKCU_PeripClockConfig(CKCUClock, ENABLE);


    GPIO_PullResistorConfig(SIM_RX_PINGroup_HT, SIM_RX_PIN, GPIO_PR_UP);

    // Step 2: Configure GPIO pins for UART TX and RX
    AFIO_GPxConfig(SIM_TX_PINGroup, SIM_TX_PIN, AFIO_FUN_USART_UART); // PA2 as UART0 TX
    AFIO_GPxConfig(SIM_RX_PINGroup, SIM_RX_PIN, AFIO_FUN_USART_UART); // PA3 as UART0 RX

    // Step 3: Configure UART parameters
    USART_InitTypeDef UART_InitStructure;
    UART_InitStructure.USART_BaudRate = 9600;
    UART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
    UART_InitStructure.USART_StopBits = USART_STOPBITS_1;
    UART_InitStructure.USART_Parity = USART_PARITY_NO;
    UART_InitStructure.USART_Mode = USART_MODE_NORMAL;
    USART_Init(SIM_UART_HT, &UART_InitStructure);

    // Step 4: Enable UART
    USART_TxCmd(SIM_UART_HT, ENABLE);
    USART_RxCmd(SIM_UART_HT, ENABLE);

    // Step 5: Enable UART receive interrupt
    USART_IntConfig(SIM_UART_HT, USART_INT_RXDR, ENABLE);

    // Step 6: Configure NVIC for UART interrupt
    NVIC_SetPriority(UART3_IRQn, 2); // 设置中断优先级为1，优先级值越小，优先级越高
    NVIC_EnableIRQ(UART3_IRQn);
}
/**********************************************************************
描述: SIM900A 初始化 函数
返回: 0:成功 1:失败
***********************************************************************/
uint8_t sim900a_init(void)
{
	SIM_USART_Init();
	uint8_t ack;		//存放状态
	uint8_t err;		//设置可错误次数 

	err = 3;
	//发送指令AT 检测是否返回OK 给300ms时间检测

	
	ack = sim900a_send_cmd(SIM900A_SEND_AT,"OK",30);	
		while(ack)
		{
			if(err-- == 0)
			{
				//printf("AT FAIL\r\n");				//ESP8266模块不存在
				return 1;	
			}

	ack = sim900a_send_cmd(SIM900A_SEND_AT,"OK",30);	
		}
		//printf("AT SUCCESS\r\n");
		
	return 0;
}

void USART3_IRQHandler(void)
{
	if(USART_GetFlagStatus(SIM_UART_HT, USART_FLAG_RXDR) == SET)
	{
		USART_ClearFlag(SIM_UART_HT, USART_FLAG_RXDR);

		sim900a_receive_data[sim900a_receive_count++] = USART_ReceiveData(SIM_UART_HT);
	}
}



