#ifndef GPS_H
#define GPS_H

#define  GPS_RX_PINGroup  GPIO_PA
#define  GPS_RX_PIN       GPIO_PIN_10
#define  GPS_TX_PINGroup  GPIO_PA
#define  GPS_TX_PIN       GPIO_PIN_8 
#define  GPS_UART         USART0
#define  GPS_UART_HT   HT_USART0
#define  GPS_RX_PINGroup_HT HT_GPIOA




#define  USART_REC_LEN  150
#define GPS_Buffer_Length 150
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 
#include "string.h"

typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//是否获取到GPS数据
	char isParseData;	//是否解析完成
	char UTCTime[UTCTime_Length];		//UTC时间
	char latitude[latitude_Length];		//纬度
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//经度
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//定位信息是否有效
} _SaveData;

void GPS_Init(void);
void parseGpsBuffer(void);
#endif
