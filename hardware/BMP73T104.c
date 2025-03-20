/*********************************************************************************************************//**
 * @file    BMP73T104_HT32/src/BMP73T104.c
 * @version V1.0.1
 * @date    2024-08-06
 * @brief   The function of BMP73T104 driver.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/

/* Includes ------------------------------------------------------------------------------------------------*/
#include "BMP73T104.h"

/* Settings ------------------------------------------------------------------------------------------------*/
/* Private types -------------------------------------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------------------------------------*/
/* Global variables ----------------------------------------------------------------------------------------*/
uint32_t gBMP73T104_WIRE = BMP73T104_WIRE;

int8_t _dcMotorPosition[5] = {0};
uint16_t _stepperMaxSpeed[3] = {0};
uint16_t _stepperAccel[3] = {0};
 
/* Private variables ---------------------------------------------------------------------------------------*/
/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief 	module wire number select.
 * @param 	wire_number: select wire number.
 * @retval 	BMP73T104_SUCCESS/BMP73T104_FAILURE
 ************************************************************************************************************/
BMP73T104_selStatus BMP73T104_selWire(uint32_t wire_number)
{
  if(CHECK_WIREn(wire_number) == -1)
  {
    return BMP73T104_FAILURE;
  }

  gBMP73T104_WIRE = wire_number;
  return BMP73T104_SUCCESS;
}

/*********************************************************************************************************//**
 * @brief  Module initialization using I2C communication.
 * @param  void
 * @retval void
 ************************************************************************************************************/
