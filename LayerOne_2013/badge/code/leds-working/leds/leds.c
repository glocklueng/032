/*
 * leds.c
 *
 * Created: 5/23/2013 8:13:35 PM
 *  Author: charliex
 */ 

#include <avr/io.h>
#include <util/delay.h>

#include "leds.h"

static int brite = 1;
static int britedir = 10;
int gMode = 0;


unsigned char mode_switch( void ) 
{
	
	return 0;
}
void fadeup(void)
{
	return;

	int quit = 0;

	while(quit == 0 ){

		quit = 1;

		for(unsigned int i = 0 ; i < (NUM_TLC5947); i++) {

			if( LEDChannels[ i ] < 4095 ) {
				LEDChannels[ i ]*=2;
				quit = 0;
			}
		}

		WriteLEDArray(NUM_TLC5947);
	}
}

void fade(void)
{
	int quit = 0;
	while(quit == 0 ){

		quit = 1;

		for(unsigned int i = 0 ; i < (NUM_TLC5947); i++) {
			if( LEDChannels[ i ] > 2 ) {
				LEDChannels[i]/=2;
				quit = 0;
			}
		}

		WriteLEDArray(NUM_TLC5947);
	}
}

ISR(TCC0_OVF_vect)
{
	static unsigned int counter = 0;
	
	counter ++ ;
	
	if( counter == 1000 )	{
		counter = 0;
	
		gMode++;
	}
 
 }

