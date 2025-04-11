#include "ht32f5xxxx_01.h"              // Device header

#include "BM32S2031_1.h"


//使用串口UART1
uint8_t BiZhang_Init(void)
{
 if(BM32S2031_1_selSerial(UARTM_CH1) == BM32S2031_1_FAILURE) return 1;
BM32S2031_1_Init();
 BM32S2031_1_setMode(0x80);
BM32S2031_1_setIROutputTime(0x03);
 BM32S2031_1_setIRDebounce(15);
	BM32S2031_1_setIRThreshold(0x02);
 BM32S2031_1_saveSettings();
	return 0;


}
uint8_t Bihang_learning(void)
{
   return BM32S2031_1_distanceLearning();
}
