#ifndef __MOTR_H
#define __MOTR_H

#include "sys.h"
#include "stm32f10x.h"
#include "receive_port.h"
#include "delay.h"

#define WHEEL GPIOB
#define Wheel_1 GPIO_Pin_0
#define Wheel_1_on GPIO_Pin_10
#define Wheel_1_back GPIO_Pin_12
#define Wheel_2 GPIO_Pin_8
#define Wheel_2_on GPIO_Pin_11
#define Wheel_2_back GPIO_Pin_13


void Wheel_init(void);
void Move_on(void);
void Move_left(void);
void Move_right(void);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);
s16 myabs(s16 out);



#endif
