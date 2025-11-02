#ifndef __SPI_H
#define __SPI_H

#include "at32f403a_407.h"


void SPI1_Init(void);
void SPI1_SetSpeed(spi_mclk_freq_div_type SPI_BaudRatePrescaler);
uint8_t SPI1_ReadWriteByte(u8 data);

void SPI4_Init(void);
void SPI4_SetSpeed(spi_mclk_freq_div_type SPI_BaudRatePrescaler);
uint8_t SPI4_ReadWriteByte(u8 data);






#endif





