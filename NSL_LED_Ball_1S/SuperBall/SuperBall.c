// todo:
// convert the spi to software bitbang.
// write comments, tidy up code :)
// fuse 0xFF 0xD9 0xCE

#include "SuperBall.h"

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

int gCount = 0;
int gMode = 0;

void setup_scroll(void);
void loop_scroll(void);

#define BAILOUT 			( 16 )
#define MAX_ITERATIONS		( 16 ) 

#define BAUDRATE (250000)//(76800)
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

static int brite = 1;
static int britedir = 10;
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

		for(unsigned int i = 0 ; i < (NUM_TLC5947*24); i++) {
			if( LEDChannels[ i ] > 2 ) { 
				LEDChannels[i]/=2;
				quit = 0;
			}
		}
		
		WriteLEDArray(NUM_TLC5947);
	}
}

unsigned char mode_switch( void )
{
	return !( PINC & _BV(PC5) );
}

void WriteArrayOffset(unsigned int count, unsigned int offset) {
  //LATPORT |= (1 << BLANKPIN);
  LATPORT &= ~(1 << LATPIN);
  _delay_us(30);
  unsigned short tempOne = 0;
  unsigned int 	 tempOffset = 0;

  for (unsigned int i = 0; i < (count  * 24); i++) {
	if (i+offset>=count*24) tempOffset = offset-(count*24);
		else tempOffset = offset;
	tempOne = LEDChannels[i+tempOffset];

    for (int j = 0; j < 12; j++) {
      if ((tempOne >> (11 - j)) & 1) {
        DATPORT |= (1 << DATPIN);
      } 
      else {
        DATPORT &= ~(1 << DATPIN);
      }

      CLKPORT |= (1 << CLKPIN);
      CLKPORT &= ~(1 << CLKPIN); 

    } 
  }

	LATPORT |= (1 << LATPIN);
	_delay_us(30);
  	//LATPORT &= ~(1 << BLANKPIN);
}


unsigned char chargeMode = 0;

//Overflow ISR
ISR(TIMER1_OVF_vect)
{
	// read chargeMode
	chargeMode  =  !(PINC  & _BV(PC4));

	if( chargeMode ) {
		USART_send( '1' );
	} else {
		USART_send( '0' );
	}

}

void init_timer(void)
{

	TIMSK1	=	0x01; 	// enabled global and timer overflow interrupt;
	TCCR1A	=	0x00; 	// normal operation page 148 (mode0);
	TCNT1	=	0x0000; // 16bit counter register
	TCCR1B	=	0x04; 	// start timer/ set clock
}

