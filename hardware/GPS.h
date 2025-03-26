#ifndef GPS_H
#define GPS_H

#define  GPS_RX_PINGroup  GPIO_PA
#define  GPS_RX_PIN       GPIO_PIN_3
#define  GPS_TX_PINGroup  GPIO_PA
#define  GPS_TX_PIN       GPIO_PIN_2 
#define  GPS_UART         UART0
#define  GPS_UART_HT   HT_UART0
#define  GPS_RX_PINGroup_HT HT_GPIOA




#define  USART_REC_LEN  150 //定义最大接收字节数 150
#define GPS_Buffer_Length 150//GPS缓存长度
#define UTCTime_Length 11//UTC时间长度
#define latitude_Length 11//纬度长度
#define N_S_Length 2//N/S长度
#define longitude_Length 12//经度长度
#define E_W_Length 2 //E/W长度
#include "string.h"



/*
	经纬度换算公式
纬度：ddmm.mmmm 北纬 2236.9453 22+(36.9453/60)= 22.615755
经度：dddmm.mmmm 东经 11408.4790 114+(08.4790/60)=114.141317


*/
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
 extern _SaveData Save_Data;


void GPS_Init(void);//GPS初始化
void parseGpsBuffer(void);//解析获取GPS数据
void GPS_ReStart(void);//GPS重启
void GPS_Stop(void);//GPS停止
void GPS_Set_Start(void);//GPS开始
void GPS_Save_Config(void);//GPS保存配置
#endif
