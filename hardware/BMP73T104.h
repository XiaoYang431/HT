/*********************************************************************************************************//**
 * @file    BMP73T104_HT32/src/BMP73T104.h
 * @version V1.0.1
 * @date    2024-08-06
 * @brief   The header file of BMP73T104 driver.
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
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __BMP73T104_H
#define __BMP73T104_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"
#include "i2c_master_config.h"
#include "ht32_board_config.h"
#include "bmduino_check.h"

#include "middleware/i2c_master.h"

/* Settings ------------------------------------------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------------------------------------*/
typedef enum {
  BMP73T104_SUCCESS,
  BMP73T104_FAILURE
} BMP73T104_selStatus;
/* Exported constants --------------------------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------------------------------------*/
#define BMP73T104_MASTERADDR     0x19
#define BMP73T104_CLKFREQ        100000

#define BMP73T104_READ_OK          	0
#define BMP73T104_READ_ERROR       	1
#define BMP73T104_NO_ACK           	2
#define BMP73T104_TIMEOUT_ERROR    	3

#define BMP73T104_DISABLE         	0
#define BMP73T104_ENABLE          	1

#define   BMP73T104_R_SUCCESS      0
#define   BMP73T104_R_FAIL         1
#define   BMP73T104_REQUESTLEN1        1
#define   BMP73T104_REQUESTLEN16       16
#define   BMP73T104_REQUESTLEN18       18 

#define BMP73T104_MOTOR1	0x01
#define BMP73T104_MOTOR2	0x02
#define BMP73T104_MOTOR3	0x04
#define BMP73T104_MOTOR4	0x08

#define BMP73T104_STEPPER1	0x03
#define BMP73T104_STEPPER2	0x0C

#define BMP73T104_CW	0
#define	BMP73T104_CCW 1

#define BMP73T104_FULL_STEP  	(0)
#define BMP73T104_HALF_STEP  	(1)
#define BMP73T104_MICRO_STEP 	(2)

#define BMP73T104_IIC_CMD_BEGIN           (0x01)
#define BMP73T104_IIC_CMD_DCMOTOR_RUN     (0x02)
#define BMP73T104_IIC_CMD_DCMOTOR_BRAKE   (0x03)
#define BMP73T104_IIC_CMD_DCMOTOR_STOP    (0x04)
#define BMP73T104_IIC_CMD_SET_OCP         (0x05)
#define BMP73T104_IIC_CMD_SET_MAXSPEED    (0x06)
#define BMP73T104_IIC_CMD_SET_ACCEL       (0x07)
#define BMP73T104_IIC_CMD_SET_POSITION    (0x08)
#define BMP73T104_IIC_CMD_MOVETO_S        (0x09)
#define BMP73T104_IIC_CMD_MOVETO_A        (0x0A)
#define BMP73T104_IIC_CMD_MOVE_S          (0x0B)
#define BMP73T104_IIC_CMD_MOVE_A          (0x0C)
#define BMP73T104_IIC_CMD_STEP_RUN        (0x0D)
#define BMP73T104_IIC_CMD_STEP_STOP       (0x0E)
#define BMP73T104_IIC_CMD_RESET           (0x0F)
#define BMP73T104_IIC_CMD_RUNNING         (0x10)
#define BMP73T104_IIC_CMD_GET_SPEED       (0x11)
#define BMP73T104_IIC_CMD_GET_T_POSITION  (0x12)
#define BMP73T104_IIC_CMD_GET_STEP_TOGO   (0x13)
#define BMP73T104_IIC_CMD_GET_POSITION    (0x14)
#define BMP73T104_IIC_CMD_CLEAR_OCP_STU   (0x15)
#define BMP73T104_IIC_CMD_HOLD_TORQUE     (0x16)
#define BMP73T104_IIC_CMD_GET_ADC         (0x17)
#define BMP73T104_IIC_CMD_SET_IIC_ADDR    (0x18)
#define BMP73T104_IIC_CMD_GET_FWVER       (0X19)
/* Exported variables --------------------------------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------------------------------------*/
	BMP73T104_selStatus BMP73T104_selWire(uint32_t wire_number);
	void BMP73T104_Init(void);
	uint8_t BMP73T104_begin(uint8_t dcMotor,uint8_t stepper,uint8_t interface);
	uint8_t BMP73T104_setOCP(uint8_t motor,uint8_t ocp,uint8_t debounce,uint8_t status); 
	uint8_t BMP73T104_clearOverCurrent(void);
	uint8_t BMP73T104_dcMotorRun(uint8_t motor,int8_t rank);  
	uint8_t BMP73T104_dcMotorStop(uint8_t motor);    
	uint8_t BMP73T104_dcMotorBrake(uint8_t motor);
	uint8_t BMP73T104_getINT(void);
	int8_t BMP73T104_getDcMotorRank(uint8_t motor);
	uint8_t BMP73T104_stepperUniformSpeedMoveTo(uint8_t stepper,int32_t absolute, uint16_t speed);
	uint8_t BMP73T104_stepperUnevenSpeedMoveTo(uint8_t stepper,int32_t absolute);
	uint8_t BMP73T104_stepperUniformSpeedMove(uint8_t stepper,int32_t relative, uint16_t speed);
	uint8_t BMP73T104_stepperUnevenSpeedMove(uint8_t stepper,int32_t relative);
	uint8_t BMP73T104_stepperRun(uint8_t stepper,uint8_t dir, uint16_t speed);
	uint8_t BMP73T104_stepperStop(uint8_t stepper);
	uint8_t BMP73T104_stepperRelease(uint8_t stepper);
	uint8_t BMP73T104_isRunning(uint8_t stepper);
	uint16_t BMP73T104_getStepperMaxSpeed(uint8_t stepper);
	uint16_t BMP73T104_getStepperSpeed(uint8_t stepper);
	uint16_t BMP73T104_getStepperAcceleration(uint8_t stepper);
	int32_t BMP73T104_getStepperTargetPosition(uint8_t stepper);
	int32_t BMP73T104_getStepperDistanceToGo(uint8_t stepper);
	int32_t BMP73T104_getStepperPosition(uint8_t stepper);
	uint8_t BMP73T104_setStepperMaxSpeed(uint8_t stepper,uint16_t maxSpeed);
	uint8_t BMP73T104_setStepperAcceleration(uint8_t stepper,uint16_t acceleration);
	uint8_t BMP73T104_setStepperCurrentPosition(uint8_t stepper,int32_t position);
	uint8_t BMP73T104_setStepperHoldTorque(uint8_t stepper,uint8_t status);
    
	uint8_t BMP73T104_setIICSlaveAddr(uint8_t i2c_addr);
	void BMP73T104_reset(void);
	uint16_t BMP73T104_getFWVer(void);
	
	
	void BMP73T104_writeBytes(uint8_t wbuf[], uint8_t wlen);
	uint8_t BMP73T104_readBytes(uint8_t rbuf[], uint8_t rlen);
	void _BMP73T104_delay(vu32 count);
#ifdef __cplusplus
}
#endif

#endif

