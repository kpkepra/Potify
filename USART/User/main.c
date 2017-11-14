#include "stm32f10x.h"
#include "lcd.h"
#include <stdio.h>
/*------------------------------------------------------------
STM32 USART Communication - dmo

Initialize the UART Communication interface of STM32 
STM32 to PC : When K1 is pressed, STM32 will send a 'A' to PC
              When K2 is pressed, STM32 will send a 'B' to PC
PC to STM32 : When '0' is pressed on keyboard the RGB Light will off
              When 'R' is pressed on keyboard the RGB Light will turn Red				 
              When 'G' is pressed on keyboard the RGB Light will turn Green				 
              When 'B' is pressed on keyboard the RGB Light will turn Blue				 


Before writing the function, please make sure in stm32f10x_conf.h, the following #include lines are uncommented
			"stm32f10x_fsmc.h" 
			"stm32f10x_gpio.h"
			"stm32f10x_rcc.h" 
			"stm32f10x_usart.h"
			"misc.h" 
-------------------------------------------------------------*/

void Delayus(int duration);
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

int main(void)
{	
	uint16_t rxdata = 0;
	
	//TX Bluetooth - A10
	//RX Bluetooth - A9
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

  GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5);
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	USART_Cmd(USART1, ENABLE); 
	
	while ( 1 ){
		Delayus(100000);
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1) {
			USART_SendData(USART1, 'A'); 
		}
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 1) {
			USART_SendData(USART1, 'B'); 
		}

		if (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET) {
			rxdata = USART_ReceiveData(USART1);
		  if (rxdata == 0x30) {
          GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5);
			}	
			else if (rxdata == 'R') {
		     GPIO_ResetBits(GPIOB, GPIO_Pin_5);		
         GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
			}	
			else if (rxdata == 'G') {
		     GPIO_ResetBits(GPIOB, GPIO_Pin_0);		
         GPIO_SetBits(GPIOB, GPIO_Pin_5 | GPIO_Pin_1);
			}	
			else if (rxdata == 'B') {
		     GPIO_ResetBits(GPIOB, GPIO_Pin_1);		
         GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_5);
			}	
		}
	}		
}

void Delayus(int duration)
{
		while(duration--) 
		{
			int i=0x02;				
			while(i--)
			__asm("nop");
		}
}
