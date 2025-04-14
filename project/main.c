//-----------------------------------------------------------------------------
#include "ht32.h"
#include "ht32_board.h"
#include "BMP73T104.h"
#include "Bizhang.h"
#include "Motor.h"
#include "Kaerman.h"
//-----------------------------------------------------------------------------
//  uint8_t a = 0;
//	  uint16_t b = 0;
//  uint8_t c = 0;
//	  uint8_t d = 0;
int32_t Speed_left = 0,Speed_right = 0;
int32_t Turn = 0,Veloc = 0;
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
//初始化电机驱动
BMP73T104_Init();                   //Initialise module
Motor_Init();
//初始化编码器
Set_Motor_decode1();
Set_Motor_decode2();
//初始化定时器，采集编码器数值，200Hz
 Motor_Get_decode_TMInit();
//初始化陀螺仪
 while(BMS56M605_init());
//	  	  Motor_Run(Motor_Left_up,0);
//	Motor_Run(Motor_Left_down,0);
//	Motor_Run(Motor_Right_up,0);
//	Motor_Run(Motor_Right_down,0);
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
//b  = BM32S2031_1_readIRSignalValue();//获得差值
//		c =_BM32S2031_1_readIRRef();
//		d = _BM32S2031_1_readIREnv();
				/*************************************近接感应模块*********************/
     		//卡尔曼滤波计算角度值
	 Angle_Cal();
//	
//      if(BM32S2031_1_getIRStatus() == 1)
//      {
//      Turn = PID_Turn(gyro_z,encder_left,encder_right);
//       Velocity = velocity(1,encder_left,encder_right);
//           // _BM32S2031_1_delay(1000);
//      }
//      else
//      {
//          Velocity = velocity(1,encder_left,encder_right);
//      }
//      Speed_left = Velocity + Turn;
//      Speed_right = -(Velocity - Turn);
//	  Motor_Run(Motor_Left_up,Speed_left);
//	Motor_Run(Motor_Left_down,Speed_left);
//	Motor_Run(Motor_Right_up,Speed_right);
//	Motor_Run(Motor_Right_down, Speed_right);
//		Velocity = 0;
//		Turn = 0;
//	  _BMP73T104_delay(1000);
//	  	  Motor_Run(Motor_Left_up,Velocity);
//	Motor_Run(Motor_Left_down,Velocity);
//	Motor_Run(Motor_Right_up,-Velocity);
//	Motor_Run(Motor_Right_down,- Velocity);
//
//Veloc = velocity(0,encder_left,encder_right);
//	  	  Motor_Run(Motor_Left_up,Veloc);
//	Motor_Run(Motor_Left_down,Veloc);
//	Motor_Run(Motor_Right_up,-Veloc);
//	Motor_Run(Motor_Right_down,- Veloc);
	}

}
