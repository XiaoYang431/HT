#include "ht32f5xxxx_01.h"              // Device header
#include "BMP73T104.h"
#include "Motor.h"
#include "Kaerman.h"
#include "Bizhang.h"
#include <math.h>
#include <stdlib.h>

#define M_PI 3.14159265358979323846
 float  angle_error = 5;
 float current_angle  = 0;
uint8_t Motor_Init(void)
{
    /*
    if(BM32S2031_1_selSerial(HTCFG_UARTM_CH2) == BM32S2031_1_FAILURE)
    {
        return - 1;
    }
    BM32S2031_1_Init();
    */
    if(BMP73T104_selWire(I2C_MASTER_CH0) == BMP73T104_FAILURE)
    {
        return - 1;
    }
    BMP73T104_Init();
    BMP73T104_begin(0x0F,0,0);
	return 0;
}

uint8_t ocp = (Iset * 0.05 / V_IOref) * 4095;

void Motor_Run(uint8_t Motor_Nam,int8_t Speed)
{
   BMP73T104_dcMotorRun(Motor_Nam,Speed);
  //BMP73T104_setOCP(Motor_Nam, ocp,10,ENABLE );//开启过流保护,要连接Int引脚进行检测，此时输出低电平
}
void Motor_Stop(uint8_t Motor_Nam)
{
 BMP73T104_dcMotorStop(Motor_Nam);
 // BMP73T104_setOCP(Motor_Nam, ocp,10,ENABLE );//关闭过流保护
}
void Motor_shache(uint8_t Motor_Nam)
{
 BMP73T104_dcMotorBrake( Motor_Nam);
}
void Set_Motor_decode1(void)
{


TM_TimeBaseInitTypeDef TimeBaseIniture;         //结构体

  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
		
	
   CKCUClock.Bit.PA         = 1;       //GPIOA时钟打开
   CKCUClock.Bit.AFIO       = 1;       //复用时钟
   CKCUClock.Bit.Motor_decode1A_IPN      = 1;       //开启马达定时器
   CKCU_PeripClockConfig(CKCUClock, ENABLE);      //时钟使能
   AFIO_GPxConfig(HTCFG_CAP_GPIO_ID_decode1A, HTCFG_CAP_AFIO_PIN_decode1A, AFIO_MODE_4);  //开启复用功能
	 
	//捕获IO口的设置 
   CKCUClock.Bit.PA         = 1;       //GPIOA时钟打开
   CKCUClock.Bit.AFIO       = 1;       //复用时钟
   CKCUClock.Bit.Motor_decode1B_IPN      = 1;       //开启马达定时器
   CKCU_PeripClockConfig(CKCUClock, ENABLE);      //时钟使能
   AFIO_GPxConfig(HTCFG_CAP_GPIO_ID_decode1B, HTCFG_CAP_AFIO_PIN_decode1B, AFIO_MODE_4);  //开启复用功能
		
	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_4, ENABLE);       //使能输入功能
    GPIO_InputConfig(HT_GPIOA, GPIO_PIN_5, ENABLE);       //使能输入功能	 
	 	 
	 TM_DeInit(HTCFG_CAP_PORT_decode1A);    //恢复默认设置
	 TM_TimeBaseStructInit(&TimeBaseIniture);
	 
 //对定时器时基以及计数方式初始化
	// TimeBaseIniture.CounterMode=TM_CNT_MODE_UP;                //边沿对齐向上计数
	 TimeBaseIniture.CounterReload=768*2*4;                           //计数重装载计数器
	 TimeBaseIniture.Prescaler=0;                               //预分频系数
	 TimeBaseIniture.PSCReloadTime=TM_PSC_RLD_IMMEDIATE;        //立即重装载预分频寄存器
	 TimeBaseIniture.RepetitionCounter=0;                         //定时器重复寄存器 
	 TM_TimeBaseInit(HTCFG_CAP_PORT_decode1A, &TimeBaseIniture);
 
 
 
 
     TM_CaptureInitTypeDef  CapInit;
//
    TM_CaptureStructInit(&CapInit);
    CapInit.Channel = HTCFG_CAP_CH_decode1A | HTCFG_CAP_CH_decode1B;
