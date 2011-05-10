#if 0

#include<avr/io.h> 
#include<avr/interrupt.h> 
#include <util/delay.h>
#include <avr/pgmspace.h>
int value=0; 

static const char PROGMEM sinetable[32] =
{ 
   127,  // Put 32 step 8 bit sine table into array. 
   152, 
   176, 
   198, 
   217, 
   233, 
   245, 
   252, 
   254, 
   252, 
   245, 
   233, 
   217, 
   198, 
   176, 
   152, 
   128, 
   103, 
   79, 
   57, 
   38, 
   22, 
   10, 
   3, 
   0, 
   3, 
   10, 
   22, 
   38, 
   57, 
   79, 
   103
  };

int potval=128; 
unsigned char avalue,avalue1,val;

void ADC_convert() 
{ 
	avalue=0x00; 
	avalue1=0x00; 

	ADCSRA = 	(1<<ADEN)|(1<<ADSC) ;

	//delay(50); 
	while(ADCSRA & 0x40); // wait for the end of the conversion 
	_delay_us(10); 
	avalue = ADCL; 
	avalue1 = ADCH; 

	potval = avalue1;
}

int main(){ 
    
	DDRC=0xff;

    DDRA=0;    //porta output 
    DDRB=0;    //portb output 
    DDRE=0;    //porte output 
	//ADCSRA |= A_DEN; //ENABLE ADC 
	
	ADMUX  = 0x40; //reference external voltage, channel selection 
	ADCSRA = (1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0) ; //starting the conversion 

//	ADCSRA |= 0X80; //ENABLE ADC 

    /*----------------------------------------- 
    ; 
    ; Timer/Counter1 
    ; 
    -----------------------------------------*/ 

    /*----------------------------------------- 
    ; Timer/Counter1 
    -----------------------------------------*/ 
    //COM1A1 = 0 & COM1A0 = 1 => Toggle OC1A 
    //COM1B1 = 0 & COM1B0 = 0 => normal OC1B 
    //COM1C1 = 0 & COM1C0 = 0 => Normal OC1C 
    //WGM11 = 0 & WGM10 = 0 => CTC 
    TCCR1A = (0<<COM1A1)|(1<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)|(0<<COM1C1)|(0<<COM1C0)|(0<<WGM11)|(0<<WGM10); 
    
    //WGM13 = 0 & WGM12 = 1 => CTC 
    //CS12 = 0 & CS11 = 0 & CS10 = 1 => Clki_o 
    TCCR1B = (0<<WGM13)|(1<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10); 
    
    //FOC1A = 1 => Force output compare A 
    //FOC1B = 0 => Force output compare B 
    //FOC1B = 0 => Force output compare B 
    TCCR1C = (1<<FOC1A)|(0<<FOC1B)|(0<<FOC1C); 
    
    //OCR1A = 6666;    => 600Hz 
    //OCR1A = 3333; => 1.2KHz 
    //OCR1A = 1666; => 2.4KHz <- 
    OCR1A = 1666; 

//OCIE1A = 1 => Timer/Counter1, Output Compare A Match Interrupt Enable 
	TIMSK = (1<<OCIE1A); 

    //Clear Timer1 Interrupt flag 
    TIFR = (1<<OCF1A); 

    /*----------------------------------------- 
    ; 
    ; Timer/Counter2 
    ; 
    -----------------------------------------*/ 
    //CTC, clki_o/256 
    TCCR2 = (0<<FOC2)|(0<<WGM20)|(0<<COM21)|(0<<COM20)|(1<<WGM21)|(1<<CS22)|(0<<CS21)|(0<<CS20); 


//Timer2 compare output interrupt enable 
	TIMSK |= (1<<OCIE2);

    //Compare every 1.344mS 
    OCR2 = 124; 
    
    //Clear interrupt flag 
    TIFR=(1<<OCF2); 

    sei(); 


    while(1){ 

        ADC_convert();
    } 

} 

