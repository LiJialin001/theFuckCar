#include "stm32f10x.h"
#include "ooled_font.h"
#include "sys.h"


/*????*/
#define OLED_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)(x))
#define OLED_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)(x))

/*?????*/
void OLED_I2C_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C??
  * @param  ?
  * @retval ?
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

/**
  * @brief  I2C??
  * @param  ?
  * @retval ?
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C??????
  * @param  Byte ????????
  * @retval ?
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//???????,???????
	OLED_W_SCL(0);
}

/**
  * @brief  OLED???
  * @param  Command ??????
  * @retval ?
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//????
	OLED_I2C_SendByte(0x00);		//???
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/**
  * @brief  OLED???
  * @param  Data ??????
  * @retval ?
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//????
	OLED_I2C_SendByte(0x40);		//???
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED??????
  * @param  Y ???????,???????,??:0~7
  * @param  X ???????,???????,??:0~127
  * @retval ?
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//??Y??
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//??X???4?
	OLED_WriteCommand(0x00 | (X & 0x0F));			//??X???4?
}

/**
  * @brief  OLED??
  * @param  ?
  * @retval ?
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}

/**
  * @brief  OLED??????
  * @param  Line ???,??:1~4
  * @param  Column ???,??:1~16
  * @param  Char ????????,??:ASCII????
  * @retval ?
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//???????????
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//????????
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//???????????
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//????????
	}
}




/**
  * @brief  OLED?????
  * @param  Line ?????,??:1~4
  * @param  Column ?????,??:1~16
  * @param  String ???????,??:ASCII????
  * @retval ?
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED????
  * @retval ?????X?Y??
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED????(???,??)
  * @param  Line ?????,??:1~4
  * @param  Column ?????,??:1~16
  * @param  Number ??????,??:0~4294967295
  * @param  Length ????????,??:1~10
  * @retval ?
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED????(???,????)
  * @param  Line ?????,??:1~4
  * @param  Column ?????,??:1~16
  * @param  Number ??????,??:-2147483648~2147483647
  * @param  Length ????????,??:1~10
  * @retval ?
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED????(????,??)
  * @param  Line ?????,??:1~4
  * @param  Column ?????,??:1~16
  * @param  Number ??????,??:0~0xFFFFFFFF
  * @param  Length ????????,??:1~8
  * @retval ?
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED????(???,??)
  * @param  Line ?????,??:1~4
  * @param  Column ?????,??:1~16
  * @param  Number ??????,??:0~1111 1111 1111 1111
  * @param  Length ????????,??:1~16
  * @retval ?
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

/**********************************
??????
????: 
x----?
y----?
no----???????????
**********************************/
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_SetCursor(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WriteData(Hzk[2*no][t]);
				adder+=1;
     }	
		OLED_SetCursor(x+1,y);	
    for(t=0;t<16;t++)
			{	
				OLED_WriteData(Hzk[2*no+1][t]);
				adder+=1;
      }					
}

/**
  * @brief  OLED???
  * @param  ?
  * @retval ?
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//????
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//?????
	
	OLED_WriteCommand(0xAE);	//????
	
	OLED_WriteCommand(0xD5);	//?????????/?????
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//???????
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//??????
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//???????
	
	OLED_WriteCommand(0xA1);	//??????,0xA1?? 0xA0????
	
	OLED_WriteCommand(0xC8);	//??????,0xC8?? 0xC0????

	OLED_WriteCommand(0xDA);	//??COM??????
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//???????
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//???????
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//??VCOMH??????
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//????????/??

	OLED_WriteCommand(0xA6);	//????/????

	OLED_WriteCommand(0x8D);	//?????
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//????
		
	OLED_Clear();				//OLED??
}

