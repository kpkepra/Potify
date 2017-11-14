#ifndef __USART1_H
#define	__USART1_H


#include "stm32f10x.h"
#include <stdio.h>



#define             USART_BAUD_RATE                       115200

#define             USARTx                                USART1
#define             USART_APBxClock_FUN                   RCC_APB2PeriphClockCmd
#define             USART_CLK                             RCC_APB2Periph_USART1
#define             USART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             USART_GPIO_CLK                        RCC_APB2Periph_GPIOA     
#define             USART_TX_PORT                         GPIOA   
#define             USART_TX_PIN                          GPIO_Pin_9
#define             USART_RX_PORT                         GPIOA 
#define             USART_RX_PIN                          GPIO_Pin_10
#define             USART_IRQ                             USART1_IRQn
#define             USART_INT_FUN                         USART1_IRQHandler



void                USARTx_Config                           ( void );



#endif /* __USART1_H */