void BMP73T104_Init(void)
{
	CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.BMP73T104_INTPIN_GPIO = 1;
  CKCUClock.Bit.AFIO       = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
		
	I2CMaster_Init(gBMP73T104_WIRE, BMP73T104_MASTERADDR, BMP73T104_CLKFREQ);
	 
	AFIO_GPxConfig(BMP73T104_INTPIN_GPIO_ID, BMP73T104_INTPIN_AFIO_PIN, AFIO_FUN_GPIO);
	GPIO_DirectionConfig(BMP73T104_INTPIN_GPIO_PORT, BMP73T104_INTPIN_GPIO_PIN, GPIO_DIR_IN);
	GPIO_PullResistorConfig(BMP73T104_INTPIN_GPIO_PORT, BMP73T104_INTPIN_GPIO_PIN, GPIO_PR_UP);
	GPIO_InputConfig(BMP73T104_INTPIN_GPIO_PORT, BMP73T104_INTPIN_GPIO_PIN, ENABLE);
}
/*********************************************************************************************************//**
 * @brief  	Initialize function, set motor interface function
 * @param  	dcMotor:Choose the DC motor interface to use
					  @arg bit1:1,Use DC motor interface 1 0,Not using DC motor interface 1
						@arg bit2:1,Use DC motor interface 2 0,Not using DC motor interface 2
						@arg bit3:1,Use DC motor interface 3 0,Not using DC motor interface 3
						@arg bit4:1,Use DC motor interface 4 0,Not using DC motor interface 4
						stepper:Choose the stepper motor interface to use
						@arg 0x03:Use stepper motor interface 1
						@arg 0x0c:Use stepper motor interface 2
						@arg 0xff:Use stepper motor interface 1 and 2
						interface:Choose the stepper motor drive mode
						@arg 0x00:FULL_STEP
						@arg 0x01:HALF_STEP
						@arg 0x02:MICRO_STEP
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_begin(uint8_t dcMotor,uint8_t stepper,uint8_t interface)
{
	uint8_t txData[7] = {0x55,0x73,0x04,BMP73T104_IIC_CMD_BEGIN,0x00,0x00,0x00};
	uint8_t rxData[20] = {0};
	
	if((stepper & 0x03) == 0x03)
	{
		if((dcMotor&0x01) == 0x01)
		{
			return 1;
		}
		if((dcMotor&0x02) == 0x02)
		{
			return 1;
		}
	}
	if((stepper & 0x0c) == 0x0c)
	{
		if((dcMotor&0x04) == 0x04)
		{
			return 1;
		}
		if((dcMotor&0x08) == 0x08)
		{
			return 1;
		}
	}
	
	if((stepper & 0x03) == 0x03)											//stepper1
	{
		_BMP73T104_delay(2);
		txData[4] = 0x03;
		txData[5] = interface;
		txData[6] = ~(0x55+0x73+0x04+txData[3]+txData[4]+txData[5]);
		BMP73T104_writeBytes(txData,7);
     
		_BMP73T104_delay(2);
		if(BMP73T104_readBytes(rxData,6) != 0)
		{
			return 1;
		}
	}
	if((stepper & 0x0C) == 0x0C)											//stepper2
	{
		_BMP73T104_delay(2);
		txData[4] = 0x0c;
		txData[5] = interface;
		txData[6] = ~(0x55+0x73+0x04+txData[3]+txData[4]+txData[5]);
		BMP73T104_writeBytes(txData,7);
     
		_BMP73T104_delay(2);
		if(BMP73T104_readBytes(rxData,6) != 0)
		{
			return 1;
		}
	}
	if((dcMotor&0x01) == 0x01)												//dcMotor1
	{
		_BMP73T104_delay(2);
		txData[4] = 0x01;
		txData[5] = 0;
		txData[6] = ~(0x55+0x73+0x04+txData[3]+txData[4]+txData[5]);
		BMP73T104_writeBytes(txData,7);
     
		_BMP73T104_delay(2);
		if(BMP73T104_readBytes(rxData,6) != 0)
		{
			return 1;
		}
	}
	if((dcMotor&0x02) == 0x02)												//dcMotor2
	{
		_BMP73T104_delay(2);
		txData[4] = 0x02;
		txData[5] = 0;
		txData[6] = ~(0x55+0x73+0x04+txData[3]+txData[4]+txData[5]);
		BMP73T104_writeBytes(txData,7);
     
		_BMP73T104_delay(2);
		if(BMP73T104_readBytes(rxData,6) != 0)
		{
			return 1;
		}
	}
	if((dcMotor&0x04) == 0x04)												//dcMotor3
	{
		_BMP73T104_delay(2);
		txData[4] = 0x04;
		txData[5] = 0;
		txData[6] = ~(0x55+0x73+0x04+txData[3]+txData[4]+txData[5]);
		BMP73T104_writeBytes(txData,7);
     
		_BMP73T104_delay(2);
		if(BMP73T104_readBytes(rxData,6) != 0)
		{
			return 1;
		}
	}
	if((dcMotor&0x08) == 0x08)													//dcMotor4
	{
		_BMP73T104_delay(2);
		txData[4] = 0x08;
		txData[5] = 0;
		txData[6] = ~(0x55+0x73+0x04+txData[3]+txData[4]+txData[5]);
		BMP73T104_writeBytes(txData,7);
     
		_BMP73T104_delay(2);
		if(BMP73T104_readBytes(rxData,6) != 0)
		{
			return 1;
		}
	}
	return 0;
}
/*********************************************************************************************************//**
 * @brief  	Set overcurrent protection ADC value
 * @param  	motor:DC motor interface set
						@arg 0x01:DC motor interface 1
						@arg 0x02:DC motor interface 2
						@arg 0x04:DC motor interface 3
						@arg 0x08:DC motor interface 4
            ocp:(Iset*0.05/Vioref)*4095
						debounce:Prevent shaking,time = debounce * 5ms
						status:Switch,ENABLE open DISENABLE close
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_setOCP(uint8_t motor,uint8_t ocp,uint8_t debounce,uint8_t status)
{
	uint8_t txData[9] = {0x55,0x73,0x06, 0x00,0x00,0x00,0x00,0x00,0x00};
  uint8_t rxData[20] = {0};
 
	txData[3] = BMP73T104_IIC_CMD_SET_OCP;
	txData[4] = motor;
	txData[5] = status;
	txData[6] = ocp;
	txData[7] = debounce;
	
  _BMP73T104_delay(2);
  txData[8] = ~(0x55+0x73+0x06+ txData[3]+txData[4]+txData[5]+txData[6]+txData[7]);
  BMP73T104_writeBytes(txData,9);
    
  _BMP73T104_delay(2);
  if(BMP73T104_readBytes(rxData,6) != 0)
  {
    return 1;
  }
  if(rxData[3] ==  BMP73T104_IIC_CMD_SET_OCP)
  {
    return 0;  
  }
  return 1;
}
/*********************************************************************************************************//**
 * @brief  	Clear overcurrent status
 * @param  	void
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_clearOverCurrent(void)
{
	uint8_t txData[5] = {0x55,0x73,0x02, BMP73T104_IIC_CMD_CLEAR_OCP_STU,0x00};
	uint8_t rxData[20] = {0};

	_BMP73T104_delay(2);
	txData[4] = ~(0x55+0x73+0x02+txData[3]);
	BMP73T104_writeBytes(txData,5);

	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,6) != 0)
	{
		return 1;
	}
	if(rxData[3] == BMP73T104_IIC_CMD_CLEAR_OCP_STU)
	{
		return 0;  
	}
	return 1; 	
} 
/*********************************************************************************************************//**
 * @brief  	Rotating DC motor  
 * @param  	motor:DC motor interface set
						@arg 0x01:DC motor interface 1
						@arg 0x02:DC motor interface 2
						@arg 0x04:DC motor interface 3
						@arg 0x08:DC motor interface 4
						rank:Speed gear, range - 100 to + 100
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_dcMotorRun(uint8_t motor,int8_t rank)
{   
	uint8_t txData[7] = {0x55,0x73,0x04,0x00,0x00,0x00,0x00};
	uint8_t rxData[20] = {0};
	
	if (rank > 100)
	{
		rank = 100;
	}
	else if (rank < -100) 
	{
		rank = -100;
	}       

	_BMP73T104_delay(2);
	
	txData[3] = BMP73T104_IIC_CMD_DCMOTOR_RUN;
	txData[4] = motor;
	txData[5] = rank;
	txData[6] = ~(0x55+0x73+0x04+txData[3]+txData[4]+txData[5]);
	BMP73T104_writeBytes(txData,7);
	
	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,6) != 0)
	{
		return 1;
	}
	if(rxData[3] == BMP73T104_IIC_CMD_DCMOTOR_RUN)
	{
		switch(motor)
		{
			case 0x01:_dcMotorPosition[1] = rank;break;
			case 0x02:_dcMotorPosition[2] = rank;break;
			case 0x04:_dcMotorPosition[3] = rank;break;
			case 0x08:_dcMotorPosition[4] = rank;break;
			default:break;
		}
		return 0;  
	}
	return 1;
}

/*********************************************************************************************************//**
 * @brief  	DC motor enter standby mode  
 * @param  	motor:DC motor interface set
						@arg 0x01:DC motor interface 1
						@arg 0x02:DC motor interface 2
						@arg 0x04:DC motor interface 3
						@arg 0x08:DC motor interface 4
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_dcMotorStop(uint8_t motor)
{
	uint8_t txData[6] = {0x55,0x73,0x03,0x00,0x00,0x00};
	uint8_t rxData[20] = {0};
	
	txData[3] = BMP73T104_IIC_CMD_DCMOTOR_STOP;
	txData[4] = motor;
	
	_BMP73T104_delay(2);
	txData[5] = ~(0x55+0x73+0x03+BMP73T104_IIC_CMD_DCMOTOR_STOP+motor);
	BMP73T104_writeBytes(txData,6);
	
	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,6) != 0)
	{
		return 1;
	}
	if(rxData[3] == BMP73T104_IIC_CMD_DCMOTOR_STOP)
	{
		return 0;  
	}
	_dcMotorPosition[motor] = 0;
	return 1;     
}
/*********************************************************************************************************//**
 * @brief  	DC motor stop stall  
 * @param  	motor:DC motor interface set
						@arg 0x01:DC motor interface 1
						@arg 0x02:DC motor interface 2
						@arg 0x04:DC motor interface 3
						@arg 0x08:DC motor interface 4
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_dcMotorBrake(uint8_t motor)
{
	uint8_t txData[6] = {0x55,0x73,0x03,0x00,0x00,0x00};
	uint8_t rxData[20] = {0};
		
	txData[3] = BMP73T104_IIC_CMD_DCMOTOR_BRAKE;
	txData[4] = motor;
		
	_BMP73T104_delay(2);
	txData[5] = ~(0x55+0x73+0x03+BMP73T104_IIC_CMD_DCMOTOR_BRAKE+motor);
	BMP73T104_writeBytes(txData,6);
    
	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,6) != 0)
	{
		return 1;
	}
	if(rxData[3] == BMP73T104_IIC_CMD_DCMOTOR_BRAKE)
	{
		return 0;  
	}
	_dcMotorPosition[motor] = 0;
	return 1;
}
/*********************************************************************************************************//**
 * @brief  	get INI Status
 * @param  	void
 * @retval 	Overcurrent signal
						@arg  0:overcurrent
						@arg  1:No overcurrent
 ************************************************************************************************************/
