#include "ht32f5xxxx_01.h"              // Device header
#include "BMP73T104.h"
#include "Motor.h"
//使用I2C_MASTER_CH1，引脚为PC4，PC5
extern int16_t encder_left;
extern int16_t encder_right;
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
	 TimeBaseIniture.CounterMode=TM_CNT_MODE_UP;                //边沿对齐向上计数
	 TimeBaseIniture.CounterReload=768*2*4;                           //计数重装载计数器
	 TimeBaseIniture.Prescaler=0;                               //预分频系数
	 TimeBaseIniture.PSCReloadTime=TM_PSC_RLD_IMMEDIATE;        //立即重装载预分频寄存器
	 TimeBaseIniture.RepetitionCounter=0;                         //定时器重复寄存器 
	 TM_TimeBaseInit(HTCFG_CAP_PORT_decode1A, &TimeBaseIniture);
 
 
 
 
//     TM_CaptureInitTypeDef  CapInit;
//
//    TM_CaptureStructInit(&CapInit);
//    CapInit.Channel = HTCFG_CAP_CH_decode1A | HTCFG_CAP_CH_decode1B;
//    CapInit.Polarity = TM_CHP_NONINVERTED;
//    CapInit.Selection = TM_CHCCS_DIRECT;
//    CapInit.Prescaler = TM_CHPSC_OFF;
//    CapInit.Filter = 0x0;
//    TM_CaptureInit(HTCFG_CAP_PORT_decode1B, &CapInit);
//  TM_StiConfig(HTCFG_CAP_PORT_decode1B, TM_TRSEL_TI0S0);
//
//  /* Slave Mode selection: Trigger Mode                                                                     */
//  TM_SlaveModeConfig(HTCFG_CAP_PORT_decode1B, TM_SMSEL_STIED);
//


   TM_DecoderConfig(HTCFG_CAP_PORT_decode1A, TM_SMSEL_DECODER3,                   //模式3双边沿计数
                        TM_CHP_NONINVERTED, TM_CHP_NONINVERTED);   //编码器模式的设置
	
   TM_ChExternalClockConfig(HTCFG_CAP_PORT_decode1A, TM_TRSEL_TI0BED, TM_CHP_INVERTED, 6);//比较滤波器
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
    BFTM_SetCompare(HT_BFTM0, SystemCoreClock/200);  //5ms产生中断
    
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
 
 
 
 
//     TM_CaptureInitTypeDef  CapInit;
//
//    TM_CaptureStructInit(&CapInit);
//    CapInit.Channel = HTCFG_CAP_CH_decode1A | HTCFG_CAP_CH_decode1B;
//    CapInit.Polarity = TM_CHP_NONINVERTED;
//    CapInit.Selection = TM_CHCCS_DIRECT;
//    CapInit.Prescaler = TM_CHPSC_OFF;
//    CapInit.Filter = 0x0;
//    TM_CaptureInit(HTCFG_CAP_PORT_decode1B, &CapInit);
//  TM_StiConfig(HTCFG_CAP_PORT_decode1B, TM_TRSEL_TI0S0);
//
//  /* Slave Mode selection: Trigger Mode                                                                     */
//  TM_SlaveModeConfig(HTCFG_CAP_PORT_decode1B, TM_SMSEL_STIED);
//


   TM_DecoderConfig(HTCFG_CAP_PORT_decode2A, TM_SMSEL_DECODER3,                   //模式3双边沿计数
                        TM_CHP_NONINVERTED, TM_CHP_NONINVERTED);   //编码器模式的设置
	
   TM_ChExternalClockConfig(HTCFG_CAP_PORT_decode2A, TM_TRSEL_TI0BED, TM_CHP_INVERTED, 6);//比较滤波器
	//TM_IntConfig(HTCFG_CAP_PORT_decode1A, TM_INT_CH1CC | TM_INT_UEV, ENABLE);//禁用中断

   HTCFG_CAP_PORT_decode2A->CNTR=0;      //计数器初始值
   TM_Cmd(HTCFG_CAP_PORT_decode2A,ENABLE);  //使能计数器 
}


int16_t  PID_Turn(float gzro, int16_t encoder_left,int16_t encoder_right)
{
   uint16_t Kp = 15, Kd = 5; //===PID参数
    static int16_t bias = 0;
	int16_t Turn_Amplitude=100, turn, encoder_temp;
	
	encoder_temp = encoder_left - encoder_right;
	bias += encoder_temp; //对角速度积分
	
	//限幅
	if(bias > Turn_Amplitude)  
    	bias = Turn_Amplitude;
	if(bias < -Turn_Amplitude) 
		bias = -Turn_Amplitude;
	
	turn = Kp * bias + Kd * gzro; //===结合Z轴陀螺仪进行PD控制
	
	return turn;

}
int16_t velocity(int16_t Targrt_Speed,int16_t encoder_left,int16_t encoder_right,int16_t RC)
{  
	static float Velocity,Encoder_Least,Encoder;
	static float Encoder_Integral;
	float velocity_KP=-150;//-300;
	float velocity_KI=-0;//-0.5;	
   int16_t Targrt= Targrt_Speed;
	Encoder_Least =(encoder_left-encoder_right)/2-Targrt;                    //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
	Encoder *= 0.7;		                                                //===一阶低通滤波器       
	Encoder += Encoder_Least*0.3;	                                    //===一阶低通滤波器    
	Encoder_Integral +=Encoder;                                       //===积分出位移 积分时间：5ms                                     
	if(Encoder_Integral>15000)  	Encoder_Integral=15000;             //===积分限幅
	if(Encoder_Integral<-15000)		Encoder_Integral=-15000;          //===积分限幅	
	Velocity=Encoder*velocity_KP+Encoder_Integral*velocity_KI*RC;        //===速度控制					
	return Velocity;
}
  