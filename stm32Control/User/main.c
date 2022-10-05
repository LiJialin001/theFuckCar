#include "stm32f10x.h" //STM32ͷ�ļ�
#include "sys.h"
#include "delay.h"
#include "motr.h"
#include "receive_port.h"


u8 INT_MARK;//�жϱ�־λ

vu16 ADC_DMA_IN[2]; 

void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);



int main (void){
	delay_ms(500);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_Configuration(); //ʱ������
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
���������塿
u32     a; //����32λ�޷��ű���a
u16     a; //����16λ�޷��ű���a
u8     a; //����8λ�޷��ű���a
vu32     a; //�����ױ��32λ�޷��ű���a
vu16     a; //�����ױ�� 16λ�޷��ű���a
vu8     a; //�����ױ�� 8λ�޷��ű���a
uc32     a; //����ֻ����32λ�޷��ű���a
uc16     a; //����ֻ�� ��16λ�޷��ű���a
uc8     a; //����ֻ�� ��8λ�޷��ű���a

#define ONE  1   //�궨��

delay_us(1); //��ʱ1΢��
delay_ms(1); //��ʱ1����
delay_s(1); //��ʱ1��
*/

