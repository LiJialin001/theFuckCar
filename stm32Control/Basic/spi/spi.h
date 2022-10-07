#ifndef _SPI_H
#define _SPI_H

#include "sys.h"


// spi 时钟引脚PB13
// 读写模式引脚PB14
// 数据引脚PB15

 
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte);
void SPI2_Init(void);
void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet);

#endif