void start_timer(void)
{
	//Start timer without prescaller
	TCCR1B	|=	(1<<CS10);
	
	//Enable global interrutps
	sei();
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

	init_timer();

#define SUB	( 2 )
	
	start_timer();

	while( 0 ) {
    	USART_send('X');_delay_ms(50);
	}

	while(0){

		Clear( 4095/16 );

		WriteLEDArray(NUM_TLC5947);
	}


	while(0){

		if( PINC  & _BV(PC4) ) {

			Clear( r+=10);

			if( r>4095) 
				r=0;
		}  else {
			r = 0;
			Clear( 0);
		}

		WriteLEDArray(NUM_TLC5947);
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
			_delay_us(1);
		}
	}

	while( 1 )					// Loop Here
	{
		int d;

		//USART_send( gMode+'a');
		//USART_send( 13);
		//USART_send( 10);
		if( mode_switch( ) ){ 
			brightnessShift++;
			brightnessShift%=6;
			_delay_ms( 1000 );
		}

		//gMode = 14;

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
			if( chargeMode == 0  )
				 gMode = 1;
			else 
				gMode = 2;

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
				if( (rand() % 150) == 5 )  {
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
						for(int k =1 ; k <= 16 ;k++)
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
			if( chargeMode == 0 )
				 gMode = 10;
			else 
				gMode = 11;
			fade();
			break;

			case 10:
			{
	
				for(unsigned int j =0 ; j < 50 ;j++) {
					for(unsigned int i =0 ; i < NUM_TLC5947*24 ;i++){
						LEDChannels[i]=rand()%2047;
					}
				
				
			  	WriteLEDArray(NUM_TLC5947);  
				}

			 gMode=11;
		//	fade();
			 }
			 brite =1;britedir=10;
			break;


			case 11:
				for(int i=0;i<80;i+=4) {
					//Clear(i);
					brite+=britedir;
					if (brite>6050) britedir=-30;
					if (brite<40) britedir=30;

					LEDscan2(brite,i,1);
					LEDscan2Add(brite,i-80,1);
					for(int t = 0 ; t < NUM_TLC5947 ; t++ ) 
						WriteLEDArray(1);
					//_delay_us(100);
				}
				
				if( (rand() % 20 ) == 5 )  {
					if( chargeMode == 0 )
						 gMode = 12;
					else 
						gMode = 0;
				}
				break;
	
			case 12:
			{


				
				for(unsigned int i =1 ; i < 6 ;i++){
				LEDscan(0,0,1);
					for (unsigned int offset = 0; offset < 360; offset += 16) {

						LEDscan2Add(4096/i,offset,1);
						for(int k =1 ; k <= 16 ;k++)
					  	  WriteLEDArray(1);  

 					}
				}
			 gMode=0;
			}
			break;

		case 13:
			for (int r=0;r<NUM_TLC5947*24;r+=24){
				for (int x=r;x<r+24;x+=1) {
					LEDChannels[x]=rand()/1024;
					if (LEDChannels[x]>30) LEDChannels[x]=4095;
						else if (LEDChannels[x]<10) LEDChannels[x]=0;
					}
					for(int i=0;i<NUM_TLC5947*24;i+=24){
						WriteArrayOffset(NUM_TLC5947,i);
						_delay_ms(10);
					}
				}

			gMode = 14;

		break;

		case 14:
			#define NUM_DOTS (6)
			{
			signed char  DotX[NUM_DOTS];
			signed char  DotY[NUM_DOTS];
			signed char  DotDirX[NUM_DOTS];
			signed char  DotDirY[NUM_DOTS];
			
			int		ran;
			
			for (unsigned char x=0;x<NUM_DOTS;x++) {DotX[x]=0;DotY[x]=12; DotDirX[x]=1;DotDirY[x]=0;}
			
			for (unsigned char y=0;y<250;y++) 
			{
				for (int x=0;x<NUM_TLC5947*24;x++) LEDChannels[x]*=.75;
				for (int x=0;x<NUM_DOTS;x++) {
					LEDChannels[((DotX[x]*24)+(DotY[x]))] = 2000;
					//DotX[x]+=-1;
					DotX[x]+=DotDirX[x];
					DotY[x]+=DotDirY[x];
					if (DotX[x]>15) DotX[x]-=16;
					if (DotX[x]<0) DotX[x]+=16;
					if (DotY[x]>23) DotY[x]-=24;
					if (DotY[x]<0) DotY[x]+=24;
					ran=rand();
					if ((ran>31744)&&DotDirX[x]!=0) {DotDirX[x]=0;DotDirY[x]=1;}
						else if ((ran>31744)&&DotDirY[x]!=0) {DotDirY[x]=0;DotDirX[x]=1;}
						else if ((ran>30720)&&DotDirX[x]!=0) {DotDirX[x]=0;DotDirY[x]=-1;}
						else if ((ran>30720)&&DotDirY[x]!=0) {DotDirY[x]=0;DotDirX[x]=-1;}
					}
				WriteArrayOffset(NUM_TLC5947,0);
				_delay_ms(rand()%100);
				}
			}


/// next mode
			gMode = 0;

			break;
		}
	}


	return 0;
}

