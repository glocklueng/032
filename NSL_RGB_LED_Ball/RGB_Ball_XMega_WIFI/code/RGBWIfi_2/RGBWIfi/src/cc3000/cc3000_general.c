

#include "cc3000\cc3000_general.h"
#include "cc3000\cc3000.h"
#include "cc3000\cc3000_hci.h"
#include "cc3000\cc3000_spi.h"


uint8_t cc3000_general_read_buffer_size(uint8_t *free_buffers, uint16_t *buffer_length) {

uint8_t status;
uint8_t *ptr;

	cc3000_hci_send_command(HCI_COMMAND_READ_BUFFER_SIZE, NULL, 0);
	cc3000_hci_wait_for(HCI_EVENT_READ_BUFFER_SIZE);
	
	status = wlan_rx_buffer[4];
	ptr = &wlan_rx_buffer[5];
	*free_buffers = *ptr++;
	*buffer_length = stream_to_uint16_t(ptr);
	return status;
	
}

void cc3000_general_startup(uint8_t patches_request) {
	
	__PROLOG("--- cc3000_startup\n");
	
	cc3000_smart_config_complete = 0;
	cc3000_connected = 0;
	cc3000_dhcp_complete = 0;
	
	cc3000_module_disable();
	_delay_ms(200);

	debug_str("wait IRQ line to go high\n");
	// todo: put timeout here
	while (cc3000_read_irq_pin() != 1);

	cc3000_module_enable();
	
	debug_str("Waiting for IRQ line to go low\n");
	// todo: put timeout here
	while (cc3000_read_irq_pin() != 0);
	
	cc3000_cs_enable();
	
	// Time for the magic start up sequence
	_delay_us(50);
	debug_str("Sending start command\n");
	cc3000_spi_send(SPI_OPERATION_WRITE);	//0x01
	cc3000_spi_send(0);	// length msb
	cc3000_spi_send(5);	// length lsb
	cc3000_spi_send(0);	// busy 0
	
	_delay_us(50);
	
	cc3000_spi_send(0);	// busy 1
	cc3000_spi_send(HCI_TYPE_COMMAND);
	cc3000_spi_send(HCI_COMMAND_SIMPLE_LINK_START & 0xff);
	cc3000_spi_send(HCI_COMMAND_SIMPLE_LINK_START >> 8);
	cc3000_spi_send(1);		// 1 byte payload
	cc3000_spi_send(patches_request);
	
	cc3000_cs_disable();
	
	// wait for irq
	debug_str("Waiting for IRQ line to go low (active)\n");
	// todo: add timeout here
	while (cc3000_read_irq_pin() != 0);
	debug_str("Got it\n");	
	
	cc3000_hci_receive();
	
	debug_str("Waiting for IRQ line to go high (in active)\n");
	// todo: add timeout here
	while (cc3000_read_irq_pin() != 1);
	
	// now turn interrupts on
	cc3000_interrupt_enable();

	// grab the free buffers and buffer length for future use
	
	uint8_t status = cc3000_general_read_buffer_size(&free_buffers, &buffer_length);
	
	debug_var("status=", status);
	debug_str(" Free buffers= ");
	debug_int(free_buffers);
	debug_str(" Buffer length = ");
	debug_int_hex_16bit(buffer_length);
	debug_nl();	
	debug_str("startup complete\n");
}

void cc3000_general_shutdown() {

	debug_str("\nSHUTDOWN---\n");
	cc3000_module_disable();
	_delay_ms(200);

	debug_str("gs IRQ HIGH\n");
	
//	while (cc3000_read_irq_pin() != 1);
	
	debug_str("SHUTDOWN\n");
}	
