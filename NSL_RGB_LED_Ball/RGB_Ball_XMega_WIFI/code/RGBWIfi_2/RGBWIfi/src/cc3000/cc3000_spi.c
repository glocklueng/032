
#include "cc3000\cc3000_spi.h"
#include "cc3000\cc3000.h"

uint8_t interrupts_enabled = 0;

uint8_t cc3000_spi_start_send(uint16_t hci_length) {

uint8_t spi_header	[5];
	uint16_t spi_length = 5 + hci_length;

	uint8_t padding_byte_required = spi_length & 0x01;
	
	debug_var("spi_start\nhci_length=", hci_length);
	
	if (padding_byte_required) {
		hci_length++;
		debug_str("+1");
	}

	if (cc3000_interrupts_enabled) {
		interrupts_enabled = 1;
		cc3000_interrupt_disable();	
	}

	debug_str("cc3000_spi_start_send:IRQ low?\n");

	cc3000_cs_enable();
	
	
	while (cc3000_read_irq_pin() == CC3000_IRQ_IDLE) asm("nop");
	
	debug_str("Got it\n");
	
	cc3000_spi_send(SPI_OPERATION_WRITE);	//0x01
	cc3000_spi_send(hci_length >> 8);	// length msb
	cc3000_spi_send(hci_length & 0xff);	// length lsb
	cc3000_spi_send(0);	// busy 0
	cc3000_spi_send(0);	// busy 1
	return (padding_byte_required);
	// interrupts still off at this point
}

void cc3000_spi_finish_send(uint8_t padding_byte_required) {
	
	if (padding_byte_required) {
		cc3000_spi_send(0);
	}
	
	cc3000_cs_disable();
	if (interrupts_enabled) {
		interrupts_enabled = 0;
		cc3000_interrupt_enable();
	}	
}

uint16_t cc3000_spi_receive() {

	cc3000_cs_enable();

	cc3000_spi_send(SPI_OPERATION_READ);
	cc3000_spi_send(0);	// busy 0
	cc3000_spi_send(0);	// busy 1
	
	spi_length = cc3000_spi_recv();
	spi_length = spi_length << 8;
	spi_length += cc3000_spi_recv();
	
	debug_str("\nSPI_READ: ");
	debug_int(spi_length);
	debug_str(" bytes\n");
	
	if ((spi_length > 255) || (spi_length == 0)) {
		return(0);
	}	

	for (uint16_t count = 0; count < spi_length; count++) {
		wlan_rx_buffer[count] = cc3000_spi_recv();
		debug_int_hex(wlan_rx_buffer[count]);
		debug_putc(' ');
	}
	// CS left disabled

}

