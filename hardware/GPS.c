#include "ht32f5xxxx_01.h"              // Device header
#include "GPS.h"
#include <stdio.h> 
#include <stdbool.h>

 uint16_t point1 = 0;
 char USART_RX_BUF[USART_REC_LEN];
_SaveData Save_Data = {0};

void GPS_Init(void)
{
     // Step 1: Enable peripheral clocks for GPIO and UART
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.PA = 1; // Assuming UART is on GPIO Port A
    CKCUClock.Bit.GPS_UART = 1; // Assuming using UART0
    CKCU_PeripClockConfig(CKCUClock, ENABLE);


    GPIO_PullResistorConfig(GPS_RX_PINGroup_HT, GPS_RX_PIN, GPIO_PR_UP);

    // Step 2: Configure GPIO pins for UART TX and RX
    AFIO_GPxConfig(GPS_TX_PINGroup, GPS_TX_PIN, AFIO_FUN_USART_UART); // PA2 as UART0 TX
    AFIO_GPxConfig(GPS_RX_PINGroup, GPS_RX_PIN, AFIO_FUN_USART_UART); // PA3 as UART0 RX

    // Step 3: Configure UART parameters
    USART_InitTypeDef UART_InitStructure;
    UART_InitStructure.USART_BaudRate = 9600;
    UART_InitStructure.USART_WordLength = USART_WORDLENGTH_8B;
    UART_InitStructure.USART_StopBits = USART_STOPBITS_1;
    UART_InitStructure.USART_Parity = USART_PARITY_NO;
    UART_InitStructure.USART_Mode = USART_MODE_NORMAL;
    USART_Init(GPS_UART_HT, &UART_InitStructure);

    // Step 4: Enable UART
    USART_TxCmd(GPS_UART_HT, ENABLE);
    USART_RxCmd(GPS_UART_HT, ENABLE);

    // Step 5: Enable UART receive interrupt
    USART_IntConfig(GPS_UART_HT, USART_INT_RXDR, ENABLE);

    // Step 6: Configure NVIC for UART interrupt
    NVIC_SetPriority(UART0_IRQn, 1); // 设置中断优先级为1，优先级值越小，优先级越高
    NVIC_EnableIRQ(UART0_IRQn);

}

void errorLog(int num)
{
	
	while(1)
	{
	  	printf("ERROR%d\r\n",num);
	}
}
//获取位置信息
//需要通过商家给的串口进行初始化配置
void parseGpsBuffer(void)
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		//printf("**************\r\n");
		///printf(Save_Data.GPS_Buffer);

		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//解析错误
				}
			}


		}
	}
}

void printGpsBuffer(void)
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		
		printf("Save_Data.UTCTime = ");
		printf(Save_Data.UTCTime);
		printf("\r\n");

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			printf("Save_Data.latitude = ");
			printf(Save_Data.latitude);
			printf("\r\n");


			printf("Save_Data.N_S = ");
			printf(Save_Data.N_S);
			printf("\r\n");

			printf("Save_Data.longitude = ");
			printf(Save_Data.longitude);
			printf("\r\n");

			printf("Save_Data.E_W = ");
			printf(Save_Data.E_W);
			printf("\r\n");
		}
		else
		{
			printf("GPS DATA is not usefull!\r\n");
		}	
	}
}


void clrStruct()
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
	memset(Save_Data.UTCTime, 0, UTCTime_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.N_S, 0, N_S_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
	memset(Save_Data.E_W, 0, E_W_Length);
}


void UART0_IRQHandler(void)
{
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

void CLR_Buf(void)                           // 串口缓存清理
{
	memset(USART_RX_BUF, 0, USART_REC_LEN);      //清空
    point1 = 0;                    
}

void GPS_ReStart(void)
{
	USART_SendData(GPS_UART_HT, "$PCAS10,2*1E\r\n");
}
void GPS_Stop(void)
{
	USART_SendData(GPS_UART_HT, "$PCAS03,0,0,0,0,0,0,0,0*02\r\n");
}
void GPS_Set_Start(void)
{
	USART_SendData(GPS_UART_HT, "$PCAS03,0,0,0,0,1,0,0,0*03\r\n\r\n");
}
//永久保存配置信息
void GPS_Save_Config(void)
{
	USART_SendData(GPS_UART_HT, "$PCAS00*01\r\n");
}