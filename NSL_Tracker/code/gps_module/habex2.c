// need this for delay
#define F_CPU ( 16000000UL )

// avr specific
#include <avr/io.h> 
#include <avr/interrupt.h>

// utilities
#include <util/delay.h>

// handy macros
#define SET_BIT(p,m) ((p) |= (m))
#define CLEAR_BIT(p,m) ((p) &= ~(m))

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//////////////////////////////////////////////////////////////////
//
// USART code
//
//////////////////////////////////////////////////////////////////

// Define baud rate 
#define USART_BAUDRATE	( 57600UL )
#define BAUD_PRESCALE	((( F_CPU / ( USART_BAUDRATE * 16UL ))) - 1 ) 


/*! \brief Initialize the USART0 for the communication bus 
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
unsigned char usart0_sendstring(const char *data) {
	
	// send ASCIZ string	
	while(*data != 0 ) {

		usart0_transmit(*(data++));
	
		// delay in transmit
		_delay_ms( 1 );
	}	

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

/*! \brief The USART0 receive loop back
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

/*! \brief Initialize the USART3 for the radio interface
 * This function is used to initialize the USART which a baudrate
 * that needs to be sent in as a parameter Use the baudrate settings
 * specified in the ATMEGA2560 baudrate setting.
 * \param baud The baudrate param from the ATMEGA2560 data sheet.
 */
void usart3_init(unsigned int baudrate) {

	/* Set baud rate */
	UBRR3H = (unsigned char) (baudrate>>8);
	UBRR3L = (unsigned char) baudrate;

	/* Set frame format: 8data, no parity */
	UCSR3C = (1<<UCSZ31) | (1<<UCSZ30) | (0<<UCSZ32);
	
	UCSR3C |= (0<<USBS3);
	
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
	UDR3 = data;

	return 0;
}

/*! \brief Send a string of characters to the USART3
 * Send a string of characters to the USART used for the communication bus
 * \param data The string of characters you wish to send
 * \param length The length of the string you wish to send
 */
unsigned char usart3_sendstring(const char *data) {

	// send ASCIZ string	
	while(*data != 0 ) {

		usart3_transmit(*(data++));
	
		// delay in transmit
		_delay_ms( 1 );
	}

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

/*! \brief Retrieve one character from the USART3
 * Retrieve one character from the USART. With this function you will
 * need to poll the USART, it does NOT wait until a character is in the buffer.
 * \return The character from the RX USART buffer
 */
unsigned char poll_usart3_receive(void ) 
{
	/* Check if data is received */
	return ((UCSR3A & (1<<RXC3)));
}


/*! \brief UART3 Receive Complete interrupt
 * Purpose:  called when the UART3 has received a character
 * echos to UART0
 * \return none
 */
ISR(USART3_RX_vect)
{
	unsigned char data;

	/* read UART data register */
	data = UDR3;
	
	/* Wait for empty transmit buffer */
	while (!( UCSR0A & (1<<UDRE0)));
	
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

/*! \brief UART0 Receive Complete interrupt
 * Purpose:  called when the UART0 has received a character
 * echos to UART3
 * \return none
 */
ISR(USART0_RX_vect)
{
	unsigned char data;

	/* read UART data register */
	data = UDR0;
	
	/* Wait for empty transmit buffer */
	while (!( UCSR3A & (1<<UDRE3)));
	
	/* Put data into buffer, sends the data */
	UDR3 = data;
	
	
	// NO ECHO
	/* Wait for empty transmit buffer */
	//while (!( UCSR0A & (1<<UDRE0)));
	//UDR0 = data;

}

/////////////////////////////////////////////////////
//
// Main code
//
/////////////////////////////////////////////////////

// status of PC3 line
static unsigned char pc3on = 0;

// AT CMD's
static const char string_at_gps_inf[]	= "AT+CGPSINF=0\r\n";
static const char gsm_init_string[]		= "AT+CNUM\r\nATE1\r\n";

// Boot string
static const char string_hi[] = "\r\n\r\n[NULL SPACE LABS] 032.la\r\n\r\nSYSTEM BOOTING\r\n\r\n";

int main(void) 
{ 
	static unsigned int counter = 0;
	
	// initialise the two UARTS used
	usart0_init( BAUD_PRESCALE );

	usart3_init( BAUD_PRESCALE ) ;

	// say hi
	usart0_sendstring( string_hi );

	// setup outputs

	// power on
	SET_BIT( DDRD,  _BV(PD7) );
	SET_BIT( PORTD, _BV(PD7) );
	
	// setup inputs

	// first switch
	CLEAR_BIT( DDRC,  _BV(PC3) );
	SET_BIT  ( PORTC, _BV(PC3) );

	// state of PC3
	pc3on = 1;
	
	// second switch
	CLEAR_BIT( DDRC,  _BV(PC0) );
  	SET_BIT  ( PORTC, _BV(PC0) );

	// give it time to warmup
	_delay_ms( 1500 );

	// init GPS etc
	usart3_sendstring( gsm_init_string );

	//Start interrupts
	sei();
	
	// main loop
	for(;;) { 

		// switch pressed ?
		if( bit_is_clear( PINC, PC3 ) ) {
			
			if(  pc3on == 0 ) {
			
				SET_BIT(PORTD, PD7);
				pc3on = 1;
				
			} else {
				
				CLEAR_BIT(PORTD, PD7);
				pc3on = 0;
			}

			// delay, simple debouncer
			_delay_ms( 500 );	
	
		}
		
		// if button is pressed, send this gps string
		if( bit_is_clear( PINC, PC0 ) ) {
			usart3_sendstring( string_at_gps_inf );
		}
						
		counter++ ;
	}
}			
