#include "stm32f10x.h" //STM32ͷ�ļ�
#include "sys.h"
#include "delay.h"
#include "motr.h"
#include "receive_port.h"
#include "ooled.h"
#include "i2c.h"


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
	OLED_Init();
//  OLED0561_Init ();
//	I2C_Configuration();
//	
//	OLED_DISPLAY_8x16_BUFFER(0,"out is :");
//	OLED_DISPLAY_8x16_BUFFER(2, "sig is :");
//		OLED_DISPLAY_8x16_BUFFER(6, "dir is :");
	
	TIM3_PWM_Init(9999,2); 
	TIM4_PWM_Init(9999,2); 

	while(1){
		USART1_IRQHandler();
		if (out<0){
			u16 temp;
			OLED_ShowChar(1,1,'-');
			temp=-out;
			OLED_ShowChar(1,2,temp/10+0x30);
			OLED_ShowChar(1,3,temp%10+0x30);
		
		}
		else{
			OLED_ShowChar(1,1,out/10+0x30);
			OLED_ShowChar(1,2,out%10+0x30);

		}
		OLED_ShowChar(2,1,sig+0x30);
//		OLED_DISPLAY_8x16(0,8*8,out+0x30);
//	  OLED_DISPLAY_8x16(2,8*8,sig+0x30);
		
    USART1_IRQHandler_dire();	
//		 OLED_DISPLAY_8x16(6,8*8,dire+0x30);
		OLED_ShowChar(3,1,dire+0x30);
		
//		while(dire==0){
//			Move_on();
//			USART1_IRQHandler_dire();	
//		}
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

