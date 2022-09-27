/*********************************************************************************************


Ӳ����	STM32F103C8   �ⲿ����8MHz RCC����������Ƶ72MHz��  

 # ��ģ�������STM32F103�ڲ���RCCʱ������

*********************************************************************************************/
#include "stm32f10x.h" //STM32ͷ�ļ�
#include "sys.h"
#include "delay.h"
#include "hcsr.h"
#include "motr.h"


#define FEEL GPIOA
#define FEELPower GPIOC
#define Led_Left GPIO_Pin_0
#define Led_Right GPIO_Pin_1
#define Led1 GPIO_Pin_13
#define Led2 GPIO_Pin_14

#define ADC1_DR_Address    ((uint32_t)0x4001244C) //ADC1�������ĵ�ַ
#define ADCPORT		GPIOA	//����ADC�ӿ�
#define ADC_CH_Left		GPIO_Pin_4
#define ADC_CH_Right		GPIO_Pin_5	//����ADC�ӿ�

#define Speed GPIOB
#define Speed_Left_Power GPIO_Pin_3
#define Speed_Right_Power GPIO_Pin_4
#define Respeed_Left GPIO_Pin_5
#define Respeed_Right GPIO_Pin_9


u8 INT_MARK;//�жϱ�־λ

vu16 ADC_DMA_IN[2]; 

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
	RCC_Configuration(); //ʱ������
	ADC_Configuration(); //ADC��ʼ������
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
			Move_on(5600, 5400);
	}
}

void SpeedPower_init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure; //����GPIO�ĳ�ʼ��ö�ٽṹ		
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);       
    GPIO_InitStructure.GPIO_Pin = Speed_Left_Power | Speed_Right_Power;  //ѡ��˿ںţ�0~15��all��   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //ѡ��IO�ӿڹ�����ʽ       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��    
	GPIO_Init(Speed, &GPIO_InitStructure);	
}

void FEELPower_init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure; //����GPIO�ĳ�ʼ��ö�ٽṹ		
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);       
    GPIO_InitStructure.GPIO_Pin = Led1 | Led2; //ѡ��˿ںţ�0~15��all��   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //ѡ��IO�ӿڹ�����ʽ       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��    
	GPIO_Init(FEELPower, &GPIO_InitStructure);	
}

void Respeed_init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);       
  GPIO_InitStructure.GPIO_Pin = Led_Left | Led_Right;                      
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //ѡ��IO�ӿڹ�����ʽ //��������       
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��    
	GPIO_Init(Speed,&GPIO_InitStructure);			
}

void GND_init(void) { 
	GPIO_InitTypeDef  GPIO_InitStructure; //����GPIO�ĳ�ʼ��ö�ٽṹ		
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);       
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; //ѡ��˿ںţ�0~15��all��   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //ѡ��IO�ӿڹ�����ʽ       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��    
	GPIO_Init(FEEL, &GPIO_InitStructure);	
}

void Led_init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);       
  GPIO_InitStructure.GPIO_Pin = Led_Left | Led_Right;                      
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //ѡ��IO�ӿڹ�����ʽ //��������       
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��    
	GPIO_Init(FEEL,&GPIO_InitStructure);			
}


void ADC_DMA_Init(void){ //DMA��ʼ������
	DMA_InitTypeDef DMA_InitStructure;//����DMA��ʼ���ṹ��
	DMA_DeInit(DMA1_Channel1);//��λDMAͨ��1
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address; //���� DMAͨ���������ַ=ADC1_DR_Address
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_DMA_IN; //!!!����DMAͨ��ADC���ݴ洢��������������ֱ�Ӷ��˱�������ADCֵ��
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//ָ������ΪԴ��ַ
	DMA_InitStructure.DMA_BufferSize = 2;//!!!����DMA��������С������ADC�ɼ�ͨ�������޸ģ�
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//��ǰ����Ĵ�����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//!!! ��ǰ�洢����ַ��Disable���䣬Enable���������ڶ�ͨ���ɼ���
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�����������ݿ��16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //����洢�����ݿ��16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMAͨ������ģʽλ���λ���ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMAͨ�����ȼ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//��ֹDMAͨ���洢�����洢������
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//��ʼ��DMAͨ��1
	DMA_Cmd(DMA1_Channel1, ENABLE); //ʹ��DMAͨ��1
}
void ADC_GPIO_Init(void){ //GPIO��ʼ������
	GPIO_InitTypeDef  GPIO_InitStructure; 	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);       
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//ʹ��DMAʱ�ӣ�����ADC�����ݴ��ͣ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//ʹ��ADC1ʱ��
    GPIO_InitStructure.GPIO_Pin = ADC_CH_Left | ADC_CH_Right; //!!!ѡ��˿�                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //ѡ��IO�ӿڹ�����ʽ       
	GPIO_Init(ADCPORT, &GPIO_InitStructure);			
}
void ADC_Configuration(void){ //��ʼ������
	ADC_InitTypeDef ADC_InitStructure;//����ADC��ʼ���ṹ�����
	ADC_GPIO_Init();//GPIO��ʼ������
	ADC_DMA_Init();//DMA��ʼ������
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; //ʹ��ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//ADCת������������ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//���������ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ת�������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;//!!!˳����й���ת����ADCͨ������Ŀ������ADC�ɼ�ͨ�������޸ģ�
	ADC_Init(ADC1, &ADC_InitStructure); //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���
	//����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	//ADC1,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ28����		 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_28Cycles5);//!!! ADC1ѡ���ŵ�x,����˳��y,����ʱ��n������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_28Cycles5);//!!! ADC1ѡ���ŵ�x,����˳��y,����ʱ��n������

	ADC_DMACmd(ADC1, ENABLE);// ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
	ADC_Cmd(ADC1, ENABLE);//ʹ��ADC1
	ADC_ResetCalibration(ADC1); //����ADC1У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));//�ȴ�ADC1У׼�������
	ADC_StartCalibration(ADC1);//��ʼADC1У׼
	while(ADC_GetCalibrationStatus(ADC1));//�ȴ�ADC1У׼���
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ʹ��ADC1�����ʼת��
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

