#ifndef MOTOR_H
#define MOTOR_H

#define Motor_Left_up 0x01//正数为正转
#define Motor_Left_down 0x02//正数为正转
#define Motor_Right_up 0x04//负数为正转
#define Motor_Right_down 0x08//负数为正转

#define V_IOref 5  //过流保护参考电压
#define Iset 2   //过流保护电流
/************************************电机一编码器A相****************************************8*/
#define Motor_decode1A_GPIOX                        A               // D27
#define Motor_decode1A_GPION                        4
#define  Motor_decode1A_IPN                          GPTM0
#define Motor_decode1A_CHN                          0
#define HTCFG_CAP_GPIO_ID_decode1A                        STRCAT2(GPIO_P,         Motor_decode1A_GPIOX)
#define HTCFG_CAP_AFIO_PIN_decode1A                        STRCAT2(AFIO_PIN_,      Motor_decode1A_GPION)
#define HTCFG_CAP_AFIO_FUN_decode1A                        STRCAT2(AFIO_FUN_,       Motor_decode1A_IPN)
#define HTCFG_CAP_PORT_decode1A                            STRCAT2(HT_,             Motor_decode1A_IPN)
#define HTCFG_CAP_CH_decode1A                              STRCAT2(TM_CH_,         Motor_decode1A_CHN)
/************************************电机一编码器A相****************************************8*/
/************************************电机一编码器B相****************************************8*/

  #define Motor_decode1B_GPIOX                        A               // D27
  #define Motor_decode1B_GPION                        5
  #define  Motor_decode1B_IPN                          GPTM0
  #define Motor_decode1B_CHN                          1
#define HTCFG_CAP_GPIO_ID_decode1B                         STRCAT2(GPIO_P,         Motor_decode1B_GPIOX)
#define HTCFG_CAP_AFIO_PIN_decode1B                        STRCAT2(AFIO_PIN_,      Motor_decode1B_GPION)
#define HTCFG_CAP_AFIO_FUN_decode1B                        STRCAT2(AFIO_FUN_,       Motor_decode1B_IPN)
#define HTCFG_CAP_PORT_decode1B                            STRCAT2(HT_,             Motor_decode1B_IPN)
#define HTCFG_CAP_CH_decode1B                              STRCAT2(TM_CH_,         Motor_decode1B_CHN)
/************************************电机一编码器B相****************************************8*/

/************************************电机二编码器A相****************************************8*/
  #define Motor_decode2A_GPIOX                        A               // D27
  #define Motor_decode2A_GPION                        14
  #define  Motor_decode2A_IPN                          MCTM0
  #define Motor_decode2A_CHN                          0
#define HTCFG_CAP_GPIO_ID_decode2A                        STRCAT2(GPIO_P,         Motor_decode2A_GPIOX)
#define HTCFG_CAP_AFIO_PIN_decode2A                        STRCAT2(AFIO_PIN_,      Motor_decode2A_GPION)
#define HTCFG_CAP_AFIO_FUN_decode2A                        STRCAT2(AFIO_FUN_,       Motor_decode2A_IPN)
#define HTCFG_CAP_PORT_decode2A                            STRCAT2(HT_,             Motor_decode2A_IPN)
#define HTCFG_CAP_CH_decode2A                              STRCAT2(TM_CH_,         Motor_decode2A_CHN)
/************************************电机二编码器A相****************************************8*/
/************************************电机二编码器B相****************************************8*/
#define Motor_decode2B_GPIOX                        A               // D27
#define Motor_decode2B_GPION                        10
#define  Motor_decode2B_IPN                          MCTM0
#define Motor_decode2B_CHN                          1
#define HTCFG_CAP_GPIO_ID_decode2B                         STRCAT2(GPIO_P,         Motor_decode2B_GPIOX)
#define HTCFG_CAP_AFIO_PIN_decode2B                        STRCAT2(AFIO_PIN_,      Motor_decode2B_GPION)
#define HTCFG_CAP_AFIO_FUN_decode2B                        STRCAT2(AFIO_FUN_,       Motor_decode2B_IPN)
#define HTCFG_CAP_PORT_decode2B                            STRCAT2(HT_,             Motor_decode2B_IPN)
#define HTCFG_CAP_CH_decode2B                              STRCAT2(TM_CH_,         Motor_decode2B_CHN)
/************************************电机二编码器B相****************************************8*/


//   #define Motor_decode3A_GPIOX                        A               // D27
//   #define Motor_decode3A_GPION                        10
//   #define  Motor_decode3A_IPN                          PWM0
//   #define Motor_decode3A_CHN                          0
// #define HTCFG_CAP_GPIO_ID_decode3A                        STRCAT2(GPIO_P,         Motor_decode3A_GPIOX)
// #define HTCFG_CAP_AFIO_PIN_decode3A                        STRCAT2(AFIO_PIN_,      Motor_decode3A_GPION)
// #define HTCFG_CAP_AFIO_FUN_decode3A                        STRCAT2(AFIO_FUN_,       Motor_decode3A_IPN)
// #define HTCFG_CAP_PORT_decode3A                            STRCAT2(HT_,             Motor_decode3A_IPN)
// #define HTCFG_CAP_CH_decode3A                              STRCAT2(TM_CH_,         Motor_decode3A_CHN)

//   #define Motor_decode3B_GPIOX                        A               // D27
//   #define Motor_decode3B_GPION                        14
//   #define  Motor_decode3B_IPN                          PWM0
//   #define Motor_decode3B_CHN                          1
// #define HTCFG_CAP_GPIO_ID_decode3B                         STRCAT2(GPIO_P,         Motor_decode3B_GPIOX)
// #define HTCFG_CAP_AFIO_PIN_decode3B                        STRCAT2(AFIO_PIN_,      Motor_decode3B_GPION)
// #define HTCFG_CAP_AFIO_FUN_decode3B                        STRCAT2(AFIO_FUN_,       Motor_decode3B_IPN)
// #define HTCFG_CAP_PORT_decode3B                            STRCAT2(HT_,             Motor_decode3B_IPN)
// #define HTCFG_CAP_CH_decode3B                              STRCAT2(TM_CH_,         Motor_decode3B_CHN)


void Set_Motor_decode1(void);
void Set_Motor_decode2(void);
uint8_t Motor_Init(void);
void Motor_Run(uint8_t Motor_Nam,int8_t Speed);
void Motor_Stop(uint8_t Motor_Nam);
void Motor_shache(uint8_t Motor_Nam);
void Motor_Get_decode_TMInit(void);


#endif