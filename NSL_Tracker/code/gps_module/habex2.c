#include <avr/io.h> 

// Define baud rate 
#define F_CPU 16000000UL
//#define F_CPU  7372800UL

#define USART_BAUDRATE 57600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) 


/*! \brief Initiliaze the USART0 for the communication bus 
 * This function is used to initialize the USART which a baudrate
 * that needs to be sent in as a parameter Use the baudrate settings
 * specified in the ATMEGA2560 baudrate setting.
 * \param baud The baudrate param from the ATMEGA2560 datasheet.
 */
void usart0_init(unsigned int baudrate) {
	/* Set baud rate */
	UBRR0H = (unsigned char) (baudrate>>8);
	UBRR0L = (unsigned char) baudrate;
	/* Set frame format: 8data, no parity & 1 stop bits */
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
}

/*! \brief Send a character to the USART0
 * Send a single character to the USART used for the communication bus
 * \param data The character you want to send
 */
void usart0_transmit(char data ) {
	/* Wait for empty transmit buffer */
	while (!( UCSR0A & (1<<UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

/*! \brief Send a string of characters to the USART0
 * Send a string of characters to the USART used for the communication bus
 * \param data The string of characters you wish to send
 * \param length The length of the string you wish to send
 */
unsigned char usart0_sendstring(char *data,unsigned char length) {
	int i;
	for (i=0;i<length;i++)
		usart0_transmit(*(data++));
	
	return 0;
}

/*! \brief Retrieve one character from the USART0
 * Retrieve one character from the USART. This function will wait until
 * there is a character in the USART RX buffer
 * \return The character from the RX USART buffer
 */
unsigned char usart0_receive(void ) {
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}

/*! \brief The USART0 recieve loopback
 * This function does wait for a character in the RX buffer and returns
 * it to the transmit buffer.
 * \return The character from the RX USART buffer
 */
unsigned char usart0_receive_loopback(void ) {
	uint8_t rbuff;
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	rbuff = UDR0;
	usart0_transmit(rbuff);
	return rbuff;
}

/*! \brief Retrieve one character from the USART0
 * Retrieve one character from the USART. With this function you will
 * need to poll the USART, it does NOT wait until a character is in the buffer.
 * \return The character from the RX USART buffer
 */
unsigned char poll_usart0_receive(void ) {
	/* Check if data is received */
	return ((UCSR0A & (1<<RXC0)));
}
/*! \brief Initiliaze the USART3 for the radio interface
 * This function is used to initialize the USART which a baudrate
 * that needs to be sent in as a parameter Use the baudrate settings
 * specified in the ATMEGA2560 baudrate setting.
 * \param baud The baudrate param from the ATMEGA2560 datasheet.
 */
void usart3_init(unsigned int baudrate, unsigned char stopbits) {

	/* Set baud rate */
	UBRR3H = (unsigned char) (baudrate>>8);
	UBRR3L = (unsigned char) baudrate;

	/* Set frame format: 8data, no parity */
	UCSR3C = (1<<UCSZ31) | (1<<UCSZ30) | (0<<UCSZ32);
	/* Set the number of stopbits */
	if (stopbits == 1)
		UCSR3C |= (0<<USBS3);
	else if (stopbits == 2)
		UCSR3C |= (1<<USBS3);
	
	/* Enable receiver, transmitter and RX interrupt */
	UCSR3B = (1<<RXEN3) | (1<<TXEN3) | (1<<RXCIE3);
}

/*! \brief Send a character to the USART3
 * Send a single character to the USART used for the communication bus
 * \param data The character you want to send
 */
unsigned char usart3_transmit(char  data ) {
	/* Wait for empty transmit buffer */
	while (!( UCSR3A & (1<<UDRE3)));
	/* Put data into buffer, sends the data */
	UDR0 = data;
	return 0;
}

/*! \brief Send a string of characters to the USART3
 * Send a string of characters to the USART used for the communication bus
 * \param data The string of characters you wish to send
 * \param length The length of the string you wish to send
 */
unsigned char usart3_sendstring(char *data,unsigned char length) {
	int i;
	for (i=0;i<length;i++)
		usart3_transmit(*(data++));
	
	return 0;
}

/*! \brief Retrieve one character from the USART3
 * Retrieve one character from the USART. This function will wait until
 * there is a character in the USART RX buffer
 * \return The character from the RX USART buffer
 */
unsigned char usart3_receive(void ) {
	/* Wait for data to be received */
	while (!(UCSR3A & (1<<RXC3)));
	/* Get and return received data from buffer */
	return UDR0;
}


int main(void) 
{ 
	usart0_init( BAUD_PRESCALE );

	usart3_init( BAUD_PRESCALE, 1 ) ;

	unsigned char data = 'z'; 


	for(;;) // Repeat indefinitely 
	{ 

		data = usart3_receive ();

		usart0_transmit (data );
} 
} 
