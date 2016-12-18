/*===================================================================*/
/*                                                                   */
/*  spidma.h : SPI DMA function header                               */
/*                                                                   */
/*  2016/1/20  Racoon                                                */
/*                                                                   */
/*===================================================================*/

#ifndef SPIDMA_H_INCLUDED
#define SPIDMA_H_INCLUDED

void spi_init();
void spi_write(uint8_t data);
void spi_writew(uint16_t data);
uint8_t spi_read(void);

extern SPI_HandleTypeDef SpiHandle;

#endif

