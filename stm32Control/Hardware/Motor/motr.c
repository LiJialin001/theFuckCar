#include "motr.h"

u16 out0=4000;

void Wheel_init(void) {    //wheel�Ľӿڳ�ʼ��
	GPIO_InitTypeDef  GPIO_InitStructure; //����GPIO�ĳ�ʼ��ö�ٽṹ		
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);       
    GPIO_InitStructure.GPIO_Pin = Wheel_1 | Wheel_2 | Wheel_1_on | Wheel_1_back | Wheel_2_on | Wheel_2_back; //ѡ��˿ںţ�0~15��all��   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //ѡ��IO�ӿڹ�����ʽ       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��    
	GPIO_Init(WHEEL, &GPIO_InitStructure);	
}

//С���н���PID��openmv����
void Move_on() {
	GPIO_WriteBit(WHEEL, Wheel_1_on, (BitAction)(0));   //right
	GPIO_WriteBit(WHEEL, Wheel_1_back, (BitAction)(1)); 
	GPIO_WriteBit(WHEEL, Wheel_2_on, (BitAction)(0)); 
	GPIO_WriteBit(WHEEL, Wheel_2_back, (BitAction)(1)); 
	TIM_SetCompare3(TIM3,out0*sig-out*10); //right  
	TIM_SetCompare3(TIM4,out0*sig+out*10); //left  
}

void Move_left(){
  GPIO_WriteBit(WHEEL, Wheel_1_on, (BitAction)(0));   //right
	GPIO_WriteBit(WHEEL, Wheel_1_back, (BitAction)(1)); 
	GPIO_WriteBit(WHEEL, Wheel_2_on, (BitAction)(1)); 
	GPIO_WriteBit(WHEEL, Wheel_2_back, (BitAction)(0)); 
	TIM_SetCompare3(TIM3,out0*sig-out*30); //right  r
	TIM_SetCompare3(TIM4,out0*sig+out*30); //left  l
}

void Move_right(){
  GPIO_WriteBit(WHEEL, Wheel_1_on, (BitAction)(1));   //right
	GPIO_WriteBit(WHEEL, Wheel_1_back, (BitAction)(0)); 
	GPIO_WriteBit(WHEEL, Wheel_2_on, (BitAction)(0)); 
	GPIO_WriteBit(WHEEL, Wheel_2_back, (BitAction)(1)); 
	TIM_SetCompare3(TIM3,out0*sig-out*30); //right  r
	TIM_SetCompare3(TIM4,out0*sig+out*30); //left  l
}


void TIM3_PWM_Init(u16 arr,u16 psc){  //TIM3 PWM��ʼ�� arr��װ��ֵ pscԤ��Ƶϵ��
    GPIO_InitTypeDef     GPIO_InitStrue;
    TIM_OCInitTypeDef     TIM_OCInitStrue;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//ʹ��TIM3�����GPIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIOBʱ��(LED��PB0����)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//ʹ��AFIOʱ��(��ʱ��3ͨ��3��Ҫ��ӳ�䵽BP5����)
    
    GPIO_InitStrue.GPIO_Pin=Wheel_1;     // TIM_CH3
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // ��������
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //�����������ٶ�
    GPIO_Init(GPIOB,&GPIO_InitStrue);                //GPIO�˿ڳ�ʼ������
    
//    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE); //ӳ�䣬��ӳ��ֻ����64��100��144�ŵ�Ƭ��
   //��û����ӳ��ʱ��TIM3���ĸ�ͨ��CH1��CH2��CH3��CH4�ֱ��ӦPA6��PA7,PB0,PB1
   //��������ӳ��ʱ��TIM3���ĸ�ͨ��CH1��CH2��CH3��CH4�ֱ��ӦPB4��PB5,PB0,PB1 (GPIO_PartialRemap_TIM3)
   //����ȫ��ӳ��ʱ��TIM3���ĸ�ͨ��CH1��CH2��CH3��CH4�ֱ��ӦPC6��PC7,PC8,PC9 (GPIO_FullRemap_TIM3) 
	      
    TIM_TimeBaseInitStrue.TIM_Period=arr;    //�����Զ���װ��ֵ
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc;        //Ԥ��Ƶϵ��
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;    //�������������
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1;        //ʱ�ӵķ�Ƶ���ӣ�����һ������ʱ���ã�һ����ΪTIM_CKD_DIV1
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);        //TIM3��ʼ������(����PWM������)
    
    TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM1;        // PWMģʽ1:CNT < CCRʱ�����Ч��ƽ
    TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;// ���ü���-��Ч��ƽΪ���ߵ�ƽ
    TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;// ���ʹ��
    TIM_OC3Init(TIM3,&TIM_OCInitStrue);        //TIM3��ͨ��3 PWM ģʽ����

    TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);        //ʹ��Ԥװ�ؼĴ���
    
    TIM_Cmd(TIM3,ENABLE);        //ʹ��TIM3
		
}

void TIM4_PWM_Init(u16 arr,u16 psc){  //TIM3 PWM��ʼ�� arr��װ��ֵ pscԤ��Ƶϵ��
    GPIO_InitTypeDef     GPIO_InitStrue;
    TIM_OCInitTypeDef     TIM_OCInitStrue;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//ʹ��TIM4�����GPIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIOBʱ��(LED��PB0����)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//ʹ��AFIOʱ��(��ʱ��3ͨ��3��Ҫ��ӳ�䵽BP5����)
    
    GPIO_InitStrue.GPIO_Pin=Wheel_2;     // TIM_CH3
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // ��������
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //�����������ٶ�
    GPIO_Init(GPIOB,&GPIO_InitStrue);                //GPIO�˿ڳ�ʼ������
    
//    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE); //ӳ�䣬��ӳ��ֻ����64��100��144�ŵ�Ƭ��
   //��û����ӳ��ʱ��TIM3���ĸ�ͨ��CH1��CH2��CH3��CH4�ֱ��ӦPA6��PA7,PB0,PB1
   //��������ӳ��ʱ��TIM3���ĸ�ͨ��CH1��CH2��CH3��CH4�ֱ��ӦPB4��PB5,PB0,PB1 (GPIO_PartialRemap_TIM3)
   //����ȫ��ӳ��ʱ��TIM3���ĸ�ͨ��CH1��CH2��CH3��CH4�ֱ��ӦPC6��PC7,PC8,PC9 (GPIO_FullRemap_TIM3) 
	      
    TIM_TimeBaseInitStrue.TIM_Period=arr;    //�����Զ���װ��ֵ
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc;        //Ԥ��Ƶϵ��
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;    //�������������
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1;        //ʱ�ӵķ�Ƶ���ӣ�����һ������ʱ���ã�һ����ΪTIM_CKD_DIV1
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStrue);        //TIM3��ʼ������(����PWM������)
    
    TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM1;        // PWMģʽ1:CNT < CCRʱ�����Ч��ƽ
    TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;// ���ü���-��Ч��ƽΪ���ߵ�ƽ
    TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;// ���ʹ��
    TIM_OC3Init(TIM4,&TIM_OCInitStrue);        //TIM3��ͨ��3 PWM ģʽ����

    TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);        //ʹ��Ԥװ�ؼĴ���
    
    TIM_Cmd(TIM4,ENABLE);        //ʹ��TIM3
		
}

