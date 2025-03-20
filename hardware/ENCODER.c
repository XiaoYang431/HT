#include "ENCODER.h"
#include "ht32f5xxxx_01.h"              // Device header

// 存储四个电机编码器的捕获值
volatile uint32_t encoder_count[4] = {0};

// 中断服务函数
void GPTM1_IRQHandler(void)
{
    if (TM_GetIntStatus(HT_GPTM1, TM_INT_CH0CC) != RESET)
    {
        encoder_count[0] = TM_GetCaptureCompare0(HT_GPTM1);
        TM_ClearIntPendingBit(HT_GPTM1, TM_INT_CH0CC);
    }
    if (TM_GetIntStatus(HT_GPTM1, TM_INT_CH1CC) != RESET)
    {
        encoder_count[1] = TM_GetCaptureCompare1(HT_GPTM1);
        TM_ClearIntPendingBit(HT_GPTM1, TM_INT_CH1CC);
    }
    if (TM_GetIntStatus(HT_GPTM1, TM_INT_CH2CC) != RESET)
    {
        encoder_count[2] = TM_GetCaptureCompare2(HT_GPTM1);
        TM_ClearIntPendingBit(HT_GPTM1, TM_INT_CH2CC);
    }
    if (TM_GetIntStatus(HT_GPTM1, TM_INT_CH3CC) != RESET)
    {
        encoder_count[3] = TM_GetCaptureCompare3(HT_GPTM1);
        TM_ClearIntPendingBit(HT_GPTM1, TM_INT_CH3CC);
    }
}

// 初始化编码器
void Encoder_Init(void)
{
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    TM_TimeBaseInitTypeDef TM_TimeBaseInitStruct;
    TM_CaptureInitTypeDef TM_InputCaptureInitStruct;

    // 使能时钟
    CKCUClock.Bit.GPTM1 = 1;
    CKCUClock.Bit.AFIO  = 1;
    CKCUClock.Bit.PB    = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);

    // 配置 GPIO 复用
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_0, AFIO_MODE_4);
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_1, AFIO_MODE_4);
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_2, AFIO_MODE_4);
    AFIO_GPxConfig(GPIO_PB, AFIO_PIN_3, AFIO_MODE_4);

    // 配置定时器
    TM_TimeBaseInitStruct.Prescaler = 99; // 降低计数频率，避免溢出
    TM_TimeBaseInitStruct.CounterReload = 0xFFFF;
    TM_TimeBaseInitStruct.RepetitionCounter = 0;
    TM_TimeBaseInitStruct.CounterMode = TM_CNT_MODE_UP;
    TM_TimeBaseInitStruct.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
    TM_TimeBaseInit(HT_GPTM1, &TM_TimeBaseInitStruct);

    // 初始化捕获通道
    TM_CaptureStructInit(&TM_InputCaptureInitStruct);
    TM_InputCaptureInitStruct.Channel = TM_CH_0;
    TM_InputCaptureInitStruct.Polarity = TM_CHP_NONINVERTED;
    TM_InputCaptureInitStruct.Selection = TM_CHCCS_DIRECT;
    TM_InputCaptureInitStruct.Prescaler = TM_CHPSC_OFF;
    TM_InputCaptureInitStruct.Filter = 0xF;
    TM_CaptureInit(HT_GPTM1, &TM_InputCaptureInitStruct);

    // 其他三个通道
    TM_InputCaptureInitStruct.Channel = TM_CH_1;
    TM_CaptureInit(HT_GPTM1, &TM_InputCaptureInitStruct);

    TM_InputCaptureInitStruct.Channel = TM_CH_2;
    TM_CaptureInit(HT_GPTM1, &TM_InputCaptureInitStruct);

    TM_InputCaptureInitStruct.Channel = TM_CH_3;
    TM_CaptureInit(HT_GPTM1, &TM_InputCaptureInitStruct);

    // 使能中断
    NVIC_EnableIRQ(GPTM1_IRQn);
    TM_IntConfig(HT_GPTM1, TM_INT_CH0CC | TM_INT_CH1CC | TM_INT_CH2CC | TM_INT_CH3CC, ENABLE);

    // 启动定时器
    TM_Cmd(HT_GPTM1, ENABLE);
}

// 获取电机转速
uint32_t Get_Encoder_Speed(uint8_t motor_id)
{
    if (motor_id > 3) return 0;
    return encoder_count[motor_id];
}
