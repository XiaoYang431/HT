//-----------------------------------------------------------------------------
#include "ht32.h"
#include "ht32_board.h"
#include "BMP73T104.h"
#include "GPS.h"
#include "BMS56M605.h"
#include "BM7701_00_1.h"
#include "Blueteech.h"
#include "motor.h"
#include "SIM900A.h"
#include "PUSH.h"
#include <math.h>
//-----------------------------------函数声明----------------------------------


void _delay(vu32 count);

//-----------------------------------------------------------------------------

//蓝牙连接状态

  uint8_t sampleLat[] = { 0x30 ,0x39, 0x30 ,0x36 ,0x34 ,0x32 ,0x2E ,0x30, 0x30, 0x30 ,0x32, 0x33 ,
	                                           //
	  
	  0x30 ,0x32,0x2E, 0x35, 0x32 ,0x32 ,0x33, 0x35,0x4E
   ,0x31, 0x31, 0x33 , 0x32, 0x33 ,0x2E,0x33 ,0x34, 0x35 ,0x30 ,0x38,0x45
};   // 示例纬度数据

extern uint8_t blueteech_avaible_flag;

 int32_t Speed_Turn = 0;
int32_t Turn = 0;
int16_t k =0 ;
static uint8_t jishu = 0;

extern _SaveData Save_Data;
extern _SaveData my_target;
extern  DirectionVector Dis;
float current_force=0;
float decent=0;
int main(void)
{
	RETARGET_Configuration();           //Initialise the serial monitor with a baud rate of 115200
	
	//全部初始化
	All_init();
	 
	 _delay(3000);
	 //while(Bihang_learning());
	 //Get_Maopi();
	//SIM900A_SendSms("17707592248", "test");
    //SIM900A_makeCall("17707592248");
	
	//等待所有模块初始化完毕后，再启用主循环
	
	while(1)
{
	decent=BMS56M605_readGyroscopeZ();
	parseGpsBuffer();
	//parseGpsBuffer();
	_delay(1000);
	GPS_To_BlueTooth();
	_delay(100);
	processBluetoothGPS(&my_target);
	_delay(100);
	Dis.dx=atof(my_target.latitude);
	Dis.dy=atof(my_target.longitude);
	Dis.isValid=true;
}
	
/*	
	while(1)
	{
	
	
	if(blueteech_avaible_flag==1)
	{	parseGpsBuffer();
		
		processBluetoothGPS( &my_target);
	}
	k= BM32S2031_1_getIRStatus();
	
	CalculateDirectionVector(Save_Data,  my_target) ;
	//float currentLat = atof(Save_Data.latitude);
    //float currentLon = atof(Save_Data.longitude);
    //float targetLat = atof(my_target.latitude);
    //float targetLon = atof(my_target.longitude);
	//
    //// 计算方向向量
    //Dis.dx = targetLon - currentLon; // 经度方向
    //Dis.dy = targetLat - currentLat; // 纬度方向
	//Dis.isValid = 1; // 设置为有效
	
	if(BM32S2031_1_getIRStatus() == 1)
     {
		
		jishu++;
	Motor_Run(Motor_Left_up,+80);
	Motor_Run(Motor_Left_down,-80);
	Motor_Run(Motor_Right_up,+80);
	Motor_Run(Motor_Right_down,-80);
		   _BMP73T104_delay(1000);
  
           Motor_Run(Motor_Left_up,+60);
	Motor_Run(Motor_Left_down,+60);
	Motor_Run(Motor_Right_up,-60);
	Motor_Run(Motor_Right_down,-60);
		  _BMP73T104_delay(1000);

     Motor_Run(Motor_Left_up,-80);
	Motor_Run(Motor_Left_down,+80);
	Motor_Run(Motor_Right_up,-80);
	Motor_Run(Motor_Right_down,+80);
		   _BMP73T104_delay(1000);
   
     }
	 else
	 {
		 Motor_Run(Motor_Left_up,-0);
	Motor_Run(Motor_Left_down,+0);
	Motor_Run(Motor_Right_up,-0);
	Motor_Run(Motor_Right_down,+0);
	 	 
		if(jishu != 0)
		{
			jishu = 0;
			
	      CalculateDirectionVector(Save_Data,  my_target) ;

		}
			if(  GPIO_ReadInBit(HT_GPIOB, GPIO_PIN_5) == RESET)
		{
			if(Dis.isValid)
			{
				// ?ж??????????????
				
					if(fabs(angle_error) < 0.052)
				{
				//???????????????????????3????
				Speed_Turn = 0;
				current_angle = 0;
				Dis.isValid = 0;
				angle_error = 5;
				}
					else
				{
				Speed_Turn =  PID_Turn(Dis.dx, Dis.dy);//dx,dy
				}
			}
	
		Motor_Run(Motor_Left_up,+Speed_Turn);
		Motor_Run(Motor_Left_down,-Speed_Turn);
		Motor_Run(Motor_Right_up,+Speed_Turn);
		Motor_Run(Motor_Right_down,-Speed_Turn);
		GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_5, GPIO_PR_UP);
	
		}																
	 }
   }*/	
}





void _delay(vu32 count)
{
  count = SystemCoreClock / 8000 * count;
  while(count--);
}
