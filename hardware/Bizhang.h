#ifndef _BIZHANG_H_
#define _BIZHANG_H_

uint8_t Bihang_learning(void);

uint8_t BM32S2031_1_getIRStatus(void);//�Ƿ񿿽�
int16_t BM32S2031_1_readIRSignalValue(void);//��ò�ֵ

uint8_t  _BM32S2031_1_readIRRef(void);//��òο�ֵ
uint8_t  _BM32S2031_1_readIREnv(void);//��û���ֵ

uint8_t BiZhang_Init(void);

#endif
