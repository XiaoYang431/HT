#ifndef __ENCODER_H
#define __ENCODER_H
#include "ht32f5xxxx_01.h"
void Encoder_Init(void);        // 编码器初始化
uint32_t Get_Encoder_Speed(uint8_t motor_id); // 获取指定电机的速度

#endif