static int ic =0;

ISR(TIMER1_COMPA_vect){ 

}
    
 

ISR(TIMER2_COMP_vect)
{ 


 	PORTC = pgm_read_byte(&sinetable[ic++]); 


	 OCR2=potval/4;//sinetable[ic]/8;  //TCNT2

	if(ic>=32){
		ic=0; 
	}  
	
}
    

#endif
#if 1

#include <avr/io.h> 
#include <avr/eeprom.h> 
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include <util/delay.h>

#include <stdlib.h>


/* *** user macros ***/

/* for upto 8-bit binary constants */
#define B8(d) ((unsigned char)B8__(HEX__(d)))

/* for upto 16-bit binary constants, MSB first */
#define B16(dmsb,dlsb) (((unsigned short)B8(dmsb)<<	+ B8(dlsb)))

/* for upto 32-bit binary constants, MSB first */
#define B32(dmsb,db2,db3,dlsb) (((unsigned long)B8(dmsb)<<24)	 \
	+ ((unsigned long)B8(db2)<<16) \
	+ ((unsigned long)B8(db3)<<	 \
	+ B8(dlsb))



//  state definitions for each individual LED
//  A2  A1  A0  A4  A3
//  PB0 PB1 PB2 PB3 PB4  
//  PA2 PA1 PA0 PA4 PA3

// 00 = LOW
// 01 = HIGH
// 11 = X (input)

static const unsigned char PROGMEM led30[] = {

	B8( 11),  B8(11110001 ),		// LED1
	B8( 11) , B8(11110100 ),		// LED2
	B8( 11) , B8(11001101 ),		// LED3
	B8( 11) , B8(11011100 ),		// LED4
	B8( 11) , B8(00111101 ),		// LED5

	B8( 11) , B8(01111100 ),		// LED6
	B8( 11) , B8(11000111 ),		// LED11
	B8( 11) , B8(11010011 ),		// LED12
	B8( 11) , B8(00110111 ),		// LED13
	B8( 11) , B8(01110011 ),		// LED14

	B8( 00) , B8(11110111 ),		// LED15
	B8( 01) , B8(11110011 ),		// LED16
	B8( 11) , B8(00011111 ),		// LED19
	B8( 11) , B8(01001111 ),		// LED20
	B8( 00),  B8(11011111 ),		// LED21

	B8( 01) , B8(11001111 ),		// LED22
	B8( 11) , B8(11011111 ),		// LED23
	B8( 11) , B8(11001111 ),		// LED24
	B8( 00) , B8(01111111 ),		// LED25
	B8( 01) , B8(00111111 ),		// LED26

};


unsigned int X = 0;
signed char XDIR =1;

