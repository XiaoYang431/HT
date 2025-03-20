//-----------------------------------------------------------------------------
#include "ht32.h"
#include "ht32_board.h"
#include "DELAY.h"
#include "BMP73T104.h"
//-----------------------------------------------------------------------------
int main(void)
{
	RETARGET_Configuration();           //Initialise the serial monitor with a baud rate of 115200
	BMP73T104_Init();                   //Initialise module
	BMP73T104_begin(0x0f,0x00,BMP73T104_HALF_STEP);
	 
	while(1)
	{
		BMP73T104_dcMotorRun(BMP73T104_MOTOR1,70);
		//BMP73T104_dcMotorRun(BMP73T104_MOTOR2,60);
		Delay_ms(5000);
		BMP73T104_dcMotorRun(BMP73T104_MOTOR1,-60);
		//BMP73T104_dcMotorRun(BMP73T104_MOTOR2,-80);
		Delay_ms(5000);
	}

}
