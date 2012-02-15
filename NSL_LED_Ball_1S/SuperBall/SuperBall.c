// todo:
// convert the spi to software bitbang.

#include "SuperBall.h"

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

int gCount = 0;
int gMode = 0;

void setup_scroll(void);
void loop_scroll(void);

#define BAILOUT 			( 16 )
#define MAX_ITERATIONS		( 16 ) 

#define BAUDRATE (57600)
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)


static char String[]="P";


void USART_init(void)
{
 UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
 UBRR0L = (uint8_t)(BAUD_PRESCALLER);
 UCSR0B = (1<<RXEN0)|(1<<TXEN0);
 UCSR0C = (3<<UCSZ00);

 UCSR0B = _BV(RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
}

unsigned char USART_receive(void){

 while(!(UCSR0A & (1<<RXC0)));
 return UDR0;

}

void USART_send( unsigned char data){

 while(!(UCSR0A & (1<<UDRE0)));
 UDR0 = data;

}

void USART_putstring(char* StringPtr){

while(*StringPtr != 0x00){
 USART_send(*StringPtr);
 StringPtr++;}

}


static unsigned char pushLeds = 0;
static unsigned short indexLed = 0;

SIGNAL(USART_RX_vect )
/*************************************************************************
Function: UART Receive Complete interrupt
Purpose:  called when the UART has received a character
**************************************************************************/
{
   unsigned short data ;
    unsigned char usr;

 
    /* read UART status register and UART data register */ 
    usr  = UCSR0A;
    data = UDR0;

	LEDChannels[indexLed++] = data *16;


	// fill buffer


	if( indexLed == (NUM_TLC5947*24) ) {
		indexLed = 0;
		pushLeds = 1;

	}
}

void fadeup(void)
{
return;

	int quit = 0;
	while(quit == 0 ){
		
		quit = 1;

		for(unsigned int i = 0 ; i < (NUM_TLC5947*24); i++) {
			if( LEDChannels[ i ] < 4095 ) { 
				LEDChannels[i]*=2;
				quit = 0;
			}
			if(LEDChannels[ i ] >4095) 
				LEDChannels[ i ] = 4095 ;
		}
		
		WriteLEDArray(NUM_TLC5947);
	}
}

void fade(void)
{
	int quit = 0;
	while(quit == 0 ){
		
		quit = 1;

		for(unsigned int i = 0 ; i < (NUM_TLC5947*24); i++) {
			if( LEDChannels[ i ] > 2 ) { 
				LEDChannels[i]/=2;
				quit = 0;
			}
		}
		
		WriteLEDArray(NUM_TLC5947);
	}
}


int main(void)
{
	// Setup chip
	unsigned short r,g,b;
	unsigned short tr,tg,tb;

	unsigned short l = 40;
	unsigned short ld = 1;

	USART_init();

	LED_Init();						// Sequence/Flash LEDs on startup
	_delay_ms( 250 );
	WriteLEDArray(NUM_TLC5947);

	r = 0;
	g = 0;
	b = 0;
	
	tr = 4096;
	tg = 0;
	tb = 0;

	setup_scroll();

	l = 0;
	ld = 40;
	gMode = 0;

#define SUB	( 2 )
	sei();


	while( 0 ) {
    	USART_send('X');_delay_ms(250);
	}
	while(0){
		if( pushLeds == 1 ) {

			// notifys
			USART_putstring(String);

			//clamp
			for(unsigned int i = 0 ; i < (NUM_TLC5947*24); i++) {
				if(LEDChannels[i]>4095) 
					LEDChannels[i]=4095;				
			}

			WriteLEDArray(NUM_TLC5947);
			pushLeds =0;
		} else {
			_delay_us(10);
		}
	}

	while( 1 )					// Loop Here
	{
		int d;

		USART_send( gMode+'a');
		USART_send( 13);
		USART_send( 10);
			

		gMode = 1;

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
			 gMode=1;
			 l = 0;
			 ld = 40;
			 //fadeup();
			break;

			case 1:

				Clear( l );

				l+=ld;

				if( l == 4000 ) 
					ld = -40;
				if( l == 40 ) 
					ld = 40;

				WriteLEDArray(NUM_TLC5947);
				_delay_ms(10);
				
				if((rand() % 150 ) == 5 )  gMode++;

				break;
			
			
			case 2:
			fadeup();
				for(int i =0 ; i < 2 ;i++)
				for (int offset = 240; offset < 310; offset += 1) {

					LEDscan(4096, offset,1);
 				}
		
				gMode++;
	
			break;

			case 3:

				Clear(0);
				loop_scroll();

				WriteLEDArray(NUM_TLC5947);
				_delay_ms(40);
				if( (rand() % 150 ) == 5 )  {
					fade();
					gMode++;
				}
				break;

			case 4:
				//Clear(0);
				LEDChannels[l++]=ld;

				if( l > ( NUM_TLC5947  * 24 ) ) {
					l = 0;
					if( ld == 4095 ) 
						ld = 0 ; 
					else 
						ld = 4095;	
				}

				WriteLEDArray(NUM_TLC5947);

				if( (rand() % 100 ) == 5 )  {
					gMode=5;
					fadeup();
				}
				break;

			case 5:
				for(int i =0 ; i < 5 ;i++)
				for (int offset = 0; offset < 360; offset += 1) {

					LEDscan2(4096, offset,1);    WriteLEDArray(1);  
 				}
			 gMode=6;
	
			break;

			case 6:
				for(int i =0 ; i < 5 ;i++)
				for (int offset = 0; offset < 360; offset += 1) {

					LEDscan2(4096, offset,i/2);

				    WriteLEDArray(i/2);  
 				}
			 gMode=7;
	
			break;
			case 7:
			{

				for(int i =1 ; i < 6 ;i++)
				for (int offset = 0; offset < 360; offset += 2) {

					//LEDscan2(4096, offset,1);
					LEDscan2(4096/i,360-offset,1);
				    WriteLEDArray(1);  

 				}
			 gMode=8;
			 fade();
			}
			break;
			case 8:
			{

				for(unsigned int i =1 ; i < 6 ;i++){
					for (unsigned int offset = 0; offset < 360; offset += 16) {

						LEDscan2(4096/i,offset,1);
						for(int k =1 ; k < 16 ;k++)
					  	  WriteLEDArray(1);  

 					}
				}
			 gMode=9;
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
						_delay_ms(i*6);
 					
				}
				}

			}
			gMode=10;
			fade();
			break;

			case 10:
			{
	
				for(unsigned int j =0 ; j < 50 ;j++) {
					for(unsigned int i =0 ; i < NUM_TLC5947*24 ;i++){
						LEDChannels[i]=4095;//rand()%2047;
					}
				
				
			  	WriteLEDArray(NUM_TLC5947);  
				}

			 gMode=0;
		//	fade();
			 }
			break;
		}
	}


	return 0;
}

