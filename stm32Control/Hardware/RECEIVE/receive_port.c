#include "receive_port.h"

u16 data;
u16 data1;
u16 data2;
u16 out;
u16 sig;
u16 dire;
u16 flag;

void uart1_Init(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //��������
  GPIO_InitStructure.GPIO_Pin = Data;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DataGPIO,&GPIO_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;                  //���ò�����115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //�ֳ�8λ����ģʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;        //һλֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;           //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                  //�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure);   
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //ʹ���ж�
  USART_Cmd(USART1, ENABLE);                      //ʹ�ܴ���1
	USART_ClearFlag(USART1, USART_FLAG_TC);        // �崮��1
}


void USART1_IRQHandler(void)			 
{
		u8 com_data; 
		u8 i;
		static u8 RxCounter1=0;
		static u16 RxBuffer1[10]={0};
		static u8 RxState = 0;	
		static u8 RxFlag1 = 0;

		if( USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)  	   //�����ж�
		{
				USART_ClearITPendingBit(USART1,USART_IT_RXNE);   //����жϱ�־
				com_data = USART_ReceiveData(USART1);						 //���մ���1����
			
				if(RxState==0&&com_data==0xb3)  //0xb3֡ͷ��ʼ����
				{
					RxState=1;
					RxBuffer1[RxCounter1++]=com_data;
				}
		
				else if(RxState==1&&com_data==0xb3)  //0xb3֡ͷ
				{
					RxState=2;
					RxBuffer1[RxCounter1++]=com_data;
				}
		
				else if(RxState==2)
				{
					RxBuffer1[RxCounter1++]=com_data;

					if(RxCounter1>=10||com_data == 0x5B)       //RxBuffer1����������ս������յ�ֹͣλ
					{
						RxState=3;
						RxFlag1=1;
						data=RxBuffer1[RxCounter1-2];	
            data1=RxBuffer1[RxCounter1-3];						
					}
				}
		
				else if(RxState==3)		//����Ƿ��յ�������־
				{
						if(RxBuffer1[RxCounter1-1] == 0x5B)
						{
									USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);//�ر��ж�
									if(RxFlag1)
									{
										out = data1*(data-1);
									}
									RxFlag1 = 0;
									RxCounter1 = 0;
									RxState = 0;
									USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  //���ж�
						}
						else   //û�յ���ֹλ�����մ���
						{
									RxState = 0;
									RxCounter1=0;
									for(i=0;i<10;i++)
									{
											RxBuffer1[i]=0x00;      //����
									}
						}
				} 
	
				else   //�쳣
				{
						RxState = 0;
						RxCounter1=0;
						for(i=0;i<10;i++)
						{
								RxBuffer1[i]=0x00;    
						}
				}

		}
		
}



void USART1_IRQHandler_dire(void)			 
{
		u8 com_data; 
		u8 i;
		static u8 RxCounter1=0;
		static u16 RxBuffer1[10]={0};
		static u8 RxState = 0;	
		static u8 RxFlag1 = 0;

		if( USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)  	   //�����ж�
		{
				USART_ClearITPendingBit(USART1,USART_IT_RXNE);   //����жϱ�־
				com_data = USART_ReceiveData(USART1);						 //���մ���1����
			
				if(RxState==0&&com_data==0x77)  
				{
					RxState=1;
					RxBuffer1[RxCounter1++]=com_data;
				}
		
				else if(RxState==1&&com_data==0x77) 
				{
					RxState=2;
					RxBuffer1[RxCounter1++]=com_data;
				}
		
				else if(RxState==2)
				{
					RxBuffer1[RxCounter1++]=com_data;

					if(RxCounter1>=10||com_data == 0x4b)       //RxBuffer1����������ս������յ�ֹͣλ
					{
						RxState=3;
						RxFlag1=1;
						data=RxBuffer1[RxCounter1-3];		
						data2 = RxBuffer1[RxCounter1-2];	
					}
				}
		
				else if(RxState==3)		//����Ƿ��յ�������־
				{
						if(RxBuffer1[RxCounter1-1] == 0x4b)
						{
									USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);//�ر��ж�
									if(RxFlag1)
									{
										dire = data;
										flag = data2;
									}
									RxFlag1 = 0;
									RxCounter1 = 0;
									RxState = 0;
									USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  //���ж�
						}
						else   //û�յ���ֹλ�����մ���
						{
									RxState = 0;
									RxCounter1=0;
									for(i=0;i<10;i++)
									{
											RxBuffer1[i]=0x00;      //����
									}
						}
				} 
	
				else   //�쳣
				{
						RxState = 0;
						RxCounter1=0;
						for(i=0;i<10;i++)
						{
								RxBuffer1[i]=0x00;    
						}
				}

		}
		
}

void USART1_IRQHandler_count(void)			 
{
		u8 com_data; 
		u8 i;
		static u8 RxCounter1=0;
		static u16 RxBuffer1[10]={0};
		static u8 RxState = 0;	
		static u8 RxFlag1 = 0;

		if( USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)  	   //�����ж�
		{
				USART_ClearITPendingBit(USART1,USART_IT_RXNE);   //����жϱ�־
				com_data = USART_ReceiveData(USART1);						 //���մ���1����
			
				if(RxState==0&&com_data==0x55)  //0xb3֡ͷ��ʼ����
				{
					RxState=1;
					RxBuffer1[RxCounter1++]=com_data;
				}
		
				else if(RxState==1&&com_data==0x55)  //0xb3֡ͷ
				{
					RxState=2;
					RxBuffer1[RxCounter1++]=com_data;
				}
		
				else if(RxState==2)
				{
					RxBuffer1[RxCounter1++]=com_data;

					if(RxCounter1>=10||com_data == 0x2B)       //RxBuffer1����������ս������յ�ֹͣλ
					{
						RxState=3;
						RxFlag1=1;
						data=RxBuffer1[RxCounter1-2];		
					}
				}
		
				else if(RxState==3)		//����Ƿ��յ�������־
				{
						if(RxBuffer1[RxCounter1-1] == 0x2B)
						{
									USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);//�ر��ж�
									if(RxFlag1)
									{
										out = data;
									}
									RxFlag1 = 0;
									RxCounter1 = 0;
									RxState = 0;
									USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  //���ж�
						}
						else   //û�յ���ֹλ�����մ���
						{
									RxState = 0;
									RxCounter1=0;
									for(i=0;i<10;i++)
									{
											RxBuffer1[i]=0x00;      //����
									}
						}
				} 
	
				else   //�쳣
				{
						RxState = 0;
						RxCounter1=0;
						for(i=0;i<10;i++)
						{
								RxBuffer1[i]=0x00;    
						}
				}

		}
		
}

