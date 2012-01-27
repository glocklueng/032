// todo:
// convert the spi to software bitbang.

#include "RF_Blinky.h"



//------------------------------------------------------------------------------------------
// entry point
//------------------------------------------------------------------------------------------
int main(void)
{
	// Setup chip
	unsigned short r,g,b;
	unsigned short tr,tg,tb;

	//outputs
	DATDDR &= ~(_BV( PC2 ) | _BV( PC5 ) | _BV( PC4 ) );
	DATPORT |= (_BV( PC2 ) | _BV( PC5 ) | _BV( PC4 ) );

	DDRD &= ~(_BV( PD0 ) | _BV( PD2 ) );
	PORTD |= (_BV( PD0 ) | _BV( PD2 ));
	
	// inputs
	// SPI interrupt
	DDRC	|= ( _BV( PC1 ) | SIN_PIN |SCLK_PIN|BLANK_PIN | XLAT_PIN );
	PORTC	|= ( _BV( PC1 ) | SIN_PIN |SCLK_PIN|BLANK_PIN | XLAT_PIN );


	// end chip setup



	chan_num = 80;					// Was 100
	addr_len = 3;  					// ADDRESS LENGTH (# bytes)

	SPI_CFG(0x08,0x00 + addr_len);	// ADDRESS LENGTH
//	SPI_CFG(0x09,0xCA);				// DEST ADDR 0
//	SPI_CFG(0x0E,0xCA);				// SRC ADDR 0
	SPI_CFG(0x02,0x20);				// CONT MODE,1Mbps,no CRC EN,

	LED_Init();						// Sequence/Flash LEDs on startup
	_delay_ms( 250 );
	
	Config_TX();					// Configure device for RX mode on startup
	
	M_ACTIVE();						// Enable Active Mode


	while( 0 ) {

		PMODE( 1 );
		_delay_ms( 1 ) ;

		PMODE( 0 );
		_delay_ms( 1 ) ;
	}

	r = 4096;
	g = 4096;
	b = 4096;
	
	tr = 4096;
	tg = 0;
	tb = 0;
	
#define SUB	( 2 )

	while(1)					// Loop Here
	{
		sei();					// Enable all interrupts

		if(fRTESTx)				// Test if Range test ON?
		{
			Range_Config();		// Do Configuration once

		}



		for (float offset = 0; offset < 360; offset += 1 ) {

		Range_TX();

			LEDscan(r,g,b, offset);


			if (tr != r ) {if( r < tr ) r+=(abs(tr-r)/8); else r-=(abs(tr-r)/8);}
			if (tg != g ) {if( g < tg ) g+=(abs(tg-g)/8); else g-=(abs(tg-g)/8);}
			if (tb != b ) {if( b < tb ) b+=(abs(tb-b)/8); else b-=(abs(tb-b)/8);}
	
		}

	
		// new colour target
		if((rand() % 10) > 5  )
		{
		 tr = rand()%4096;
		 tg = rand()%4096;
		 tb = rand()%4096;
		}



	}

	return 0;
}