uint8_t BMP73T104_getINT(void)
{
	return (GPIO_ReadInBit(BMP73T104_INTPIN_GPIO_PORT, BMP73T104_INTPIN_GPIO_PIN));
}
/*********************************************************************************************************//**
 * @brief  	Get DC motor speed 
 * @param  	motor:DC motor interface set
						@arg 0x01:DC motor interface 1
						@arg 0x02:DC motor interface 2
						@arg 0x04:DC motor interface 3
						@arg 0x08:DC motor interface 4
 * @retval 	Rank
 ************************************************************************************************************/
int8_t BMP73T104_getDcMotorRank(uint8_t motor)
{
	switch(motor)
	{
		case 0x01:return _dcMotorPosition[1];
		case 0x02:return _dcMotorPosition[2];
		case 0x04:return _dcMotorPosition[3];
		case 0x08:return _dcMotorPosition[4];
		default:break;
	}
	return 0;
} 
/*********************************************************************************************************//**
 * @brief  	Turn to the target position at a constant speed
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
						absolute:Absolute position  
						speed:the speed of the stepper motor
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_stepperUniformSpeedMoveTo(uint8_t stepper,int32_t absolute, uint16_t speed)
{
	uint8_t txData[12] = {0x55,0x73,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	uint8_t rxData[20] = {0};
	
	_BMP73T104_delay(2);
	
	txData[3] = BMP73T104_IIC_CMD_MOVETO_S;
	txData[4] = stepper;
	txData[5] = (absolute>>24)&0xff;
	txData[6] = (absolute>>16)&0xff;
	txData[7] = (absolute>>8)&0xff;
	txData[8] = absolute&0xff;
	txData[9] = (speed>>8)&0xff;
	txData[10] = speed&0xff;
	txData[11] = ~(txData[0]+txData[1]+txData[2]+txData[3]+txData[4]+txData[5]+txData[6]+txData[7]+txData[8]+txData[9]+txData[10]);
	
	BMP73T104_writeBytes(txData,12);
	
	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,6) != 0)
	{
		return 1;
	}
	if(rxData[3] == BMP73T104_IIC_CMD_MOVETO_S)
	{
		return 0;  
	}
	return 1; 
}
/*********************************************************************************************************//**
 * @brief  	Rotate to the target position with acceleration
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
						absolute:Absolute position  
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_stepperUnevenSpeedMoveTo(uint8_t stepper,int32_t absolute)
{
	uint8_t txData[10] = {0x55,0x73,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	uint8_t rxData[20] = {0};
	
	_BMP73T104_delay(2);

	txData[3] = BMP73T104_IIC_CMD_MOVETO_A;
	txData[4] = stepper;
	txData[5] = (absolute>>24)&0xff;
	txData[6] = (absolute>>16)&0xff;
	txData[7] = (absolute>>8)&0xff;
	txData[8] = absolute&0xff;
	txData[9] = ~(0x55+0x73+0x07+txData[3]+txData[4]+txData[5]+txData[6]+txData[7]+txData[8]);
	BMP73T104_writeBytes(txData,10);
	
	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,6) != 0)
	{
		return 1;
	}
	if(rxData[3] == BMP73T104_IIC_CMD_MOVETO_A)
	{
		return 0;  
	}
	return 1;  
}
/*********************************************************************************************************//**
 * @brief  	Rotate with acceleration for a certain number of steps
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
						relative:Motor rotation steps, positive indicates clockwise, negative indicates counterclockwise 
						speed:the speed of the stepper motor,unit step/s
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_stepperUniformSpeedMove(uint8_t stepper,int32_t relative, uint16_t speed)
{
	uint8_t txData[12] = {0x55,0x73,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	uint8_t rxData[20] = {0};
	
	_BMP73T104_delay(2);

	txData[3] = BMP73T104_IIC_CMD_MOVE_S;
	txData[4] = stepper;
	txData[5] = (relative>>24)&0xff;   
	txData[6] = (relative>>16)&0xff;
	txData[7] = (relative>>8)&0xff;
	txData[8] = relative&0xff;
	txData[9] = (speed>>8)&0xff;
	txData[10] = speed&0xff;;
	txData[11] = ~(txData[0]+txData[1]+txData[2]+txData[3]+txData[4]+txData[5]+txData[6]+txData[7]+txData[8]+txData[9]+txData[10]);
	
	BMP73T104_writeBytes(txData,12);
	
	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,6) != 0)
	{
		return 1;
	}
	if(rxData[3] == BMP73T104_IIC_CMD_MOVE_S)
	{
		return 0;  
	}
	return 1; 
}
/*********************************************************************************************************//**
 * @brief  	Number of steps of a rotating target with acceleration
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
						relative:Motor rotation steps, positive indicates clockwise, negative indicates counterclockwise 
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_stepperUnevenSpeedMove(uint8_t stepper,int32_t relative)
{
	uint8_t txData[10] = {0x55,0x73,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	uint8_t rxData[20] = {0};
	
	_BMP73T104_delay(2);

	txData[3] = BMP73T104_IIC_CMD_MOVE_A;
	txData[4] = stepper;
	txData[5] = (relative>>24)&0xff;  
	txData[6] = (relative>>16)&0xff;
	txData[7] = (relative>>8)&0xff;
	txData[8] = relative&0xff;
	txData[9] = ~(0x55+0x73+0x07+txData[3]+txData[4]+txData[5]+txData[6]+txData[7]+txData[8]);
	BMP73T104_writeBytes(txData,10);
	
	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,6) != 0)
	{
		return 1;
	}
	if(rxData[3] == BMP73T104_IIC_CMD_MOVE_A)
	{
		return 0;  
	}
	return 1; 
}
/*********************************************************************************************************//**
 * @brief  	The stepper motor rotates continuously at a constant speed 
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
						dir:Stepper motor direction
						@arg 0x00:clockwise
						@arg 0x01:anticlockwise
            FORWARD = 1 indicates clockwise rotation and REVERSAL = 0 indicates counterclockwise rotation  
            speed:the speed of the stepper motor,unit step/s
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_stepperRun(uint8_t stepper,uint8_t dir,uint16_t speed)
{
	uint8_t txData[9] = {0x55,0x73,0x06,0x00,0x00,0x00,0x00,0x00,0x00};
	uint8_t rxData[20] = {0};
	
	_BMP73T104_delay(2);

	txData[3] = BMP73T104_IIC_CMD_STEP_RUN;
	txData[4] = stepper;
	txData[5] = dir;
	txData[6] = (speed>>8)&0xff;
	txData[7] = speed&0xff;

	txData[8] = ~(txData[0]+txData[1]+txData[2]+txData[3]+txData[4]+txData[5]+txData[6]+txData[7]);
	BMP73T104_writeBytes(txData,9);
	
	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,6) != 0)
	{
		return 1;
	}
	if(rxData[3] == BMP73T104_IIC_CMD_STEP_RUN)
	{
		return 0;  
	}
	return 1;   
}
/*********************************************************************************************************//**
 * @brief  	Stepper motor enters stop mode 
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_stepperStop(uint8_t stepper)
{
	uint8_t txData[6] = {0x55,0x73,0x03,0x00,0x00,0x00};
	uint8_t rxData[20] = {0};
	
	_BMP73T104_delay(2);
	
	txData[3] = BMP73T104_IIC_CMD_STEP_STOP;
	txData[4] = stepper;
	txData[5] = ~(0x55+0x73+0x03 + txData[3] + txData[4]);
	BMP73T104_writeBytes(txData,6);
	
	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,6) != 0)
	{
		return 1;
	}
	if(rxData[3] == BMP73T104_IIC_CMD_STEP_STOP)
	{
		return 0;  
	}
	return 1;   
      
}
/*********************************************************************************************************//**
 * @brief  	Release all pins of the stepper motor so it free-spins.
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_stepperRelease (uint8_t stepper)
{
	uint8_t txData[6] = {0x55,0x73,0x03,0x00,0x00,0x00};
	uint8_t rxData[20] = {0};
	
	_BMP73T104_delay(2);
	
	txData[3] = BMP73T104_IIC_CMD_STEP_STOP;
	txData[4] = stepper;
	txData[5] = ~(0x55+0x73+0x03 + txData[3] + txData[4]);
	BMP73T104_writeBytes(txData,6);
	
	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,6) != 0)
	{
		return 1;
	}
	if(rxData[3] == BMP73T104_IIC_CMD_STEP_STOP)
	{
		return 0;  
	}
	return 1;   
}
/*********************************************************************************************************//**
 * @brief  	Check whether the motor is running towards the target     
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
 * @retval	true:Arrived  false:Not arrived
 ************************************************************************************************************/
