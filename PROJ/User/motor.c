#include "motor.h"
#define MOTOR_TIM	TIM3

void motor_init(u16 presc, u16 count, u16 init_val) {
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	/*
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	*/
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	/*
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	*/
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_Prescaler = presc;
	TIM_TimeBaseStructure.TIM_Period = count;
	TIM_TimeBaseInit(MOTOR_TIM, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = init_val;
	
	TIM_OC1Init(MOTOR_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(MOTOR_TIM, ENABLE);
	
	TIM_OC2Init(MOTOR_TIM, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(MOTOR_TIM, ENABLE);

	TIM_OC3Init(MOTOR_TIM, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(MOTOR_TIM, ENABLE);
	
	TIM_ARRPreloadConfig(MOTOR_TIM, ENABLE);
	TIM_Cmd(MOTOR_TIM, ENABLE);	
	TIM_SetCounter(MOTOR_TIM,0);
	TIM_CtrlPWMOutputs(MOTOR_TIM, ENABLE);
}

void motor_control(MOTOR_ID motor_id, u16 magnitude){
	if (motor_id == MOTOR1) {
		TIM_SetCompare1(MOTOR_TIM,magnitude);
	}
	if (motor_id == MOTOR2) {
		TIM_SetCompare2(MOTOR_TIM,magnitude);
	}
	if (motor_id == MOTOR3) {
		TIM_SetCompare3(MOTOR_TIM,magnitude);
	}
}