// this routine is written to help understand how the process works, not fast.
void SetRC( unsigned char R,unsigned char C ) 
{	
	unsigned char b1,b2;
	unsigned char portb, ddrb;

	if ( C  > 4  ) C = 0;
	if ( R  > 5  ) R = 0;

	// get current values, Can't cache PORTA/DDRA since the sound interrupt can change, 
	// should cache DDRD6/PORTD6 instead
	portb = PORTA;
	ddrb = DDRA;

//	read two values from array, we use pgm_read_byte because the data is in program memory space
// its 4*2 because there are 4 leds in each column, and two bytes per led
	b1 = pgm_read_byte(&led30[ (R*(4*2))+(C*2)   ] ) ;
	b2 = pgm_read_byte(&led30[((R*(4*2))+(C*2))+1] ) ;

//	b1 = led30[ (R*(4*2))+(C*2)   ]  ;
//	b2 = led30[((R*(4*2))+(C*2))+1];
	
// if 10 == 11 then set to don't care (input ) tristate for PB0
	if( GET_BIT( b1,1 )==1 && GET_BIT( b1,0 )==1 ) {

		CLR_BIT(ddrb,2 );

	} else { 

		// otherwise its an output
		SET_BIT(ddrb ,2 );
	}

// if 76 == 11 then set to don't care (input ) tristate  for PB1
	if( GET_BIT( b2,7 )==1 && GET_BIT( b2,6 )==1 ) {

		CLR_BIT(ddrb,1 );

	} else { 

		// otherwise its an output
		SET_BIT(ddrb ,1 );
	}

// if 54 == 11 then set to don't care (input ) tristate for PB2
	if( GET_BIT( b2,5 )==1 && GET_BIT( b2,4 )==1 ) {

		CLR_BIT(ddrb,0 );

	} else { 

		// otherwise its an output
		SET_BIT(ddrb ,0 );
	}

// if 32 == 11 then set to don't care (input ) tristate for PB3
	if( GET_BIT( b2,3 )==1 && GET_BIT( b2,2 )==1 ) {

		CLR_BIT(ddrb,4 );

	} elseF { 

		// otherwise its an output
		SET_BIT(ddrb ,4 );
	}

// if 10 == 11 then set to don't care (input ) tristate for PB4
	if( GET_BIT( b2,1 )==1 && GET_BIT( b2,0 )==1 ) {

		CLR_BIT(ddrb,3 );

	} else { 

		// otherwise its an output
		SET_BIT(ddrb ,3 );
	}

/// all of the DDR's are set


// if 10 == 00 then set to low for PB0
	if( GET_BIT( b1,1 )==0 && GET_BIT( b1,0 )==0 ) {

		CLR_BIT(portb,2); //set to off

	// if 32 == 01 then set to high for PB0
	} else if( GET_BIT( b1,1 )==0 && GET_BIT( b1,0 )==1 ) { 

		// set to on
		SET_BIT(portb ,2 );
	}

// if 76 == 00 then set to low for PB1
	if( GET_BIT( b2,7 )==0 && GET_BIT( b2,6 )==0 ) {

		CLR_BIT(portb,1); //set to off

	// if 32 == 01 then set to high for PB1
	} else if( GET_BIT( b2,7 )==0 && GET_BIT( b2,6 ) ==1 ) { 

		// set to on
		SET_BIT(portb ,1 );
	}


// if 54 == 00 then set to low for PB2
	if( GET_BIT( b2,5 )==0 && GET_BIT( b2,4 )==0 ) {

		CLR_BIT(portb,0); //set to off

	// if 32 == 01 then set to high for PB2
	} else if( GET_BIT( b2,5 )==0 && GET_BIT( b2,4 )==1 ) { 

		// set to on
		SET_BIT(portb ,0 );
	}

// if 32 == 00 then set to low for PB3
	if( GET_BIT( b2,3 )==0 && GET_BIT( b2,2 )==0 ) {

		CLR_BIT(portb,4); //set to off

	// if 32 == 01 then set to high for PB3
	} else if( GET_BIT( b2,3 ) ==0 && GET_BIT( b2,2 )==1 ) { 

		// set to on
		SET_BIT(portb ,4 );
	}

// if 10 == 00 then set to low for PB4
	if( GET_BIT( b2,1 )==0 && GET_BIT( b2,0 )==0 ) {

		CLR_BIT(portb,3); //set to off

// if 32 == 01 then set to high for PB4
	} else if( GET_BIT( b2,1)==0 && GET_BIT( b2,0 )==1 ) { 

		// set to on
		SET_BIT(portb ,3 );
	}

	//set all at once.
	PORTA = portb;
	DDRA = ddrb;

}

static const unsigned char PROGMEM pattern[]  =
{ 

//4x5	

	1,1,1,1,
	1,1,1,1,
	1,1,1,1,
	1,1,1,1,
	1,1,1,1,

	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,

};
 int potval; 
 char sinetable [32]; 
