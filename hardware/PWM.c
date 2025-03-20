#include "ht32f5xxxx_01.h"  // 设备头文件
#include "PWM.h"

// PWM 初始化函数，配置 GPTM0 产生 4 路 PWM
void PWM_Init(void)
{  
    // 定义结构体
    TM_TimeBaseInitTypeDef TM_TimeBaseInitStruct;
    TM_OutputInitTypeDef TM_OutputInitStruct;
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};

    // 使能时钟
    CKCUClock.Bit.GPTM0 = 1;
    CKCUClock.Bit.AFIO  = 1;
    CKCUClock.Bit.PA    = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);

    // 配置 GPIO 复用为 PWM（GPTM0 通道对应 PA0, PA1, PA2, PA3）
    AFIO_GPxConfig(GPIO_PA, AFIO_PIN_0, AFIO_MODE_4);
    AFIO_GPxConfig(GPIO_PA, AFIO_PIN_1, AFIO_MODE_4);
    AFIO_GPxConfig(GPIO_PA, AFIO_PIN_2, AFIO_MODE_4);
    AFIO_GPxConfig(GPIO_PA, AFIO_PIN_3, AFIO_MODE_4);

    // 配置 GPTM0 时基单元
    TM_TimeBaseInitStruct.Prescaler = 100 - 1;        // 预分频（100 分频）
    TM_TimeBaseInitStruct.CounterReload = 100 - 1;    // 计数周期，得到 0~100 的占空比
    TM_TimeBaseInitStruct.RepetitionCounter = 0;
    TM_TimeBaseInitStruct.CounterMode = TM_CNT_MODE_UP;
    TM_TimeBaseInitStruct.PSCReloadTime = TM_PSC_RLD_IMMEDIATE; 
    TM_TimeBaseInit(HT_GPTM0, &TM_TimeBaseInitStruct);  

    // 初始化输出通道
    TM_OutputStructInit(&TM_OutputInitStruct);
    TM_OutputInitStruct.OutputMode = TM_OM_PWM1;       // PWM 模式
    TM_OutputInitStruct.Control = TM_CHCTL_ENABLE;     // 使能通道
    TM_OutputInitStruct.Polarity = TM_CHP_NONINVERTED; // 非反向输出

    // 配置 4 个 PWM 通道
    TM_OutputInitStruct.Channel = TM_CH_0;
    TM_OutputInit(HT_GPTM0, &TM_OutputInitStruct);
    
    TM_OutputInitStruct.Channel = TM_CH_1;
    TM_OutputInit(HT_GPTM0, &TM_OutputInitStruct);
    
    TM_OutputInitStruct.Channel = TM_CH_2;
    TM_OutputInit(HT_GPTM0, &TM_OutputInitStruct);
    
    TM_OutputInitStruct.Channel = TM_CH_3;
    TM_OutputInit(HT_GPTM0, &TM_OutputInitStruct);
    
    // 启动 GPTM0
    TM_Cmd(HT_GPTM0, ENABLE);
}

// 设置 4 个 PWM 通道的占空比（0-100）
void PWM_Setcompare1(uint16_t CRR)
{
    TM_SetCaptureCompare0(HT_GPTM0, CRR);
}

void PWM_Setcompare2(uint16_t CRR)
{
    TM_SetCaptureCompare1(HT_GPTM0, CRR);
}

void PWM_Setcompare3(uint16_t CRR)
{
    TM_SetCaptureCompare2(HT_GPTM0, CRR);
}

void PWM_Setcompare4(uint16_t CRR)
{
    TM_SetCaptureCompare3(HT_GPTM0, CRR);
}