uint8_t BMP73T104_isRunning(uint8_t stepper)
{
	uint8_t txData[6] = {0x55,0x73,0x03,0x00,0x00,0x00};
	uint8_t isR = 1;
	uint8_t rxData[20] = {0};

	_BMP73T104_delay(2);
	
	txData[3] = BMP73T104_IIC_CMD_RUNNING;
	txData[4] = stepper;
	txData[5] = ~(0x55+0x73+0x03+txData[3]+txData[4]);
	BMP73T104_writeBytes(txData,6);
	
	_BMP73T104_delay(2);
	BMP73T104_readBytes(rxData,6);
	if(rxData[3] == BMP73T104_IIC_CMD_RUNNING)
	{
		 isR = rxData[4];
	}
	return isR;
}
/*********************************************************************************************************//**
 * @brief  	Get maximum speed
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
 * @retval	Maximum speed of stepper motor,unit step/s
 ************************************************************************************************************/
uint16_t BMP73T104_getStepperMaxSpeed(uint8_t stepper)
{
	if(stepper == 0x03)
	{
		return _stepperMaxSpeed[1];
	}
	else if(stepper == 0x0c)
	{
		return _stepperMaxSpeed[2];
	}
	return 0;
}  
/*********************************************************************************************************//**
 * @brief  	Get current speed
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
 * @retval	Speed of stepper motor,unit step/s
 ************************************************************************************************************/