//    CapInit.Polarity = TM_CHP_NONINVERTED;
//    CapInit.Selection = TM_CHCCS_DIRECT;
//    CapInit.Prescaler = TM_CHPSC_OFF;
  CapInit.Filter = 0xF;
   TM_CaptureInit(HTCFG_CAP_PORT_decode1B, &CapInit);
//  TM_StiConfig(HTCFG_CAP_PORT_decode1B, TM_TRSEL_TI0S0);
//
//  /* Slave Mode selection: Trigger Mode                                                                     */
//  TM_SlaveModeConfig(HTCFG_CAP_PORT_decode1B, TM_SMSEL_STIED);
//


   TM_DecoderConfig(HTCFG_CAP_PORT_decode1A, TM_SMSEL_DECODER3,                   //模式3双边沿计数
                        TM_CHP_NONINVERTED, TM_CHP_NONINVERTED);   //编码器模式的设置
   //TM_ChExternalClockConfig(HTCFG_CAP_PORT_decode1A, TM_TRSEL_TI0BED, TM_CHP_INVERTED, 6);//比较滤波器
	//TM_IntConfig(HTCFG_CAP_PORT_decode1A, TM_INT_CH1CC | TM_INT_UEV, ENABLE);//禁用中断

   HTCFG_CAP_PORT_decode1A->CNTR=0;      //计数器初始值
   TM_Cmd(HTCFG_CAP_PORT_decode1A,ENABLE);  //使能计数器 
}
void Motor_Get_decode_TMInit(void)
{
  
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.BFTM0 = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
    
    // 设置定时器计数器初值和比较值
    BFTM_SetCounter(HT_BFTM0, 0);
    BFTM_SetCompare(HT_BFTM0, SystemCoreClock/100);  //10ms产生中断,及200Hz
    
    // 清除中断标志位     
    BFTM_ClearFlag(HT_BFTM0);
    // 开启定时器中断
    BFTM_IntConfig(HT_BFTM0, ENABLE);
    // 使能中断向量
    NVIC_EnableIRQ(BFTM0_IRQn);
    // 启动定时器
    BFTM_EnaCmd(HT_BFTM0, ENABLE);

}





