#include "SIM900A.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ht32f5xxxx_01.h"              // Device header

uint8_t verification_code[7];		//存放验证码 


char sim900A_flag=0;

uint8_t sim900a_receive_data[128] = {0};
uint16_t sim900a_receive_count = 0;

void SIM_Usart_sendString(const char *str)
{
	while(*str)
	{
		USART_SendData(SIM_UART_HT, *(str++));
		while(USART_GetFlagStatus(SIM_UART_HT, USART_FLAG_TXDE) == RESET);
	}
}


/**********************************************************************
/ *********************************************************************
描述:   SIM900A模块发送指令 函数
参数: *str:指令内容 ack:正常返回结果的数据 timeout_ms:允许时间(单位：10ms)
返回: 0:指令得到正确回应 1: 指令没得到回应
***********************************************************************/
uint8_t sim900a_send_cmd(uint8_t *str, uint8_t *ack, uint16_t timeout_ms)
{
    memset(sim900a_receive_data, '\0', sizeof(sim900a_receive_data));
    sim900a_receive_count = 0;

    SIM_Usart_sendString((char *)str);  // 发送AT指令

    if (ack == NULL) return 0;

    while (timeout_ms--)
    {
        SIM900A_delay_ms(10);  // 延时 10ms，减少 CPU 空闲时间
        if (strstr((char *)sim900a_receive_data, (char *)ack) != NULL)
        {
            return 1; // 成功匹配ACK
        }
    }
    return 0; // 超时未匹配ACK
}

