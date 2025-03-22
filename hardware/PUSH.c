#include "ht32.h"
#include "delay.h"

#define HX711_SCK   (1 << 0)  //  GPIOB Pin 0
#define HX711_DOUT  (1 << 1)  //  GPIOB Pin 1

u32 HX711_Buffer;  //HX711�ɼ���ԭʼ����
u32 Weight_Maopi; //����Ƥëֵ
s32 Weight_Shiwu; //ʵ������ֵ
u8 Flag_Error = 0;

#define GapValue 430  //У׼����
#define THRESHOLD 5000 //�����жϵ�������ֵ

//����ѹ������������
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

//��ȡѹ������������
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

//��ȡ����ֵ
void Get_Maopi(void) 
{
    Weight_Maopi = HX711_Read();
}

//����ʵ������
void Get_Weight(void) 
{
    HX711_Buffer = HX711_Read();
    if (HX711_Buffer > Weight_Maopi) 
	{
        Weight_Shiwu = HX711_Buffer - Weight_Maopi;
        Weight_Shiwu = (s32)((float)Weight_Shiwu / GapValue);  //����ʵ������
    }
}

//���ö�ʱ��
void BFTM_Config(void) 
{
    //ʹ��BFTM0ʱ��
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.BFTM0 = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);

    //���ö�ʱ���ڣ�10ms����һ���ж�
    uint32_t timer_clk = SystemCoreClock / 1000;  // 72MHz / 1000 = 72kHz
    uint32_t reload = (timer_clk * 10) - 1;       // 10ms ??????

    //����BFTM0��ʱ��
    BFTM_SetCounter(HT_BFTM0, 0);
    BFTM_SetCompare(HT_BFTM0, reload);
    BFTM_IntConfig(HT_BFTM0, ENABLE);
    NVIC_EnableIRQ(BFTM0_IRQn);

    //����BFTM0��ʱ��
    BFTM_EnaCmd(HT_BFTM0, ENABLE);
}

//��������������Ƿ�ﵽ��������
void Check_Force(void) 
{
    int32_t weight = HX711_Read();
    if (weight > THRESHOLD) 
    {
        //������ش���       
    }
}

//��ʱ��BFTM0�жϷ�����
void BFTM0_IRQHandler(void) 
{
    BFTM_ClearFlag(HT_BFTM0);
    Check_Force();
}







