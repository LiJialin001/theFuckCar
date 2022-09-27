#ifndef __RECEIVE_PORT_H
#define __RECEIVE_PORT_H	 
#include "sys.h"

#define DataGPIO GPIOA
#define Data GPIO_Pin_10
u16 data;
u16 data2;
u16 out;
u16 dire;
u16 flag;

void uart1_Init(void);
void USART1_IRQHandler(void);		

#endif
