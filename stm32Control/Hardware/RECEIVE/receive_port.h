#ifndef __RECEIVE_PORT_H
#define __RECEIVE_PORT_H	 
#include "sys.h"



#define DataGPIO GPIOA
#define Data GPIO_Pin_10
#define Dataout GPIO_Pin_9

extern s16 data;
extern u16 data1;
extern u16 data2;
extern s16 out;
extern u16 sig;
extern u16 dire;
extern u16 flag;


void uart1_Init(void);
void USART1_IRQHandler(void);	
void USART1_IRQHandler_dire(void);	
void USART1_IRQHandler_count(void);

#endif
