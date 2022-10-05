#include "stm32f10x.h" //STM32头文件
#include "sys.h"
#include "delay.h"
#include "motr.h"
#include "receive_port.h"


u8 INT_MARK;//中断标志位

vu16 ADC_DMA_IN[2]; 

void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);



int main (void){
	delay_ms(500);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_Configuration(); //时钟设置
	Wheel_init();
	uart1_Init();
	
	TIM3_PWM_Init(9999,2); 
	TIM4_PWM_Init(9999,2); 

	while(1){
		USART1_IRQHandler();
    USART1_IRQHandler_dire();	
		if (dire==1)
		{
			Move_left();
		}
		else if(dire==2)
		{
			Move_right();
		}
		else
		{
			Move_on();
		}
	}
}




/*
【变量定义】
u32     a; //定义32位无符号变量a
u16     a; //定义16位无符号变量a
u8     a; //定义8位无符号变量a
vu32     a; //定义易变的32位无符号变量a
vu16     a; //定义易变的 16位无符号变量a
vu8     a; //定义易变的 8位无符号变量a
uc32     a; //定义只读的32位无符号变量a
uc16     a; //定义只读 的16位无符号变量a
uc8     a; //定义只读 的8位无符号变量a

#define ONE  1   //宏定义

delay_us(1); //延时1微秒
delay_ms(1); //延时1毫秒
delay_s(1); //延时1秒
*/