void Set_Motor_decode2(void)
{


TM_TimeBaseInitTypeDef TimeBaseIniture;         //结构体

  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
		
	
   CKCUClock.Bit.PA         = 1;       //GPIOA时钟打开
   CKCUClock.Bit.AFIO       = 1;       //复用时钟
   CKCUClock.Bit.MCTM0      = 1;       //开启马达定时器
   CKCU_PeripClockConfig(CKCUClock, ENABLE);      //时钟使能
   AFIO_GPxConfig(HTCFG_CAP_GPIO_ID_decode2A, HTCFG_CAP_AFIO_PIN_decode2A, HTCFG_CAP_AFIO_FUN_decode2A);  //开启复用功能
	 
	//捕获IO口的设置 
   CKCUClock.Bit.PA         = 1;       //GPIOA时钟打开
   CKCUClock.Bit.AFIO       = 1;       //复用时钟
   CKCUClock.Bit.MCTM0      = 1;       //开启马达定时器
   CKCU_PeripClockConfig(CKCUClock, ENABLE);      //时钟使能
   AFIO_GPxConfig(HTCFG_CAP_GPIO_ID_decode2B, HTCFG_CAP_AFIO_PIN_decode2B, HTCFG_CAP_AFIO_FUN_decode2B);  //开启复用功能
		
	GPIO_InputConfig(HT_GPIOA, GPIO_PIN_14, ENABLE);       //使能输入功能
    GPIO_InputConfig(HT_GPIOA, GPIO_PIN_10, ENABLE);       //使能输入功能	 
	 	 
	 TM_DeInit(HTCFG_CAP_PORT_decode2A);    //恢复默认设置
	 TM_TimeBaseStructInit(&TimeBaseIniture);	 
 //对定时器时基以及计数方式初始化
	 TimeBaseIniture.CounterMode=TM_CNT_MODE_UP;                //边沿对齐向上计数
	 TimeBaseIniture.CounterReload=768*2*4;                           //计数重装载计数器
	 TimeBaseIniture.Prescaler=0;                               //预分频系数
	 TimeBaseIniture.PSCReloadTime=TM_PSC_RLD_IMMEDIATE;        //立即重装载预分频寄存器
	 TimeBaseIniture.RepetitionCounter=0;                         //定时器重复寄存器 
	 TM_TimeBaseInit(HTCFG_CAP_PORT_decode2A, &TimeBaseIniture);
 
 
 
 
   TM_CaptureInitTypeDef  CapInit;
//
    TM_CaptureStructInit(&CapInit);
    CapInit.Channel = HTCFG_CAP_CH_decode2A | HTCFG_CAP_CH_decode2B;
//    CapInit.Polarity = TM_CHP_NONINVERTED;
//    CapInit.Selection = TM_CHCCS_DIRECT;
//    CapInit.Prescaler = TM_CHPSC_OFF;
    CapInit.Filter = 0xF;
   TM_CaptureInit(HTCFG_CAP_PORT_decode1B, &CapInit);
//  TM_StiConfig(HTCFG_CAP_PORT_decode1B, TM_TRSEL_TI0S0);
//
//  /* Slave Mode selection: Trigger Mode                                                                     */
//  TM_SlaveModeConfig(HTCFG_CAP_PORT_decode1B, TM_SMSEL_STIED);
//


   TM_DecoderConfig(HTCFG_CAP_PORT_decode2A, TM_SMSEL_DECODER3,                   //模式3双边沿计数
                        TM_CHP_NONINVERTED, TM_CHP_NONINVERTED);   //编码器模式的设置
	
  // TM_ChExternalClockConfig(HTCFG_CAP_PORT_decode2A, TM_TRSEL_TI0BED, TM_CHP_INVERTED, 6);//比较滤波器
	//TM_IntConfig(HTCFG_CAP_PORT_decode1A, TM_INT_CH1CC | TM_INT_UEV, ENABLE);//禁用中断

   HTCFG_CAP_PORT_decode2A->CNTR=0;      //计数器初始值
   TM_Cmd(HTCFG_CAP_PORT_decode2A,ENABLE);  //使能计数器 
}




