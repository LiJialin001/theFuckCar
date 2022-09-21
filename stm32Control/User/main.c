/*********************************************************************************************


硬件：	STM32F103C8   外部晶振8MHz RCC函数设置主频72MHz　  

 # 本模板加载了STM32F103内部的RCC时钟设置

*********************************************************************************************/
#include "stm32f10x.h" //STM32头文件
#include "sys.h"
#include "delay.h"
#include "hcsr.h"


#define WHEEL GPIOB
#define Wheel_1 GPIO_Pin_0
#define Wheel_1_on GPIO_Pin_10
#define Wheel_1_back GPIO_Pin_12
#define Wheel_2 GPIO_Pin_8
#define Wheel_2_on GPIO_Pin_11
#define Wheel_2_back GPIO_Pin_13

#define FEEL GPIOA
#define FEELPower GPIOC
#define Led_Left GPIO_Pin_0
#define Led_Right GPIO_Pin_1
#define Led1 GPIO_Pin_13
#define Led2 GPIO_Pin_14

#define ADC1_DR_Address    ((uint32_t)0x4001244C) //ADC1这个外设的地址
#define ADCPORT		GPIOA	//定义ADC接口
#define ADC_CH_Left		GPIO_Pin_4
#define ADC_CH_Right		GPIO_Pin_5	//定义ADC接口

#define Speed GPIOB
#define Speed_Left_Power GPIO_Pin_3
#define Speed_Right_Power GPIO_Pin_4
#define Respeed_Left GPIO_Pin_5
#define Respeed_Right GPIO_Pin_9


u8 INT_MARK;//中断标志位

vu16 ADC_DMA_IN[2]; 

//void Wheel_Control_INT_INIT (void);
void Wheel_init(void);

void Led_init(void);
void FEELPower_init(void);
void SpeedPower_init(void);

void GND_init(void);

void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);

void ADC_DMA_Init(void);
void ADC_GPIO_Init(void);
void ADC_Configuration(void);



int main (void){
	delay_ms(500);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_Configuration(); //时钟设置
	ADC_Configuration(); //ADC初始化设置
	Wheel_init();
	Hcsr_Init();
	Led_init();
	FEELPower_init();
	SpeedPower_init();
	
	GND_init();
	
	GPIO_WriteBit(FEELPower, Led1, (BitAction)(1)); 
	GPIO_WriteBit(FEELPower, Led2, (BitAction)(1)); 
	
	TIM3_PWM_Init(9999,2); 
	TIM4_PWM_Init(9999,2); 

	while(1){
		
		
		//pwm
	/*	if (GPIO_ReadInputDataBit(FEEL, Led_Left) && !GPIO_ReadInputDataBit(FEEL, Led_Right) ){
			GPIO_WriteBit(WHEEL, Wheel_1_on, (BitAction)(0)); 
	    GPIO_WriteBit(WHEEL, Wheel_1_back, (BitAction)(1)); 
			GPIO_WriteBit(WHEEL, Wheel_2_on, (BitAction)(1)); 
	    GPIO_WriteBit(WHEEL, Wheel_2_back, (BitAction)(0)); 
			TIM_SetCompare3(TIM3,2500); 
			TIM_SetCompare3(TIM4,5400); 
		} else if (GPIO_ReadInputDataBit(FEEL, Led_Right) && !GPIO_ReadInputDataBit(FEEL, Led_Left)) {
			GPIO_WriteBit(WHEEL, Wheel_1_on, (BitAction)(1)); 
	    GPIO_WriteBit(WHEEL, Wheel_1_back, (BitAction)(0)); 
			GPIO_WriteBit(WHEEL, Wheel_2_on, (BitAction)(0)); 
	    GPIO_WriteBit(WHEEL, Wheel_2_back, (BitAction)(1)); 
			TIM_SetCompare3(TIM3,5600); 
			TIM_SetCompare3(TIM4,2000); 
		} else if (GPIO_ReadInputDataBit(FEEL, Led_Right) && GPIO_ReadInputDataBit(FEEL, Led_Left)) {
			TIM_SetCompare3(TIM3,0); 
			TIM_SetCompare3(TIM4,0); 
		}else {
			*/
			if (GPIO_ReadInputDataBit(Speed, Respeed_Left)) {}
			GPIO_WriteBit(WHEEL, Wheel_1_on, (BitAction)(1)); 
	    GPIO_WriteBit(WHEEL, Wheel_1_back, (BitAction)(0)); 
			GPIO_WriteBit(WHEEL, Wheel_2_on, (BitAction)(1)); 
	    GPIO_WriteBit(WHEEL, Wheel_2_back, (BitAction)(0)); 
			TIM_SetCompare3(TIM3,5600); 
			TIM_SetCompare3(TIM4,5400); 
	//	}
	}
}