uint16_t BMP73T104_getStepperSpeed(uint8_t stepper)
{
	uint8_t txData[6] = {0x55,0x73,0x03,0x00,0x00,0x00};
	uint16_t Speed_Data = 0;
	uint8_t rxData[20] = {0};

	_BMP73T104_delay(2);
	
	txData[3] = BMP73T104_IIC_CMD_GET_SPEED;
	txData[4] = stepper;
	txData[5] = ~(0x55+0x73+0x03+txData[3]+txData[4]);
	BMP73T104_writeBytes(txData,6);
	
	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,7) != 0)
	{
		return 0;
	}

	if(rxData[3] == BMP73T104_IIC_CMD_GET_SPEED)
	{
		 Speed_Data = rxData[4];
		 Speed_Data = (Speed_Data<<8) + rxData[5];
	}
	if(Speed_Data == 0xffff)
	{
		Speed_Data = 0;
	}
	return Speed_Data;
}
/*********************************************************************************************************//**
 * @brief  	Get acceleration 
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
 * @retval	Stepper motor acceleratio,unit step/s2
 ************************************************************************************************************/
uint16_t BMP73T104_getStepperAcceleration(uint8_t stepper)
{
	if(stepper == 0x03)
	{
		return _stepperAccel[1];
	}
	else if(stepper == 0x0c)
	{
		return _stepperAccel[2];
	}
	return 0;
}
 