int32_t PID_Turn(float dx, float dy)
{
    float Kp = 500, Kd = 10;//2.5; // PID 参数
    static int32_t bias = 0;
    int32_t Turn_Amplitude = 100, turn, encoder_temp;
 
	float gyro_z =  BMS56M605_readGyroscopeZ();
	gyro_z = gyro_z*M_PI/180;
	
    // 计算目标角度（弧度）
    float target_angle = atan2(dx, dy); // 目标方向向量的角度
	
    // 获取当前角度（假设 gzro 是角速度，单位为度/秒）
     
	//if((encoder_right != 0) || (encoder_left != 0))
	//如果误差过小，则不会进行积分，结束本次转向
	 current_angle += -gyro_z * 0.005;   // 假设控制周期为 10ms

    // 计算角度偏差
      angle_error = target_angle - current_angle;
	
    // 将角度偏差转换为 [-π, π] 范围
    if (angle_error > M_PI)
        angle_error -= 2 * M_PI;
    if (angle_error < -M_PI)
        angle_error += 2 * M_PI;

    // PID 控制计算转向值
    turn = Kp * angle_error + Kd * gyro_z;

    // 限幅转向值
    if (turn > Turn_Amplitude)
        turn = Turn_Amplitude;
    if (turn < -Turn_Amplitude)
        turn = -Turn_Amplitude;


	 return turn; // 返回转向值，用于调整左右轮速度
	
   
}
int32_t velocity(int32_t Targrt_Speed,int32_t encoder_left,int32_t encoder_right)
{  //Targrt_Speed单位是毫米
	static float Velocity,Encoder_Least,Encoder =0;
   static float Encoder_last = 0;
	static float Encoder_Integral=0;
	double velocity_KP=0.125;//-300;
	double velocity_KI= 0.00075;//-0.5;	
   float velocity_KD=0;
   float Targrt= (Targrt_Speed/(200*3.14*65))*890;
	Encoder_Least =(-encoder_left-encoder_right)/2-Targrt;                    //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
	Encoder *= 0.7;		                                                //===一阶低通滤波器       
	Encoder += Encoder_Least*0.3;	                                    //===一阶低通滤波器    
	Encoder_Integral +=Encoder;                                       //===积分出位移 积分时间：5ms                                     
	if(Encoder_Integral>150)  	Encoder_Integral=150;             //===积分限幅
	if(Encoder_Integral<-150)		Encoder_Integral=-150;            //===积分限幅	

	Velocity=Encoder*velocity_KP+Encoder_Integral*velocity_KI+ velocity_KD*(Encoder-Encoder_last);        //===速度控制
	//Velocity = Velocity / 10;
if(Velocity>100.0  )
{
	Velocity =100;

}	
else if( Velocity < -100)
{
	Velocity = -100;
}
	Encoder_last = Encoder;	//===上次速度值
 
   
   return Velocity;
}
uint8_t BMS56M605_my_init(void)
{
    //if(BMS56M605_selWire(I2C_MASTER_CH1) == BMS56M605_FAILURE)
	//	return 1;

   BMS56M605_Init();
     BMS56M605_reset();
     //BMS56M605_enableCycle(1);
BMS56M605_setGyroRange(BMS56M605_GYRO_RANGE_2000);
BMS56M605_setAccelerometerRange(BMS56M605_ACC_RANGE_2G);
 //BMS56M605_setCycleRate(BMS56M605_F_1_25HZ);
 BMS56M605_setSampleRateDivisor(1000/200-1);//200HZ
BMS56M605_setFilterBandwidth(BMS56M605_ACC_96HZ_GYRO_98HZ);
BMS56M605_writeReg(BMS56M605_REG_USER_CTRL,0X00);	//I2C主模式关闭
	BMS56M605_writeReg(BMS56M605_REG_FIFO_EN,0X00);	//开启FIFO
	
	
	uint8_t res = 0;
   res = BMS56M605_readReg(BMS56M605_REG_WHO_AM_I); 
	if(res==0X68)//器件ID正确
	{
	BMS56M605_writeReg(BMS56M605_REG_PWR_MGMT_1,0X01);	//设置CLKSEL,PLL X轴为参考
	BMS56M605_writeReg(BMS56M605_REG_PWR_MGMT_2,0X00);	//加速度与陀螺仪都工作
   BMS56M605_setSampleRateDivisor(1000/200-1);
   BMS56M605_setFilterBandwidth(BMS56M605_ACC_96HZ_GYRO_98HZ);
 	}else return 1;
	BMS56M605_writeReg(BMS56M605_REG_INT_PIN_CFG,0X80);	//INT引脚低电平有效
	BMS56M605_writeReg(BMS56M605_REG_INT_ENABLE,0X01);
	return 0;
}
  

//int16_t Bizhnag_Start(void)
//{
//   int16_t Turn = 0,Velocity = 0;
//      if(BM32S2031_1_getIRStatus() == 1)
//      {
//      Turn = PID_Turn(0,0,0);
//       Velocity = velocity(0,0,0);
//           // _BM32S2031_1_delay(1000);
//      }
//      else
//      {
//          Velocity = velocity(0,0,0);
//      }
//      Speed_left = Velocity + Turn;
//      Speed_right = Velocity - Turn;
//}
//初始化一个GPIO口用来检测陀螺仪的INT引脚
void INt_Get_Gyro_Init(void)
{
    { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.PB = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  { /* Configure GPIO as input mode                                                                         */

    /* Configure AFIO mode as GPIO                                                                          */
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_5, AFIO_FUN_GPIO);

    /* Configure GPIO pull resistor                                                                         */
    GPIO_PullResistorConfig(HT_GPIOB, GPIO_PIN_5, GPIO_PR_UP);

    /* Configure GPIO direction as input                                                                    */
    GPIO_DirectionConfig(HT_GPIOB, GPIO_PIN_5, GPIO_DIR_IN);

    /* Enable input function for read                                                                       */
    GPIO_InputConfig(HT_GPIOB, GPIO_PIN_5, ENABLE);
  }

}