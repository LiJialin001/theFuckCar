#include "motr.h"

u16 out0=3200;

void Wheel_init(void) {    //wheel的接口初始化
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO的初始化枚举结构		
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);       
    GPIO_InitStructure.GPIO_Pin = Wheel_1 | Wheel_2 | Wheel_1_on | Wheel_1_back | Wheel_2_on | Wheel_2_back; //选择端口号（0~15或all）   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	GPIO_Init(WHEEL, &GPIO_InitStructure);	
}

//小车行进，PID由openmv调控
void Move_on() {
	GPIO_WriteBit(WHEEL, Wheel_1_on, (BitAction)(0));   //right
	GPIO_WriteBit(WHEEL, Wheel_1_back, (BitAction)(1)); 
	GPIO_WriteBit(WHEEL, Wheel_2_on, (BitAction)(0)); 
	GPIO_WriteBit(WHEEL, Wheel_2_back, (BitAction)(1)); 
	TIM_SetCompare3(TIM3,out0*sig+out*10); //right  
	TIM_SetCompare3(TIM4,out0*sig-out*10); //left  
}

void Move_right(){
  GPIO_WriteBit(WHEEL, Wheel_1_on, (BitAction)(0));   
	GPIO_WriteBit(WHEEL, Wheel_1_back, (BitAction)(1)); 
	GPIO_WriteBit(WHEEL, Wheel_2_on, (BitAction)(1)); 
	GPIO_WriteBit(WHEEL, Wheel_2_back, (BitAction)(0)); 
	TIM_SetCompare3(TIM3,3000); 
	TIM_SetCompare3(TIM4,0); 
    delay_ms(920);
	TIM_SetCompare3(TIM3,0); 
	TIM_SetCompare3(TIM4,0); 
	return ;

}

void Move_left(){
  GPIO_WriteBit(WHEEL, Wheel_1_on, (BitAction)(1)); 
	GPIO_WriteBit(WHEEL, Wheel_1_back, (BitAction)(0)); 
	GPIO_WriteBit(WHEEL, Wheel_2_on, (BitAction)(0)); 
	GPIO_WriteBit(WHEEL, Wheel_2_back, (BitAction)(1)); 
	TIM_SetCompare3(TIM3,0); 
	TIM_SetCompare3(TIM4,3000); 
	delay_ms(920);
	TIM_SetCompare3(TIM3,0); 
	TIM_SetCompare3(TIM4,0); 
	return ;

}

void Move_back(){
    Move_on();
    delay_ms(300)
    
    GPIO_WriteBit(WHEEL, Wheel_1_on, (BitAction)(1));
	GPIO_WriteBit(WHEEL, Wheel_1_back, (BitAction)(0)); 
	GPIO_WriteBit(WHEEL, Wheel_2_on, (BitAction)(0)); 
	GPIO_WriteBit(WHEEL, Wheel_2_back, (BitAction)(1)); 
	TIM_SetCompare3(TIM3,3000); //right  r
	TIM_SetCompare3(TIM4,3000); //left  l
	delay_ms(800);
	TIM_SetCompare3(TIM3,0); //right  r
	TIM_SetCompare3(TIM4,0); //left  l
	return ;

}
void TIM3_PWM_Init(u16 arr,u16 psc){  //TIM3 PWM初始化 arr重装载值 psc预分频系数
    GPIO_InitTypeDef     GPIO_InitStrue;
    TIM_OCInitTypeDef     TIM_OCInitStrue;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//使能TIM3和相关GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOB时钟(LED在PB0引脚)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//使能AFIO时钟(定时器3通道3需要重映射到BP5引脚)
    
    GPIO_InitStrue.GPIO_Pin=Wheel_1;     // TIM_CH3
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // 复用推挽
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //设置最大输出速度
    GPIO_Init(GPIOB,&GPIO_InitStrue);                //GPIO端口初始化设置
    
//    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE); //映射，重映射只用于64、100、144脚单片机
   //当没有重映射时，TIM3的四个通道CH1，CH2，CH3，CH4分别对应PA6，PA7,PB0,PB1
   //当部分重映射时，TIM3的四个通道CH1，CH2，CH3，CH4分别对应PB4，PB5,PB0,PB1 (GPIO_PartialRemap_TIM3)
   //当完全重映射时，TIM3的四个通道CH1，CH2，CH3，CH4分别对应PC6，PC7,PC8,PC9 (GPIO_FullRemap_TIM3) 
	      
    TIM_TimeBaseInitStrue.TIM_Period=arr;    //设置自动重装载值
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc;        //预分频系数
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;    //计数器向上溢出
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1;        //时钟的分频因子，起到了一点点的延时作用，一般设为TIM_CKD_DIV1
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);        //TIM3初始化设置(设置PWM的周期)
    
    TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM1;        // PWM模式1:CNT < CCR时输出有效电平
    TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;// 设置极性-有效电平为：高电平
    TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;// 输出使能
    TIM_OC3Init(TIM3,&TIM_OCInitStrue);        //TIM3的通道3 PWM 模式设置

    TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);        //使能预装载寄存器
    
    TIM_Cmd(TIM3,ENABLE);        //使能TIM3
		
}

void TIM4_PWM_Init(u16 arr,u16 psc){  //TIM3 PWM初始化 arr重装载值 psc预分频系数
    GPIO_InitTypeDef     GPIO_InitStrue;
    TIM_OCInitTypeDef     TIM_OCInitStrue;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//使能TIM4和相关GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOB时钟(LED在PB0引脚)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//使能AFIO时钟(定时器3通道3需要重映射到BP5引脚)
    
    GPIO_InitStrue.GPIO_Pin=Wheel_2;     // TIM_CH3
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // 复用推挽
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //设置最大输出速度
    GPIO_Init(GPIOB,&GPIO_InitStrue);                //GPIO端口初始化设置
    
//    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE); //映射，重映射只用于64、100、144脚单片机
   //当没有重映射时，TIM3的四个通道CH1，CH2，CH3，CH4分别对应PA6，PA7,PB0,PB1
   //当部分重映射时，TIM3的四个通道CH1，CH2，CH3，CH4分别对应PB4，PB5,PB0,PB1 (GPIO_PartialRemap_TIM3)
   //当完全重映射时，TIM3的四个通道CH1，CH2，CH3，CH4分别对应PC6，PC7,PC8,PC9 (GPIO_FullRemap_TIM3) 
	      
    TIM_TimeBaseInitStrue.TIM_Period=arr;    //设置自动重装载值
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc;        //预分频系数
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;    //计数器向上溢出
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1;        //时钟的分频因子，起到了一点点的延时作用，一般设为TIM_CKD_DIV1
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStrue);        //TIM3初始化设置(设置PWM的周期)
    
    TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM1;        // PWM模式1:CNT < CCR时输出有效电平
    TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;// 设置极性-有效电平为：高电平
    TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;// 输出使能
    TIM_OC3Init(TIM4,&TIM_OCInitStrue);        //TIM3的通道3 PWM 模式设置

    TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);        //使能预装载寄存器
    
    TIM_Cmd(TIM4,ENABLE);        //使能TIM3
		
}

