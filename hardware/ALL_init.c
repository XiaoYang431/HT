#include "ht32f5xxxx_01.h"
#include "Blueteech.h"
#include "GPS.h"
#include "PUSH.h" 
#include "SIM900A.h"   
#include "BIZHANG.h" 

void init_delay(vu32 count);
void All_init(void)
{
	//电机初始化
	BMP73T104_Init();
	init_delay(100);
	//避障模块初始化
	BiZhang_Init();
	init_delay(100);
	//蓝牙初始化
	BLUETEETH_Init();
	init_delay(100);
	//拉力传感器初始化
	Init_HX711pin();
	init_delay(100);
	//gps初始化
	GPS_Init();
	init_delay(100);
	//语音通信初始化
	SIM_USART_Init();
	init_delay(100);

	
}

void init_delay(vu32 count)
{
  count = SystemCoreClock / 8000 * count;
  while(count--);
}