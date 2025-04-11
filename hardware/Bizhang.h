#ifndef _BIZHANG_H_
#define _BIZHANG_H_

uint8_t Bihang_learning(void);

uint8_t BM32S2031_1_getIRStatus(void);//是否靠近
int16_t BM32S2031_1_readIRSignalValue(void);//获得查值

uint8_t  _BM32S2031_1_readIRRef(void);//获得参考值
uint8_t  _BM32S2031_1_readIREnv(void);//获得环境值

uint8_t BiZhang_Init(void);

#endif
