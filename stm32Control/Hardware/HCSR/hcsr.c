#include "hcsr.h"
#include "delay.h"


int N = 0;
float distance = 0;

void Hcsr_Init(void) {
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//Trig
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//Echo
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
  GPIO_Init(GPIOB, &GPIO_InitStructure); //50000*72/72000000=500ms
	TIM_TimeBaseStructure.TIM_Period = 49999;
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	
	TIM_Cmd(TIM2, ENABLE);
	
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
	     N += 1;		   
	}
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
}

int count=0;
float Distance(void)
{
	  GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		GPIO_SetBits(GPIOB, GPIO_Pin_6);
		delay_us(20);
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0);
		TIM2->CNT=0;
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 1);
		count=TIM2->CNT;
	  //count=N;
		distance=(float)count/58;					
		return distance;
}