/**********************************************************************
描述:   SIM900A模块发送指令 函数
参数: *str:指令内容 ack:正常返回结果的数据 timeout_ms:允许时间(单位：10ms)
返回: 0:指令得到正确回应 1: 指令没得到回应
***********************************************************************/
uint8_t sim900a_send_end(uint8_t data, uint8_t *ack, uint16_t timeout_ms)
{
    sim900a_receive_count = 0;  
    memset(sim900a_receive_data, '\0', sizeof(sim900a_receive_data));

    USART_SendData(SIM_UART_HT, data);  // 发送 Ctrl+Z 或结束符

    if (ack == NULL) return 1;  // 无需判断，直接成功

    while (timeout_ms--)
    {
        SIM900A_delay_ms(10);  // 每次检查间隔 10ms
        if (strstr((char *)sim900a_receive_data, (char *)ack) != NULL)
        {
            return 1; // 成功找到 ack
        }
    }

    return 0; // 超时未找到 ack，失败
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
	ack = sim900a_send_cmd((uint8_t *)SIM900A_SEND_AT, (uint8_t *)"OK", 30);

	while(ack)
	{
		if(err-- == 0)
		{
			//printf("AT FAIL\r\n");
			return 1;	
		}
		
	strcpy((char *)sim900a_send_buffer,"AT+CMGS=\"");
	
	//发送指令AT 检测是否返回OK 给300ms时间检测
	ack = sim900a_send_cmd((uint8_t *)SIM900A_SEND_AT, (uint8_t *)"OK", 30);
	}
	//printf("AT OK\r\n");
	err = 3;
		
	//发送 短消息格式 指令 检测是否返回OK 给300ms时间检测

	ack = sim900a_send_cmd((uint8_t *)SIM900A_SELECT_SMS_FORMAT, (uint8_t *)"OK", 30);

	while(ack)
	{
		if(err-- == 0)
		{
			//printf("SMS FORMAT FAIL\r\n");	
			return 1;	
		}
		ack = sim900a_send_cmd((uint8_t *)SIM900A_SELECT_SMS_FORMAT, (uint8_t *)"OK", 30);
	}
	//printf("SMS FORMAT SUCCESS\r\n");
	err = 3;



	//发送 选择TE字库集 指令 检测是否返回OK 给300ms时间检测

	ack = sim900a_send_cmd((uint8_t *)SIM900A_SELECT_TE_FONT, (uint8_t *)"OK", 30);
	while(ack)
	{
		if(err-- == 0)
		{
			//printf("SELECT_TE_FONT FAIL\r\n");
			return 1;	
		}
	ack = sim900a_send_cmd((uint8_t *)SIM900A_SELECT_TE_FONT, (uint8_t *)"OK", 30);
	}
	//printf("SELECT_TE_FONT SUCCESS\r\n");
	err = 3;

	//发送 选择TE字库集 指令 不检测返回值
	ack = sim900a_send_cmd((uint8_t *)SIM900A_SET_SMS_TEST_MODE, (uint8_t *)"OK", 30);
	while(ack)
	{
		if(err-- == 0)
		{
			//printf("SAVE_SMS_SET FAIL\r\n");
			return 1;	
		}
	ack = sim900a_send_cmd((uint8_t *)SIM900A_SAVE_SMS_SET, NULL, 0);
	}
	//printf("SAVE_SMS_SET SUCCESS\r\n");
	err = 3;


	//发送 设置短消息文本模式参数 指令 检测是否返回OK 给300ms时间检测
	ack = sim900a_send_cmd((uint8_t *)SIM900A_SET_SMS_TEST_MODE, (uint8_t *)"OK", 30);
		while(ack)
		{
			if(err-- == 0)
			{
				//printf("SMS TEST MODE FAIL\r\n");
				return 1;	
			}
	ack = sim900a_send_cmd((uint8_t *)SIM900A_SET_SMS_TEST_MODE, (uint8_t *)"OK", 30);
		}
		//printf("SMS TEST MODE SUCCESS\r\n");
		err = 3;

	//让我们的设置接收短信方电话号码生成unicode编码	
	ASCII_TO_Unicode(PHONE_NUMBER, (char *)&sim900a_send_buffer[strlen((const char *)sim900a_send_buffer)]);
	strcat((char *)sim900a_send_buffer,"\"\r\n"); 		
	//printf("%s\r\n",sim900a_send_buffer);
	//发送 接收短信方电话号码 指令 检测是否返回OK 给1000ms时间检测

	 ack = sim900a_send_cmd((uint8_t *)sim900a_send_buffer, (uint8_t *)">", 100);	
		while(ack)
		{
			if(err-- == 0)
			{
				//printf("SET PHONE NUMBER FAIL\r\n");
				return 1;	
			}
	 ack = sim900a_send_cmd((uint8_t *)sim900a_send_buffer, (uint8_t *)">", 100);
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
		ASCII_TO_Unicode((char *)verification_code, (char *)&sim900a_send_buffer[strlen((const char *)sim900a_send_buffer)]);
		strcat((char *)sim900a_send_buffer,"\r\n");	//加上回车换行 
		//printf("%s\r\n",sim900a_send_buffer);
		//3-发送数据

	       ack = sim900a_send_cmd((uint8_t *)sim900a_send_buffer, (uint8_t *)">", 300);
	
		while(ack)
		{
			if(err-- == 0)
			{
				//printf("SEND DATA FAIL\r\n");
				return 1;	
			}
	       ack = sim900a_send_cmd((uint8_t *)sim900a_send_buffer, (uint8_t *)">", 300);
		}
		//printf("SEND DATA SUCCESS\r\n");
		
		//4-发送 0X1A 标志
		ack = sim900a_send_end(0x1a, (uint8_t *)"OK", 800);
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
    CKCUClock.Bit.PA = 1; // Assuming UART is on GPIO Port C
    CKCUClock.Bit.SIM_UART = 1; // Assuming using UART3
    CKCU_PeripClockConfig(CKCUClock, ENABLE);

    GPIO_PullResistorConfig(SIM_RX_PINGroup_HT, SIM_RX_PIN, GPIO_PR_UP);

    // Step 2: Configure GPIO pins for UART TX and RX
    AFIO_GPxConfig(SIM_TX_PINGroup, SIM_TX_PIN, AFIO_FUN_USART_UART); // PC14 as UART3 TX
    AFIO_GPxConfig(SIM_RX_PINGroup, SIM_RX_PIN, AFIO_FUN_USART_UART); // PC15 as UART3 RX

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

     //Step 6: Configure NVIC for UART interrupt
 NVIC_SetPriority(UART3_IRQn, 2); // 设置中断优先级为1，优先级值越小，优先级越高
    NVIC_EnableIRQ(UART3_IRQn);
}

//清空缓存区
void SIM900A_ClearBuffer()
{
    sim900a_receive_count = 0;
    memset((char *)sim900a_receive_data, 0, sizeof(sim900a_receive_data));
}
/**********************************************************************
描述: SIM900A 初始化 函数
返回: 0:成功 1:失败
***********************************************************************/
uint8_t sim900a_init(void)
{
    SIM_USART_Init();   // 初始化串口
    uint8_t retry = 3;
    uint8_t ack;

    while (retry--)
    {
        SIM900A_ClearBuffer();  // 清空接收缓冲
        ack = sim900a_send_cmd((uint8_t *)"AT\r", (uint8_t *)"OK", 30);

        if (ack == 0)
        {
            // printf("SIM900A Ready\r\n");
            return 0;  // 成功
        }

        SIM900A_delay_ms(300);  // 模块可能在启动，延时等它响应
    }

    // printf("SIM900A Init Failed\r\n");
    return 1;  // 初始化失败
}
		
void SIM900A_delay_ms(vu32 count)
{
  count = SystemCoreClock / 8000 * count ;
  while(count--);
}

void SIM900A_delay_us(vu32 count)
{
  count = SystemCoreClock / 8000 * count /1000;
  while(count--);
}

int USART_ReadResponse(HT_USART_TypeDef* USARTx, char* response, int max_len)
{
    int i = 0;
    char c;
    
    // 清空响应数组
    memset(response, 0, max_len);

    // 循环读取串口数据直到达到最大长度或收到换行符
    while (i < max_len - 1) {
        // 等待数据接收
        if (USART_GetFlagStatus(USARTx, USART_FLAG_TXDE) != RESET) {   
            c = USART_ReceiveData(USARTx);  // 获取接收到的数据

            // 如果是换行符，表示响应结束
            if (c == '\n') {
                break;
            }

            // 将字符存入响应数组
            response[i++] = c;
        }
    }

    // 返回读取的字符数
    return i;
}

//发中文短信
void SIM900A_SendCHSMS(char *message, char *phonenumber)
{
    char unicode_message[512] = {0};        // UCS2 编码后的短信内容
    char unicode_number[64] = {0};          // UCS2 编码后的手机号
    char cmd_buffer[100] = {0};             // 指令缓存
    uint8_t ack = 0;

    // 将手机号转换为 UCS2 编码字符串
    ASCII_TO_Unicode(phonenumber, unicode_number);

    // 将短信内容转换为 UCS2 编码字符串
    ASCII_TO_Unicode(message, unicode_message);

    // 1. 发送 AT 指令，检查模块
    ack = sim900a_send_cmd((uint8_t *)"AT\r", (uint8_t *)"OK", 50);
    if (!ack) return;

    // 2. 设置短信格式为文本模式
    ack = sim900a_send_cmd((uint8_t *)"AT+CMGF=1\r", (uint8_t *)"OK", 50);
    if (!ack) return;

    // 3. 设置编码格式为 UCS2
    ack = sim900a_send_cmd((uint8_t *)"AT+CSCS=\"UCS2\"\r", (uint8_t *)"OK", 50);
    if (!ack) return;

    // 4. 设置短信参数（避免乱码）
    ack = sim900a_send_cmd((uint8_t *)"AT+CSMP=17,167,0,8\r", (uint8_t *)"OK", 50);
    if (!ack) return;

    // 5. 设置接收号码
    sprintf(cmd_buffer, "AT+CMGS=\"%s\"\r", unicode_number);
    ack = sim900a_send_cmd((uint8_t *)cmd_buffer, (uint8_t *)">", 200);
    if (!ack) return;

    // 6. 发送短信正文（Unicode 编码）
    SIM_Usart_sendString((char *)unicode_message);
    SIM900A_delay_ms(100); 

    // 7. 发送 Ctrl+Z 结束符
    ack = sim900a_send_end(0x1A, (uint8_t *)"OK", 800);
}

//发英文短信
void SIM900A_SendENGSMS(char *message, char *phonenumber)
{
    char cmd_buffer[100] = {0};
    uint8_t ack = 0;

    // 1. 模块基础测试
    ack = sim900a_send_cmd((uint8_t *)"AT\r", (uint8_t *)"OK", 100);
    if (!ack) return;

    // 2. 设置为文本模式
    ack = sim900a_send_cmd((uint8_t *)"AT+CMGF=1\r", (uint8_t *)"OK", 100);
    if (!ack) return;

    // 3. 设置字符集为 GSM
    ack = sim900a_send_cmd((uint8_t *)"AT+CSCS=\"GSM\"\r", (uint8_t *)"OK", 100);
    if (!ack) return;

    // 4. 查询短信中心号码（可选）
    ack = sim900a_send_cmd((uint8_t *)"AT+CSCA?\r", (uint8_t *)"OK", 100);
    if (!ack) return;

    // 5. 设置短信参数
    ack = sim900a_send_cmd((uint8_t *)"AT+CSMP=17,167,0,240\r", (uint8_t *)"OK", 100);
    if (!ack) return;

    // 6. 设定接收方号码
    sprintf(cmd_buffer, "AT+CMGS=\"%s\"\r", phonenumber);
    ack = sim900a_send_cmd((uint8_t *)cmd_buffer, (uint8_t *)">", 500);
    if (!ack) return;

    // 7. 发送短信内容
    SIM_Usart_sendString(message);
    SIM900A_delay_ms(800);  // 适当等待硬件缓冲

    // 8. 发送 Ctrl+Z 表示结束
    ack = sim900a_send_end(0x1A, (uint8_t *)"OK", 1000);
	sim900A_flag=1;
}

//打电话
int SIM900A_MakeCall(const char* phoneNumber)
{
    char cmd[32];
    char response[128];  // 用于存储 AT 命令的响应

    // 1. 发送 AT 命令确认模块在线
    SIM_Usart_sendString("AT\r\n");
    SIM900A_delay_ms(1000);  // 可选，等待响应

    // 2. 启用拨号功能（可省略）
    SIM_Usart_sendString("AT+COLP=1\r\n");
    SIM900A_delay_ms(1000);

    // 3. 构建拨号命令并发送
    sprintf(cmd, "ATD%s;\r\n", phoneNumber);  // 注意 ATD 后有分号
    SIM_Usart_sendString(cmd);

    // 4. 查询拨号状态
    SIM900A_delay_ms(2000);  // 等待拨号完成
    SIM_Usart_sendString("AT+CLCC\r\n");  // 查询当前通话状态
    SIM900A_delay_ms(1000);  // 等待响应

    // 假设有一个函数来读取串口响应
    if (USART_ReadResponse(SIM_UART_HT, response, sizeof(response)) > 0) {
        // 检查响应中是否包含通话状态
        if (strstr(response, "+CLCC: 1,1") != NULL) {
            // 通话已建立
            return 1;  // 拨号成功
        } else {
            // 通话未建立
            return 0;  // 拨号失败
        }
    }
    return 0;  // 如果没有正确响应，则视为失败
}


//挂电话
void SIM900A_HangUp(void)
{
    SIM_Usart_sendString("ATH\r\n");
    SIM900A_delay_ms(1000);  // 给模块一点响应时间
}

