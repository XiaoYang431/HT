#ifndef MOTOR_H
#define MOTOR_H

// 定义电机方向控制引脚
#define MOTOR1_DIR_PORT   HT_GPIOB
#define MOTOR1_DIR_PIN    GPIO_PIN_0

#define MOTOR2_DIR_PORT   HT_GPIOB
#define MOTOR2_DIR_PIN    GPIO_PIN_1

#define MOTOR3_DIR_PORT   HT_GPIOB
#define MOTOR3_DIR_PIN    GPIO_PIN_2

#define MOTOR4_DIR_PORT   HT_GPIOB
#define MOTOR4_DIR_PIN    GPIO_PIN_3

void MOTOR_Init(void);
void MOTOR_SetSpeed(uint8_t motor_id, uint8_t direction, uint16_t speed);
void MOTOR_MoveForward(uint16_t speed);
void MOTOR_MoveBackward(uint16_t speed);
void MOTOR_Stop(void);

#endif
