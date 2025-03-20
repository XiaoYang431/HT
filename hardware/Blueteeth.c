#include "ht32f5xxxx_01.h"
#include "Blueteeth.h"
 //蓝牙（串口0）初始化
void BLUETEETH_USART_Init(void)
{
  USART_InitTypeDef USART_InitStruct; 
  
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.USART0   = 1;
  CKCUClock.Bit.AFIO     = 1;
  CKCUClock.Bit.PA      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);
  // PA2--Tx  PA3--Rx
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_2, AFIO_MODE_6);  // 复用功能  AFIO_FUN_USART_UART
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_3, AFIO_MODE_6);  
 
  GPIO_PullResistorConfig(HT_GPIOA, GPIO_PIN_3, GPIO_PR_UP);  // 打开UxART0 Rx内部上拉电阻 
      
  USART_InitStruct.USART_BaudRate = 115200;  // 波特率
  USART_InitStruct.USART_WordLength = USART_WORDLENGTH_8B; // 数据长度
  USART_InitStruct.USART_StopBits = USART_STOPBITS_1; // 停止位
  USART_InitStruct.USART_Parity = USART_PARITY_NO; // 校验位
  USART_InitStruct.USART_Mode = USART_MODE_NORMAL; // 模式
  USART_Init(HT_USART0, &USART_InitStruct); 
  
  //USART_IntConfig(HT_USARTx, USART_INT_RXDR ,ENABLE or DISABLE) // 接收数据就绪中断
  //USART_IntConfig(HT_USARTx, USART_INT_TXDE ,ENABLE or DISABLE) // 发送数据空中断   
  //NVIC_EnableIRQ(USART0_IRQn); // 使能中断
 
  USART_RxCmd(HT_USART0, ENABLE); // 使能USART接收数据
  USART_TxCmd(HT_USART0, ENABLE); // 使能USART发送数据
}
//------------------------------------------------------------------------------------------
//蓝牙（串口0） 发生数据空中断函数
void USART0_IRQHandler(void)
{
  u8 data;
  if( USART_GetFlagStatus(HT_USART0, USART_FLAG_RXDR) ) // 接收器 FIFO 就绪标志位
    {                                                                                                                                                                                    
     data = USART_ReceiveData(HT_USART0); // 接收数据时已经自动清除中断标志位了，不用手动清除。      
    }             
}
//------------------------------------------------------------------------------------------
//蓝牙（串口0） 接收数据就绪函数
void BLUETEETH_USART_Tx(const char* TxBuffer, u32 length)
{
  int i;
 
  for (i = 0; i < length; i++)
  {
    while (USART_GetFlagStatus(HT_USART0, USART_FLAG_TXC) == RESET); // 判断是否 发送完成
    USART_SendData(HT_USART0, TxBuffer[i]);
    //while (USART_GetFlagStatus(HT_USARTx, USART_FLAG_TXDE) == RESET); // 判断是否 发送完成
  }
}



