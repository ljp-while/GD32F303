#ifndef  __SPI_H
#define  __SPI_H

#include "main.h"


#define CS_HIGH {GPIO_BOP(GPIOB) = GPIO_PIN_12;}
#define CS_LOW  {GPIO_BC(GPIOB) = GPIO_PIN_12;}


void spi_init_config(void);
uint8_t spi1_write_read_byte(uint8_t ndata,uint32_t TimeOut);

#endif /*__SPI_H*/

