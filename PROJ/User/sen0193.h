#include "stm32f10x.h"
#include "stm32f10x_adc.h"

void SEN0193_Init(void);
u16 getADC(u8 ch);
u16 getADCAvg(u8 ch, u8 n);