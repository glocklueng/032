#ifndef __cc3000_spi_h
#define __cc3000_spi_h

#include "user_board.h"

#if cc3000_spi_port == 1
	#include "spi_hw.h"
		#define cc3000_spi_setup_io()	spi_hw_setup_io()
		#define cc3000_spi_hw_init()    spi_hw_init()
		#define cc3000_spi_send(x) spi_hw_master_transmit(x)
		#define cc3000_spi_recv()	spi_hw_master_receive()
#else
	#if cc3000_spi_port == 2
		#include "spi2_hw.h"
			#define cc3000_spi_hw_init()    spi2_hw_init()
			#define cc3000_spi_setup_io()	spi2_hw_setup_io()
			#define cc3000_spi_send(x) spi2_hw_master_transmit(x)
			#define cc3000_spi_recv()	spi2_hw_master_receive()
	#else
		#error cc3000_spi_port not defined
	#endif
#endif		



#define SPI_OPERATION_WRITE 0x01
#define SPI_OPERATION_READ  0x03


uint8_t cc3000_spi_start_send(uint16_t hci_length);
void cc3000_spi_finish_send(uint8_t padding_byte_required);
uint16_t cc3000_spi_receive(void);


#endif