#include "receive_port.h"

void uart1_Init(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART1,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入
  GPIO_InitStructure.GPIO_Pin = Data;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DataGPIO,&GPIO_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;                  //设置波特率115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //字长8位数据模式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;        //一位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;           //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                  //收发模式
  USART_Init(USART1, &USART_InitStructure);   
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //使能中断
  USART_Cmd(USART1, ENABLE);                      //使能串口1
	USART_ClearFlag(USART1, USART_FLAG_TC);        // 清串口1
}


void USART1_IRQHandler(void)			 
{
		u8 com_data; 
		u8 i;
		static u8 RxCounter1=0;
		static u16 RxBuffer1[10]={0};
		static u8 RxState = 0;	
		static u8 RxFlag1 = 0;

		if( USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)  	   //接收中断
		{
				USART_ClearITPendingBit(USART1,USART_IT_RXNE);   //清除中断标志
				com_data = USART_ReceiveData(USART1);						 //接收串口1数据
			
				if(RxState==0&&com_data==0xb3)  //0xb3帧头开始接收
				{
					RxState=1;
					RxBuffer1[RxCounter1++]=com_data;
				}
		
				else if(RxState==1&&com_data==0xb3)  //0xb3帧头
				{
					RxState=2;
					RxBuffer1[RxCounter1++]=com_data;
				}
		
				else if(RxState==2)
				{
					RxBuffer1[RxCounter1++]=com_data;

					if(RxCounter1>=10||com_data == 0x5B)       //RxBuffer1存满，或接收结束即收到停止位
					{
						RxState=3;
						RxFlag1=1;
						data=RxBuffer1[RxCounter1-2];		
					}
				}
		
				else if(RxState==3)		//检测是否收到结束标志
				{
						if(RxBuffer1[RxCounter1-1] == 0x5B)
						{
									USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);//关闭中断
									if(RxFlag1)
									{
										out = data;
									}
									RxFlag1 = 0;
									RxCounter1 = 0;
									RxState = 0;
									USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  //开中断
						}
						else   //没收到截止位即接收错误
						{
									RxState = 0;
									RxCounter1=0;
									for(i=0;i<10;i++)
									{
											RxBuffer1[i]=0x00;      //清零
									}
						}
				} 
	
				else   //异常
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

		if( USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)  	   //接收中断
		{
				USART_ClearITPendingBit(USART1,USART_IT_RXNE);   //清除中断标志
				com_data = USART_ReceiveData(USART1);						 //接收串口1数据
			
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

					if(RxCounter1>=10||com_data == 0x4b)       //RxBuffer1存满，或接收结束即收到停止位
					{
						RxState=3;
						RxFlag1=1;
						data=RxBuffer1[RxCounter1-3];		
						data2 = RxBuffer1[RxCounter1-2];	
					}
				}
		
				else if(RxState==3)		//检测是否收到结束标志
				{
						if(RxBuffer1[RxCounter1-1] == 0x4b)
						{
									USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);//关闭中断
									if(RxFlag1)
									{
										dire = data;
										flag = data2;
									}
									RxFlag1 = 0;
									RxCounter1 = 0;
									RxState = 0;
									USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  //开中断
						}
						else   //没收到截止位即接收错误
						{
									RxState = 0;
									RxCounter1=0;
									for(i=0;i<10;i++)
									{
											RxBuffer1[i]=0x00;      //清零
									}
						}
				} 
	
				else   //异常
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
