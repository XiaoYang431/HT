//-----------------------------------------------------------------------------
#include "ht32.h"
#include "ht32_board.h"
#include "DELAY.h"
//#include "BMP73T104.h"
#include "Bizhang.h"
#include "Motor.h"
//-----------------------------------------------------------------------------
//  uint8_t a = 0;
//	  uint16_t b = 0;
//  uint8_t c = 0;
//	  uint8_t d = 0;
extern uint8_t Decode_count;
int main(void)
{
	//RETARGET_Configuration();//初始化电机时该模块必须使用           //Initialise the serial monitor with a baud rate of 115200
	//BMP73T104_Init();                   //Initialise module
		//BMP73T104_begin(0x0f,0x00,BMP73T104_HALF_STEP);

			/*************************************近接感应模块*********************/

   //BiZhang_Init(); 
  //Bihang_learning();
		/*************************************近接感应模块*********************/
			/*************************************电机模块*********************/		//RETARGET_Configuration();//初始化电机时该模块必须使用           //Initialise the serial monitor with a baud rate of 115200
	//BMP73T104_Init();                   //Initialise module
		//BMP73T104_begin(0x0f,0x00,BMP73T104_HALF_STEP);

// Motor_Init();

// Motor_Run(Motor_Left_up,60);
//	Motor_Run(Motor_Left_down,60);
//	Motor_Run(Motor_Right_up,-60);
//	Motor_Run(Motor_Right_down, -60);

			/*************************************电机模块*********************/
RETARGET_Configuration();
Set_Motor_decode2();
 Motor_Get_decode_TMInit();

	while(1)
	{
		//BMP73T104_dcMotorRun(BMP73T104_MOTOR1,70);
		////BMP73T104_dcMotorRun(BMP73T104_MOTOR2,60);
		//Delay_ms(5000);
		//BMP73T104_dcMotorRun(BMP73T104_MOTOR1,-60);
		////BMP73T104_dcMotorRun(BMP73T104_MOTOR2,-80);
		//Delay_ms(5000);
		/*************************************近接感应模块*********************/
//			a=  BM32S2031_1_getIRStatus();//是否靠近,返回1是靠近
//b  = BM32S2031_1_readIRSignalValue();//获得查值
//		c =_BM32S2031_1_readIRRef();
//		d = _BM32S2031_1_readIREnv();
				/*************************************近接感应模块*********************/

	}

}
