// todo:
// convert the spi to software bitbang.

#include "RF_Blinky.h"

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

//------------------------------------------------------------------------------------------
// entry point
//------------------------------------------------------------------------------------------
int main(void)
{
	// Setup chip
	unsigned short r,g,b;
	unsigned short tr,tg,tb;

	//outputs
	DDRC &= ~(_BV( PC2 ) | _BV( PC5 ) | _BV( PC4 ) | _BV( PC3 ) );
	PORTC |= (_BV( PC2 ) | _BV( PC5 ) | _BV( PC4 )  | _BV( PC3 ) );

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

	Range_Config();

	while( 0 ) {

	MODE(0);
	MODE(1);
	_delay_us(10);
	SDAT(0);
	SDAT(1);
	_delay_us(10);
	SCLK(0);
	SCLK(1);
	_delay_us(10);

	}

	M_ACTIVE();


	r = 0;
	g = 0;
	b = 0;
	
	tr = 4096;
	tg = 4096;
	tb = 4096;
	
#define SUB	( 0 )

	while( 0 )					// Loop Here
	{
	int d;
		sei();					// Enable all interrupts

		

		for (float offset = 0; offset < 180; offset += 1 ) {


			Range_TX();

			LEDscan(r,g,b, offset);
                     
					 
			if (tr != r ) {d = ( r-tr)/(MAX(abs(tr-r),16)) ; r+=d;}
            if (tg != g ) {d = ( g-tg)/(MAX(abs(tg-g),16)) ; g+=d;}
            if (tb != b ) {d = ( b-tb)/(MAX(abs(tb-b),16)) ; b+=d;}
 

	}		// new colour target
		if( abs(r - tr) < 8 ) {tr = rand()%4096 ; }
		if( abs(g - tg) < 8 ) {tg = rand()%4096 ; }
		if( abs(b - tb) < 8 ) {tb = rand()%4096 ; }

		for (float offset =180; offset < 360; offset += 1 ) {


			Range_TX();

			LEDscan(r,g,b, offset);
                     
					 
			if (tr != r ) {d = ( r-tr)/(MAX(abs(tr-r),16)) ; r+=d;}
            if (tg != g ) {d = ( g-tg)/(MAX(abs(tg-g),16)) ; g+=d;}
            if (tb != b ) {d = ( b-tb)/(MAX(abs(tb-b),16)) ; b+=d;}
 

	}


		// new colour target
		if( abs(r - tr) < 8 ) {tr = rand()%4096 ; }
		if( abs(g - tg) < 8 ) {tg = rand()%4096 ; }
		if( abs(b - tb) < 8 ) {tb = rand()%4096 ; }


	}

	return 0;
}

