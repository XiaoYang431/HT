#include "ht32f5xxxx_01.h"              
#include "SIM900A.h"      

void SIM900A_delay(vu32 count);
volatile uint8_t sim_rx_buffer[RX_BUFFER_SIZE];
volatile uint16_t sim_rx_index = 0;

// 用于标志接收完成
volatile uint8_t sim_rx_flag = 0;

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
    AFIO_GPxConfig(SIM_TX_PINGroup, SIM_TX_PIN, AFIO_FUN_USART_UART); // PA4 as UART3 TX
    AFIO_GPxConfig(SIM_RX_PINGroup, SIM_RX_PIN, AFIO_FUN_USART_UART); // PA5 as UART3 RX

    // Step 3: Configure UART parameters
    USART_InitTypeDef UART_InitStructure;
    UART_InitStructure.USART_BaudRate = 9600;
    UART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
    UART_InitStructure.USART_StopBits = USART_STOPBITS_1;
    UART_InitStructure.USART_Parity = USART_PARITY_NO;
    UART_InitStructure.USART_Mode = USART_MODE_NORMAL;
    USART_Init(SIM_UART_HT, &UART_InitStructure);

    
    NVIC_EnableIRQ(UART3_IRQn);
    
    USART_IntConfig(SIM_UART_HT, USART_INT_RXDR, ENABLE);
	//USART_IntConfig(SIM_UART_HT, USART_INT_TXDE, ENABLE);

    NVIC_SetPriority(UART3_IRQn, 2); // 设置中断优先级为2，优先级值越小，优先级越高

    USART_TxCmd(SIM_UART_HT, ENABLE);
    USART_RxCmd(SIM_UART_HT, ENABLE);


}

void SIM_USART_SendString(const char *str)
{
    while (*str)
    {
        while (!USART_GetFlagStatus(SIM_UART_HT, USART_FLAG_TXDE));
        USART_SendData(SIM_UART_HT, *str++);
    }
}

void SIM900A_SendCommand(const char *cmd)
{
    SIM_USART_SendString(cmd);
}

void SIM900A_SendSms(char *phone, char *message)
{
    char cmd[50];
// 发送短信命令
    SIM900A_SendCommand("AT\r\n"); 
	SIM900A_delay(200);
	
    SIM900A_SendCommand("AT+CMGF=1\r\n");    
	SIM900A_delay(200);
	
    SIM900A_SendCommand("AT+CSCS=\"GSM\"\r\n");      
	SIM900A_delay(200);
	
    SIM900A_SendCommand("AT+CSMP=17,167,0,240\r\n"); 
	SIM900A_delay(200);
	
    sprintf(cmd, "AT+CMGS=\"%s\"\r\n", phone);	
	SIM900A_SendCommand(cmd); 
	SIM900A_delay(200);
	// 发送短信内容
	SIM_USART_SendString(message);
	SIM900A_delay(500);
    

    USART_SendData(SIM_UART_HT, 0x1A); // 发送Ctrl+Z结束短信（ASCII 0x1A）
	SIM900A_delay(500);
}

void SIM900A_makeCall( char *phoneNumber)
{
    char cmd[50];

    SIM900A_SendCommand("ATE1\r\n"); 
	SIM900A_delay(500);
		
	SIM900A_SendCommand("AT+COLP=1\r\n");
	SIM900A_delay(500);
	//打电话
    sprintf(cmd, "ATD%s;\r\n", phoneNumber);  // 注意 ATD 后有分号
    SIM900A_SendCommand(cmd);
	SIM900A_delay(500);
	
	SIM900A_SendCommand("AT+CLCC\r\n");  // 查询当前通话状态
	
	SIM900A_delay(3000);
	//挂电话
	SIM900A_SendCommand("ATH\r\n");
    SIM900A_delay(500);


}

// 处理接收到的数据（简单版）
void SIM900A_Process(void)
{
    if (sim_rx_flag)
    {
        sim_rx_flag = 0;
        // 在这里可以解析sim_rx_buffer，简单打印或者处理
        // 比如可以通过字符串匹配看有没有 "OK"、"ERROR" 等
    }
}


void SIM900A_delay(vu32 count)
{
  count = SystemCoreClock / 8000 * count;
  while(count--);
}