// Speaker drive interrupt
//SIGNAL( SIG_TIMER1_COMPA ) 
void ab(void)
{
	// toggle speaker on/off
	//SWITCH_BIT( PORTD , 4 );

	X += XDIR;

	if (X > 10000 ) { 
		XDIR = -((rand()%9)+1);
	} else if ( X < 20) {
		XDIR = ((rand()%9)+1);
	}

	OCR1A = X+(rand()%100);
}

void arraysetup(void)
{ 
   sinetable[0]=127;  // Put 32 step 8 bit sine table into array. 
   sinetable[1]=152; 
   sinetable[2]=176; 
   sinetable[3]=198; 
   sinetable[4]=217; 
   sinetable[5]=233; 
   sinetable[6]=245; 
   sinetable[7]=252; 
   sinetable[8]=254; 
   sinetable[9]=252; 
   sinetable[10]=245; 
   sinetable[11]=233; 
   sinetable[12]=217; 
   sinetable[13]=198; 
   sinetable[14]=176; 
   sinetable[15]=152; 
   sinetable[16]=128; 
   sinetable[17]=103; 
   sinetable[18]=79; 
   sinetable[19]=57; 
   sinetable[20]=38; 
   sinetable[21]=22; 
   sinetable[22]=10; 
   sinetable[23]=3; 
   sinetable[24]=0; 
   sinetable[25]=3; 
   sinetable[26]=10; 
   sinetable[27]=22; 
   sinetable[28]=38; 
   sinetable[29]=57; 
   sinetable[30]=79; 
   sinetable[31]=103; 
 } 

ISR(TIMER2_OVF_vect) 
 { 
 static int i;
 	PORTC=(sinetable[i++]); 
	TCNT2=potval; 
	if(i==32){
		i=0; 
	}
}

int main(void)
{
	unsigned int x,speed=4;;
	unsigned int R,C;
	unsigned long i;

   TCCR2 = 0; 
   TCNT2=455;    //455 outputs 1.007khz 

   TCCR2 = B8( 00000010 ); 

   //Timer2 Overflow Interrupt Enable 
   TIMSK = 1<<TOIE2; 

	// setup timer
//	TCCR1B = (1 << WGM12) | TIMER1_PRESCALE_1;
	//OCR1A = (uint16_t)1000;
	//TIMSK |= 1 << OCIE1A;   // Output Compare Interrupt Enable (timer 1, OCR1A) 

//  speaker off (eats power otherwise)
	//DDRD = 0xff;
	//PIND = 0x0;
	//PORTD = 0x10; 
	DDRA = 0;



	arraysetup();



	// all outputs
	DDRC = 0xff ;

// switch on interrupts ( start speaker )

	//sei();

// index to start
	i = 0;

	X = 0;


//col, row
	C  = 0;

	PORTC = 0 ;

	while(1)  {
		PORTC++;
		C=2;
		SetRC(0,C);
//		_delay_ms(10);
	}

	for(;;) {	

		for ( x = 0 ; x < speed*100; x ++) {

		//if( x == 9 ) break;
;
			for ( R= 0 ; R < 5 ; R ++ ) {

				for ( C= 0 ; C < 4 ; C ++ ) {

					unsigned char code;

					if ( i < sizeof(pattern) ) {
						
						// fetch value
						code =  pgm_read_byte( &pattern[i]  );

					} else {

						break;
					}
	
					// if first column
					if (C == 0 && code > 1 ) {

						// skip rest of row ( i++ later makes it 4)
						i += 3;

						// set speed
						speed = code - 2;
					}
					else
						if( code  ) {
							SetRC(C,R);
						} 


					i++;

				}
			}

			// go back to beginning of pattern, repeat for speed%10
			i -= (4*5);
	
		}	

		if( speed < 20 ) {

			// skip a row
			i += 4;

		} else {

			speed -- ;

			if(speed == 21) {
				i += (4*5);
			}
		}


		if( i >=  sizeof(pattern) ) {
			i = 0;
		}
	}
}


#endif
