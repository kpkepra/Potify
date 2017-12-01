#ifndef __TICKS_H
#define __TICKS_H

#include "stm32f10x_tim.h"

void ticks_init(void);
void ticks_reset(void);
u16 get_second_ticks(void);
u32 get_real_ticks(void);

#endif	/* __TICKS_H */
