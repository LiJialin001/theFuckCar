#ifndef _SPI_H
#define _SPI_H

#include "sys.h"


// spi ʱ������PB13
// ��дģʽ����PB14
// ��������PB15

 
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte);
void SPI2_Init(void);
void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet);

#endif
