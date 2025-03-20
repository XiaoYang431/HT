#include "ht32f5xxxx_01.h"              // Device header
#include "MOTOR.h"
#include "PWM.h"

// 初始化电机控制引脚
void MOTOR_Init(void)
{
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.PA = 1;   // 使能 GPIOA 时钟
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
    
    // 配置 GPIO 方向引脚为输出
    GPIO_DirectionConfig(MOTOR1_DIR_PORT, MOTOR1_DIR_PIN, GPIO_DIR_OUT);
    GPIO_DirectionConfig(MOTOR2_DIR_PORT, MOTOR2_DIR_PIN, GPIO_DIR_OUT);
    GPIO_DirectionConfig(MOTOR3_DIR_PORT, MOTOR3_DIR_PIN, GPIO_DIR_OUT);
    GPIO_DirectionConfig(MOTOR4_DIR_PORT, MOTOR4_DIR_PIN, GPIO_DIR_OUT);
    
    // 初始化 PWM (需要在 PWM.c 中实现)
    PWM_Init();
}

// 设置单个电机的转向和速度
void MOTOR_SetSpeed(uint8_t motor_id, uint8_t direction, uint16_t speed)
{
    switch (motor_id)
    {
        case 1:
            GPIO_WriteOutBits(MOTOR1_DIR_PORT, MOTOR1_DIR_PIN, direction);
            PWM_Setcompare1(speed);
            break;
        case 2:
            GPIO_WriteOutBits(MOTOR2_DIR_PORT, MOTOR2_DIR_PIN, direction);
            PWM_Setcompare2(speed);
            break;
        case 3:
            GPIO_WriteOutBits(MOTOR3_DIR_PORT, MOTOR3_DIR_PIN, direction);
            PWM_Setcompare3(speed);
            break;
        case 4:
            GPIO_WriteOutBits(MOTOR4_DIR_PORT, MOTOR4_DIR_PIN, direction);
            PWM_Setcompare4(speed);
            break;
        default:
            break;
    }
}

void MOTOR_MoveForward(uint16_t speed)
{
    MOTOR_SetSpeed(1, 1, speed);
    MOTOR_SetSpeed(2, 1, speed);
    MOTOR_SetSpeed(3, 1, speed);
    MOTOR_SetSpeed(4, 1, speed);
}

void MOTOR_MoveBackward(uint16_t speed)
{
    MOTOR_SetSpeed(1, 0, speed);
    MOTOR_SetSpeed(2, 0, speed);
    MOTOR_SetSpeed(3, 0, speed);
    MOTOR_SetSpeed(4, 0, speed);
}

void MOTOR_Stop(void)
{
    MOTOR_SetSpeed(1, 0, 0);
    MOTOR_SetSpeed(2, 0, 0);
    MOTOR_SetSpeed(3, 0, 0);
    MOTOR_SetSpeed(4, 0, 0);
}
