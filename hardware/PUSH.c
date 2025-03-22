#include "ht32.h"
#include "delay.h"

#define HX711_SCK   (1 << 0)  //  GPIOB Pin 0
#define HX711_DOUT  (1 << 1)  //  GPIOB Pin 1

u32 HX711_Buffer;  //HX711采集的原始数据
u32 Weight_Maopi; //空载皮毛值
s32 Weight_Shiwu; //实际拉力值
u8 Flag_Error = 0;

#define GapValue 430  //校准参数
#define THRESHOLD 5000 //触发中断的拉力阈值

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
    Delay_us(1);

    while (GPIO_ReadInBit(HT_GPIOB, HX711_DOUT));

    for (i = 0; i < 24; i++) 
	{
        GPIO_SetOutBits(HT_GPIOB, HX711_SCK);
        count = count << 1;
        Delay_us(1);
        GPIO_ClearOutBits(HT_GPIOB, HX711_SCK);
        if (GPIO_ReadInBit(HT_GPIOB, HX711_DOUT))
            count++;
        Delay_us(1);
    }

    GPIO_SetOutBits(HT_GPIOB, HX711_SCK);
    count = count ^ 0x800000;
    Delay_us(1);
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
void BFTM_Config(void) 
{
    //使能BFTM0时钟
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.BFTM0 = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);

    //配置定时周期，10ms触发一次中断
    uint32_t timer_clk = SystemCoreClock / 1000;  // 72MHz / 1000 = 72kHz
    uint32_t reload = (timer_clk * 10) - 1;       // 10ms ??????

    //配置BFTM0定时器
    BFTM_SetCounter(HT_BFTM0, 0);
    BFTM_SetCompare(HT_BFTM0, reload);
    BFTM_IntConfig(HT_BFTM0, ENABLE);
    NVIC_EnableIRQ(BFTM0_IRQn);

    //启动BFTM0定时器
    BFTM_EnaCmd(HT_BFTM0, ENABLE);
}

//检查拉力传感器是否达到拉力上限
void Check_Force(void) 
{
    int32_t weight = HX711_Read();
    if (weight > THRESHOLD) 
    {
        //报警相关代码       
    }
}

//定时器BFTM0中断服务函数
void BFTM0_IRQHandler(void) 
{
    BFTM_ClearFlag(HT_BFTM0);
    Check_Force();
}