/*********************************************************************************************************//**
 * @brief  	Get target location
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
 * @retval	Target position of stepper motor
 ************************************************************************************************************/
int32_t BMP73T104_getStepperTargetPosition(uint8_t stepper)
{
	uint8_t txData[6] = {0x55,0x73,0x03,BMP73T104_IIC_CMD_GET_T_POSITION,0x00,0x00};
	uint32_t TargetPosition=0;
	uint8_t rxData[20] = {0};
	uint8_t i = 0;

	_BMP73T104_delay(2);
	
	txData[3] = BMP73T104_IIC_CMD_GET_T_POSITION;
	txData[4] = stepper;
	txData[5] = ~(0x55+0x73+0x03+txData[3]+txData[4]);
	BMP73T104_writeBytes(txData,6);
	
	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,9) != 0)
	{
		return 0;
	}
	if(rxData[3] == BMP73T104_IIC_CMD_GET_T_POSITION)
	{
		for(i = 4; i < 8; i++)
		{
			TargetPosition = TargetPosition<<8;
			TargetPosition = TargetPosition + rxData[i];
		}
	}
	return (int32_t)TargetPosition;
} 
/*********************************************************************************************************//**
 * @brief  	Gets the distance between the current position and the target position   
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
 * @retval	The distance between the current position and the target position   
 ************************************************************************************************************/
int32_t BMP73T104_getStepperDistanceToGo(uint8_t stepper)
{
	uint8_t txData[6] = {0x55,0x73,0x03,0x00,0x00,0x00};
	uint32_t go=0;
	uint8_t rxData[20] = {0};
	uint8_t i = 0;

	_BMP73T104_delay(2);
	
	txData[3] = BMP73T104_IIC_CMD_GET_STEP_TOGO;
	txData[4] = stepper;
	txData[5] = ~(0x55+0x73+0x03+txData[3]+txData[4]);
	BMP73T104_writeBytes(txData,6);
	
	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,9) != 0)
	{
		return 0;
	}
	if(rxData[3] == BMP73T104_IIC_CMD_GET_STEP_TOGO)
	{
		for(i = 4; i < 8; i++)
		{
			go = go<<8;
			go = go + rxData[i];
		}
	}
	return (int32_t)go;
}
/*********************************************************************************************************//**
 * @brief  	Get current location     
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
 * @retval	Current position 
 ************************************************************************************************************/