int main(void)
{
	unsigned short l = 40;
	unsigned short ld = 1;

	// Power reduction: Stop unused peripherals
	PR.PRGEN = 0x18;        // Stop: AES, EBI
	PR.PRPC  = 0x74;        // Stop: TWI, USART0, USART1, HIRES
	PR.PRPD  = 0x7C;        // Stop: TWI, USART0, USART1, SPI, HIRES
	PR.PRPE  = 0x60;        // Stop: TWI, USART1, (XMEGA bug - HIRES cannot be turned off)
	PR.PRPF  = 0x78;        // Stop: TWI, USART0, USART1, SPI, (XMEGA bug - HIRES cannot be turned off)

	PORTD.DIR      = 0xFF;  // Leds
	PORTCFG.MPCMASK = 0xFF; // Configure all pins on PORTD the same way
	PORTD.PIN0CTRL = 0x00;  // Pull up on pin PortD (Keypad and K2-K5 buttons)
	PORTD.INTCTRL  = 0x00;  // PORTD will generate no low level interrupts
	PORTD.INT0MASK = 0x00;  
	PORTD.OUT      = 0x00;
	
	/* Enable HIGH level interrupts in the PMIC. */
	PMIC.CTRL = PMIC_HILVLEN_bm;

	TCC0.CTRLA = TC_CLKSEL_DIV8_gc;
	TCC0.PER = 185;
	TCC0.CNT = 0;
	TCC0.CTRLFSET = 0x0; /* count upwards, but don't start to interrupt yet */
	TCC0.INTCTRLA = TC_OVFINTLVL_HI_gc; /* enable interrupts */
	  
	LED_Init();
  
 	WriteLEDArray(NUM_TLC5947);
	
	l = 0;
	ld = 40;

	gMode = 0;

	/* enable global interrupts */
	sei();
 
	while( 1 )					// Loop Here
	{
		int d;

		if( mode_switch( ) ){
			brightnessShift++;
			brightnessShift%=6;
			_delay_ms( 1000 );
		}


		// mode
		// 2 = all fade on off
		// 3 = none
		// 4 = not working
		// 5 = smooth around scroll
		// 6 = err
		// 7 = nothing
		// 8 = nothing
		// 9 = ok
		// 10 = none
		// 11 = flash pulse
		// 12 = fade pulse
		// 13 = not much
		// 14 =

		switch( gMode) {

			case 0:
				{
					unsigned int offset ;
					offset = 0;
					float mil;

					{
						for(float i =1 ; i < 30 ;i+=.08){

							offset = (int)( (1.0f+sinf(i)) *60.0f);

							LEDscan2(4096,(offset*2),1);
							WriteLEDArray(1);
							_delay_ms(10);

						}
					}
				}

				l = 0;
				ld = 40;

				break;
		
			case 1:
			{
	
				Clear( l );

				l += ld;

				if( l == 4000 )
				ld = -40;

				if( l == 40 )
				ld = 40;

				WriteLEDArray(NUM_TLC5947);
				_delay_ms(10);

			}
			break;

			case 2:

				fadeup();

				for(int i =0 ; i < 2 ;i++)
				for (int offset = 240; offset < 300; offset += 1) {

					LEDscan(4096, offset,1);
				}



				break;		

			case 3:

				gMode++;
			break;

			case 4:
				gMode++;
				break;

			case 5:

				fadeup();

				for(int i =0 ; i < 5 ;i++) {

					for (int offset = 240; offset < 300; offset += 1) {

						LEDscan2(4096, offset,1);
						WriteLEDArray(1);
						_delay_ms( 20 );
					}
				}

				break;

			case 6:
				
				for(int i =0 ; i < 4 ;i++)
				for (int offset = 240; offset < 300; offset += 1) {

					LEDscan2(4096, offset,i/2);

					WriteLEDArray(i/2);
					_delay_ms( 5 );
				}
			

				break;
			case 7:
			
				{

					for(int i =1 ; i < 6 ;i++)
					for (int offset = 240; offset < 300; offset += 2) {

						//LEDscan2(4096, offset,1);
						LEDscan2(4096/i,360-offset,1);
						WriteLEDArray(1);

					}
				}
				break;
				
			case 8:
		
				{

					fade();


					for(unsigned int i =1 ; i < 6 ;i++){

						for (unsigned int offset = 240; offset < 300; offset += 16) {

							LEDscan2(4096/i,offset,1);
							for(int k =1 ; k <= 16 ;k++)
							WriteLEDArray(1);

						}
					}
				}
				break;

			case 9:
			{
				unsigned int offset ;
				offset = 0;
				float mil;

				mil = 50+(rand()%70);

				{

					for(float i =1 ; i < 10 ;i+=.1){

						offset = (int)( (1.0f+sinf(i)) *mil);

						LEDscan2(4096,(offset*2),1);
						WriteLEDArray(1);
						
						for(int id=0;id<6;id++)
							_delay_ms(6);

					}
				}

			}
			break;

			case 10:
			{

				fade();

				for(unsigned int j =0 ; j < 50 ;j++) {
					for(unsigned int i =0 ; i < NUM_TLC5947 ;i++){
						LEDChannels[i]=rand()%2047;
					}


					WriteLEDArray(NUM_TLC5947);
				}

			}
			
			brite =1;
			britedir=10;
			
			break;


			case 11:
			for(unsigned int i=0;i<80;i+=4) {
				//Clear(i);
				brite+=britedir;
				if (brite>6050) britedir=-30;
				if (brite<40) britedir=30;

				LEDscan2(brite,i,1);
				LEDscan2Add(brite,i-80,1);
				for(int t = 0 ; t < NUM_TLC5947 ; t++ )
				WriteLEDArray(1);
				_delay_ms(20);
			}

			break;

			case 12:
			{

				for(unsigned int i =1 ; i < 6 ;i++){
					LEDscan(0,0,1);
					for (unsigned int offset = 0; offset < 360; offset += 16) {

						LEDscan2Add(4096/i,offset,1);
						for(int k =1 ; k <= 16 ;k++) {
							WriteLEDArray(1);
						}
						_delay_ms(10);
					}
				}
			}
			break;

			case 13:
			for (int r=0;r<NUM_TLC5947;r+=16){
				for (int x=r;x<r+15;x+=1) {
					LEDChannels[x]=rand()/1024;
					if (LEDChannels[x]>30) LEDChannels[x]=4095;
					else if (LEDChannels[x]<10) LEDChannels[x]=0;
				}
				for(int i=0;i<NUM_TLC5947;i++){
					WriteArrayOffset(NUM_TLC5947,i);
					
				}
			}


			gMode = 0;

			break;

			case 14:

			/// back to first mode
			gMode = 0;

			break;
		}
	}

}