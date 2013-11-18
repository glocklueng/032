/*
 * spi_hw.h
 *
 * Created: 11/15/2013 11:14:24 PM
 *  Author: charlie
 */ 


#ifndef SPI_HW_H_
#define SPI_HW_H_

void spi_hw_setup_io(void);
void spi_hw_init(void);
void spi_hw_master_transmit(uint8_t data);
uint8_t spi_hw_master_receive(void);

#endif /* SPI_HW_H_ */