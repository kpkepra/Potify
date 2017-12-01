#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"	  
#include "stm32f10x_tim.h"
#include "delay.h"

typedef enum{
	MOTOR1,
	MOTOR2,
	MOTOR3
}MOTOR_ID;

void motor_init(u16 presc, u16 count, u16 init_val);
void motor_control(MOTOR_ID motor_id, u16 magnitude);
#endif
