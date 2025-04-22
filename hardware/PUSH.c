#include "ht32.h"
#include "Send_ForceAlert.h"
void push_delay_us(vu32 count);
#define HX711_SCK   (1 << 0)  //  GPIOB Pin 0
#define HX711_DOUT  (1 << 1)  //  GPIOB Pin 1

u32 HX711_Buffer;  //HX711采集的原始数据
u32 Weight_Maopi; //空载皮毛值
s32 Weight_Shiwu; //实际拉力值
u8 Flag_Error = 0;

#define GapValue 430  //校准参数
#define THRESHOLD 20 //触发中断的拉力阈值，单位：N

//配置压力传感器引脚
void Init_HX711pin(void)
{
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.PB = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);

    GPIO_SetOutBits(HT_GPIOB, HX711_SCK);
    GPIO_DirectionConfig(HT_GPIOB, HX711_SCK, GPIO_DIR_OUT);
    GPIO_DirectionConfig(HT_GPIOB, HX711_DOUT, GPIO_DIR_IN);
    GPIO_PullResistorConfig(HT_GPIOB, HX711_DOUT, GPIO_PR_UP);
}

//读取压力传感器数据
u32 HX711_Read(void)
{
    unsigned long count = 0;
    unsigned char i;

    GPIO_ClearOutBits(HT_GPIOB, HX711_SCK);
    push_delay_us(1);

    while (GPIO_ReadInBit(HT_GPIOB, HX711_DOUT));

    for (i = 0; i < 24; i++) 
	{
        GPIO_SetOutBits(HT_GPIOB, HX711_SCK);
        count = count << 1;
        push_delay_us(1);
        GPIO_ClearOutBits(HT_GPIOB, HX711_SCK);
        if (GPIO_ReadInBit(HT_GPIOB, HX711_DOUT))
            count++;
        push_delay_us(1);
    }

    GPIO_SetOutBits(HT_GPIOB, HX711_SCK);
    count = count ^ 0x800000;
    push_delay_us(1);
    GPIO_ClearOutBits(HT_GPIOB, HX711_SCK);

    return count;
}

//获取空载值
void Get_Maopi(void) 
{
    Weight_Maopi = HX711_Read();
}

//计算实际拉力
void Get_Weight(void) 
{
    HX711_Buffer = HX711_Read();
    if (HX711_Buffer > Weight_Maopi) 
	{
        Weight_Shiwu = HX711_Buffer - Weight_Maopi;
        Weight_Shiwu = (s32)((float)Weight_Shiwu / GapValue);  //计算实际拉力
    }
}

//配置定时器
void BFTM1_Config(void) 
{
    //使能BFTM1时钟
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.BFTM1 = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);

    //配置定时周期，10ms触发一次中断
    uint32_t timer_clk = SystemCoreClock / 1000;  // 72MHz / 1000 = 72kHz
    uint32_t reload = (timer_clk * 10) - 1;       // 10ms 触发一次中断

    //配置BFTM1定时器
    BFTM_SetCounter(HT_BFTM1, 0);
    BFTM_SetCompare(HT_BFTM1, reload);
    BFTM_IntConfig(HT_BFTM1, ENABLE);
    NVIC_EnableIRQ(BFTM1_IRQn);

    //启动BFTM1定时器
    BFTM_EnaCmd(HT_BFTM1, ENABLE);
}

//检查拉力传感器是否达到拉力上限
void Check_Force(void) 
{
    static u8 alert_sent = 0;
    Get_Weight();
    
    if (Weight_Shiwu > THRESHOLD) 
    {
        if(!alert_sent)
        {
            alert_sent = 1;
            SendForceAlert(Weight_Shiwu);  //达到上限就报警，同时发短信/打电话给手机，同时小车停止运动
        }
    }
    else 
    {
        alert_sent = 0;  // 清除标志
    }
}


void push_delay_ms(vu32 count)
{
  count = SystemCoreClock / 8000 * count ;
  while(count--);
}

void push_delay_us(vu32 count)
{
  count = SystemCoreClock / 8000 * count /1000;
  while(count--);
}