int32_t BMP73T104_getStepperPosition(uint8_t stepper)
{
	uint8_t txData[6] = {0x55,0x73,0x03,0x00,0x00,0x00};
	uint32_t Position=0; 
	uint8_t rxData[20] = {0};
	uint8_t i = 0;
 
	_BMP73T104_delay(2);
	
	txData[3] = BMP73T104_IIC_CMD_GET_POSITION;
	txData[4] = stepper;
	txData[5] = ~(0x55+0x73+0x03+txData[3]+txData[4]);
	BMP73T104_writeBytes(txData,6);
	
	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,9) != 0)
	{
		return 0;
	}
	if(rxData[3] == BMP73T104_IIC_CMD_GET_POSITION)
	{
		for(i = 4; i < 8; i++)
		{
			Position = Position<<8;
			Position = Position + rxData[i];
		}
	}
	return (int32_t)Position; 
}
/*********************************************************************************************************//**
 * @brief  	Sets the maximum speed of the stepper motor 
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
						maxSpeed:The maximum speed of the stepper motor Steps per second
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_setStepperMaxSpeed(uint8_t stepper,uint16_t maxSpeed)
{
    uint8_t txData[8] = {0x55,0x73,0x05,0x00,0x00,0x00,0x00,0x00};
    uint8_t rxData[20] = {0};
    
    _BMP73T104_delay(2);
		
		txData[3] = BMP73T104_IIC_CMD_SET_MAXSPEED;
		txData[4] = stepper;
    txData[5] = (maxSpeed>>8)&0xff;  
    txData[6] = maxSpeed&0xff;
    txData[7] = ~(0x55+0x73+0x05+txData[3]+txData[4]+txData[5]+txData[6]);
    BMP73T104_writeBytes(txData,8);
    
    _BMP73T104_delay(2);
    if(BMP73T104_readBytes(rxData,6) != 0)
    {
      return 1;
    }
    if(rxData[3] == BMP73T104_IIC_CMD_SET_MAXSPEED)
    {
			if(stepper == 0x03)
			{
				_stepperMaxSpeed[1] = maxSpeed;
			}else if(stepper == 0x0c)
			{
				_stepperMaxSpeed[2] = maxSpeed;
			} 
      return 0;  
    }
    return 1; 
}
/*********************************************************************************************************//**
 * @brief  	Sets the acceleration of the stepper motor  
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
						acceleration:The acceleration of the stepper motor,unit step/s2
            Range 1~65535 , Step square per second
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_setStepperAcceleration(uint8_t stepper,uint16_t acceleration)
{
	uint8_t txData[8] = {0x55,0x73,0x05,BMP73T104_IIC_CMD_SET_ACCEL,0x00,0x00,0x00,0x00};
	uint8_t rxData[20] = {0};

	_BMP73T104_delay(2);
	
	txData[3] = BMP73T104_IIC_CMD_SET_ACCEL;
	txData[4] = stepper;
	txData[5] = (acceleration>>8)&0xff;   
	txData[6] = acceleration&0xff;
	txData[7] = ~(0x55+0x73+0x05+txData[3]+txData[4]+txData[5]+txData[6]);
	BMP73T104_writeBytes(txData,8);
	
	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,6) != 0)
	{
		return 1;
	}
	if(rxData[3] == BMP73T104_IIC_CMD_SET_ACCEL)
	{
		if(stepper == 0x03)
		{
			_stepperAccel[1] = acceleration;
		}else if(stepper == 0x0c)
		{
			_stepperAccel[2] = acceleration;
		}
		return 0;  
	}
	return 1; 
}
/*********************************************************************************************************//**
 * @brief  	Set the current position of the stepper motor
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
						position:The current position of the stepper motor 
            Range -2^31 ~ +2^31
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_setStepperCurrentPosition(uint8_t stepper,int32_t position)
{
	uint8_t txData[10] = {0x55,0x73,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	uint8_t rxData[20] = {0};
	
	_BMP73T104_delay(2);
			
	txData[3] = BMP73T104_IIC_CMD_SET_POSITION;
	txData[4] = stepper;
	txData[5] = (position>>24)&0xff;
	txData[6] = (position>>16)&0xff;
	txData[7] = (position>>8)&0xff;
	txData[8] = position&0xff;
	txData[9] = ~(0x55+0x73+0x07+txData[3]+txData[4]+txData[5]+txData[6]+txData[7]+txData[8]);
	 
	BMP73T104_writeBytes(txData,10);
	
	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,6) != 0)
	{
		return 1;
	}
	if(rxData[3] == BMP73T104_IIC_CMD_SET_POSITION)
	{
		return 0;  
	}
	return 1; 
}
/*********************************************************************************************************//**
 * @brief  	Set stepper motor holding torque 
 * @param  	stepper:Selected stepper motor interface
						@arg 0x03:stepper1
						@arg 0x0c:stepper2
						status:ENABLE or DISABLE 
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_setStepperHoldTorque(uint8_t stepper,uint8_t status)
{
	uint8_t txData[7] = {0x55,0x73,0x04,0x00,0x00,0x00,0x00};
	uint8_t rxData[20] = {0};

	_BMP73T104_delay(2);
	
	txData[3] = BMP73T104_IIC_CMD_HOLD_TORQUE;
	txData[4] = stepper;
	txData[5] = status;
	txData[6] = ~(0x55+0x73+0x04+txData[3]+txData[4]+txData[5]);
	BMP73T104_writeBytes(txData,7);
	
	_BMP73T104_delay(2);
	if(BMP73T104_readBytes(rxData,6) != 0)
	{
		return 1;
	}
	if(rxData[3] == BMP73T104_IIC_CMD_HOLD_TORQUE)
	{
		return 0;  
	}
	return 1;  
}
/*********************************************************************************************************//**
 * @brief  	change IIC slave address 
 * @param  	addr:slave address 0~127
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
uint8_t BMP73T104_setIICSlaveAddr(uint8_t i2c_addr)
{
	uint8_t txData[6] = {0x55,0x73,0x03,BMP73T104_IIC_CMD_SET_IIC_ADDR,0x00,0x00};
	uint8_t rxData[20] = {0};
	uint8_t i = 0;

	_BMP73T104_delay(100);
	for(i = 0; i < 128; i++)
	{
		txData[4] = i2c_addr;
		txData[5] = ~(0x55+0x73+0x03+BMP73T104_IIC_CMD_SET_IIC_ADDR+i2c_addr);
		BMP73T104_writeBytes(txData,6);
		_BMP73T104_delay(2);
	}
	_BMP73T104_delay(2);
 
	if(BMP73T104_readBytes(rxData,6) != 0)
	{
		return 1;
	}
 
	if(rxData[3] == BMP73T104_IIC_CMD_SET_IIC_ADDR)
	{
		return 0;  
	}
	return 1;
}
/*********************************************************************************************************//**
 * @brief  	Reset motor driver
 * @param  	void
 * @retval	result
						@arg 0:success
						@arg 1:fail
 ************************************************************************************************************/
