#include "sen0193.h"

GPIO_InitTypeDef GPIO_LDRStructure;
ADC_InitTypeDef ADC_LDRStructure;

void LDR_Init(void) {
	// PC.4 - ANALOG
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_LDRStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_LDRStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_LDRStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_LDRStructure);
	
	/*
		Mode:	Independent
		Scan Conversion Mode:	Disable
		Continuous Conversion Mode:	Disable
		External Trigger Conversion:	None
		Data Alignment: Right
		Number of Channel: 1
		*/
	ADC_DeInit(ADC2);
	
	ADC_LDRStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_LDRStructure.ADC_ScanConvMode = DISABLE;
	ADC_LDRStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_LDRStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_LDRStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_LDRStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADC_LDRStructure);
	
	// Connect Ch14 to ADC2, Enable ADC2
	ADC_Cmd(ADC2, ENABLE);
	
	// Self-Calibration
	ADC_ResetCalibration(ADC2);
	while(ADC_GetResetCalibrationStatus(ADC2));
	ADC_StartCalibration(ADC2);
	while(ADC_GetCalibrationStatus(ADC2));
}

u16 getVal_LDR(u8 ch) {
	ADC_RegularChannelConfig(ADC2, ADC_Channel_15, 1, ADC_SampleTime_1Cycles5);
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));
	
	return ADC_GetConversionValue(ADC2);
}