void SpeedPower_init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO的初始化枚举结构		
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);       
    GPIO_InitStructure.GPIO_Pin = Speed_Left_Power | Speed_Right_Power;  //选择端口号（0~15或all）   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	GPIO_Init(Speed, &GPIO_InitStructure);	
}

void FEELPower_init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO的初始化枚举结构		
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);       
    GPIO_InitStructure.GPIO_Pin = Led1 | Led2; //选择端口号（0~15或all）   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	GPIO_Init(FEELPower, &GPIO_InitStructure);	
}

void Wheel_init(void) { //wheel的接口初始化
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO的初始化枚举结构		
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);       
    GPIO_InitStructure.GPIO_Pin = Wheel_1 | Wheel_2 | Wheel_1_on | Wheel_1_back | Wheel_2_on | Wheel_2_back; //选择端口号（0~15或all）   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	GPIO_Init(WHEEL, &GPIO_InitStructure);	
}

void Respeed_init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);       
  GPIO_InitStructure.GPIO_Pin = Led_Left | Led_Right;                      
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //选择IO接口工作方式 //上拉电阻       
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	GPIO_Init(Speed,&GPIO_InitStructure);			
}

void GND_init(void) { 
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO的初始化枚举结构		
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);       
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; //选择端口号（0~15或all）   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //选择IO接口工作方式       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	GPIO_Init(FEEL, &GPIO_InitStructure);	
}

void Led_init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);       
  GPIO_InitStructure.GPIO_Pin = Led_Left | Led_Right;                      
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //选择IO接口工作方式 //上拉电阻       
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	GPIO_Init(FEEL,&GPIO_InitStructure);			
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





void ADC_DMA_Init(void){ //DMA初始化设置
	DMA_InitTypeDef DMA_InitStructure;//定义DMA初始化结构体
	DMA_DeInit(DMA1_Channel1);//复位DMA通道1
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address; //定义 DMA通道外设基地址=ADC1_DR_Address
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_DMA_IN; //!!!定义DMA通道ADC数据存储器（其他函数可直接读此变量即是ADC值）
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//指定外设为源地址
	DMA_InitStructure.DMA_BufferSize = 2;//!!!定义DMA缓冲区大小（根据ADC采集通道数量修改）
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//当前外设寄存器地址不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//!!! 当前存储器地址：Disable不变，Enable递增（用于多通道采集）
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//定义外设数据宽度16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //定义存储器数据宽度16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA通道操作模式位环形缓冲模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA通道优先级高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//禁止DMA通道存储器到存储器传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//初始化DMA通道1
	DMA_Cmd(DMA1_Channel1, ENABLE); //使能DMA通道1
}
void ADC_GPIO_Init(void){ //GPIO初始化设置
	GPIO_InitTypeDef  GPIO_InitStructure; 	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);       
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//使能DMA时钟（用于ADC的数据传送）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//使能ADC1时钟
    GPIO_InitStructure.GPIO_Pin = ADC_CH_Left | ADC_CH_Right; //!!!选择端口                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //选择IO接口工作方式       
	GPIO_Init(ADCPORT, &GPIO_InitStructure);			
}
void ADC_Configuration(void){ //初始化设置
	ADC_InitTypeDef ADC_InitStructure;//定义ADC初始化结构体变量
	ADC_GPIO_Init();//GPIO初始化设置
	ADC_DMA_Init();//DMA初始化设置
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; //使能扫描
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//ADC转换工作在连续模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//有软件控制转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//转换数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;//!!!顺序进行规则转换的ADC通道的数目（根据ADC采集通道数量修改）
	ADC_Init(ADC1, &ADC_InitStructure); //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器
	//设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
	//ADC1,ADC通道x,规则采样顺序值为y,采样时间为28周期		 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_28Cycles5);//!!! ADC1选择信道x,采样顺序y,采样时间n个周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_28Cycles5);//!!! ADC1选择信道x,采样顺序y,采样时间n个周期

	ADC_DMACmd(ADC1, ENABLE);// 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
	ADC_Cmd(ADC1, ENABLE);//使能ADC1
	ADC_ResetCalibration(ADC1); //重置ADC1校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));//等待ADC1校准重置完成
	ADC_StartCalibration(ADC1);//开始ADC1校准
	while(ADC_GetCalibrationStatus(ADC1));//等待ADC1校准完成
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //使能ADC1软件开始转换
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