void BMP73T104_reset(void)
{
	uint8_t txData[5] = {0x55,0x73,0x02,BMP73T104_IIC_CMD_RESET,0x00};
	uint8_t cmd_reset = BMP73T104_IIC_CMD_RESET;
	_BMP73T104_delay(2);
	txData[4] = ~(0x55+0x73+0x02+cmd_reset);
	BMP73T104_writeBytes(txData,5);
}
/*********************************************************************************************************//**
 * @brief  	Get version number 
 * @param  	void
 * @retval	0x0101:Version number 1.01
 ************************************************************************************************************/
uint16_t BMP73T104_getFWVer(void)
{
    uint8_t txData[5] = {0x55,0x73,0x02,0x00,0x00};
    uint16_t FWVer = 0;
		uint8_t rxData[20] = {0};
		
    _BMP73T104_delay(2);
		
		txData[3] = BMP73T104_IIC_CMD_GET_FWVER;
    txData[4] = ~(0x55+0x73+0x02+txData[3]);
    BMP73T104_writeBytes(txData,5);
    
    _BMP73T104_delay(2);
    if(BMP73T104_readBytes(rxData,7) != 0)
    {
			return 0;
    }
    if(rxData[3] == BMP73T104_IIC_CMD_GET_FWVER)
    {
      FWVer = rxData[4];
      FWVer = (FWVer<<8) + rxData[5];
    }
    return FWVer;
} 
/*********************************************************************************************************//**
 * @brief  write data through IIC
 * @param  wbuf:Variables for storing Data to be sent
 * @param  wlen:Length of data sent
 * @retval void  
 ************************************************************************************************************/
void BMP73T104_writeBytes(uint8_t wbuf[], uint8_t wlen)
{
	I2CMaster_Status_Enum I2CResult = I2C_MASTER_GOING;
	I2CMaster_Typedef gI2CMaster = {0x00};
	
	gI2CMaster.uSlaveAddress = BMP73T104_MASTERADDR;
	gI2CMaster.Tx.puBuffer = (uint8_t *)wbuf;
	gI2CMaster.Tx.uLength = wlen;
	gI2CMaster.uTimeout_us = 30000;
	gI2CMaster.isDynamicOnOff_I2C = FALSE;

	I2CResult = I2C_MASTER_GOING;
	I2CMaster_Write(gBMP73T104_WIRE,&gI2CMaster);
	do {
		I2CResult = I2CMaster_GetStatus(gBMP73T104_WIRE);
	} while (I2CResult == I2C_MASTER_GOING);
	_BMP73T104_delay(10);
}
/*********************************************************************************************************//**
 * @brief  read data through IIC
 * @param  rbuf:Variables for storing Data to be obtained
 * @param  rlen:Length of data to be obtained
 * @retval BMP73T104_READ_OK/BMP73T104_READ_ERROR/BMP73T104_NO_ACK/BMP73T104_TIMEOUT_ERROR 
 ************************************************************************************************************/
uint8_t BMP73T104_readBytes(uint8_t rbuf[], uint8_t rlen)
{
	uint8_t i = 0;
	uint8_t checkSum = 0;

	I2CMaster_Status_Enum I2CResult = I2C_MASTER_GOING;
	I2CMaster_Typedef I2CMaster = { 0 };

	I2CMaster.uSlaveAddress = BMP73T104_MASTERADDR;
	I2CMaster.Rx.puBuffer = (uint8_t *)rbuf;
	I2CMaster.Rx.uLength = rlen;
	I2CMaster.uTimeout_us = 30000;
	I2CMaster.isDynamicOnOff_I2C = FALSE;

	I2CMaster_Read(gBMP73T104_WIRE, &I2CMaster);
	
	do {
		I2CResult = I2CMaster_GetStatus(gBMP73T104_WIRE);
	} while (I2CResult == I2C_MASTER_GOING);

	_BMP73T104_delay(1);
	for(i = 0;i < rlen - 1;i++)
	{
		checkSum = checkSum + rbuf[i];
	}
	checkSum = ~checkSum;
	if(rbuf[i] != checkSum)
	{
		return BMP73T104_READ_ERROR;
	}
	return BMP73T104_READ_OK;
}

/*********************************************************************************************************//**
 * @brief  delay ms.
 * @param  count
 * @retval void
 ************************************************************************************************************/
void _BMP73T104_delay(vu32 count)
{
  count = SystemCoreClock / 8000 * count;
  while(count--);
}
/* Private functions ---------------------------------------------------------------------------------------*/

