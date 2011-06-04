/*
 * This file is part of 4bitsynth.
 *
 * 4bitsynth is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * 4bitsynth is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 4bitsynth.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>

#define low(x)   ((x) & 0xFF) 
#define high(x)   (((x)>>8) & 0xFF) 

#define NUM_KNOBS         (9)
#define NUM_ADC_SAMPLES   (32)
#define NUM_ADC_CHANNELS  (8)

#include "triangle.h"
#include "songs.h"

typedef struct
{
  uint16_t val;
  uint32_t accum;
} knob_t; 

#define CPU_FREQUENCY		( (unsigned long)16000000UL )
#define	BAUD_RATE			( (unsigned long)31250UL )
#define MUXCTL      		D
#define MUXCTL_PIN  		5

#define PASTE(x,y)  		x ## y
#define PORT(x)     		PASTE(PORT,x)
#define PIN(x)      		PASTE(PIN,x)
#define DDR(x)      		PASTE(DDR,x)


#define set_bit(v,b)        v |= _BV(b)
#define clear_bit(v,b)      v &= ~_BV(b)
#define pulse_bit(v,b)      do { set_bit(v,b); clear_bit(v,b); } while(0)
#define pulse_bit_low(v,b)  do { clear_bit(v,b); set_bit(v,b); } while(0)

volatile uint8_t adctimer = 0;
knob_t knobs[NUM_KNOBS] = {{0}};
void play(const unsigned char *  p);

unsigned char selfPlay = 1;

#if 0 
static const unsigned char  song[][512] = {
	"TheSimpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6\0\0",
	"Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6\0\0",
	"Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6\0\0",
	"Looney:d=4,o=5,b=140:32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f\0\0",
};

#endif

#define		PITCH_KNOB		( 0 )


typedef struct songList_tag   {
	unsigned short delay ;
	const unsigned char * const p
} songList;


songList songs[] = {
	{5000,&daft},
};

#define shortdelay(); 			asm("nop\n\tnop\n\t");

#define MIN_OF(x,y) ((x) < (y) ? (x) : (y))
#define MAX_OF(x,y) ((x) > (y) ? (x) : (y))

#define GET_BIT(a,n)    ((a >> n) & 1)
#define SET_BIT(a,n)    (a|=(1<<n))
#define CLR_BIT(a,n)    (a&=~(1<<n))
#define SWITCH_BIT(a,n) (a^=(1<<n))
#define IS_BIT_SET(a,n) (a & (1<<n))
#define IS_BIT_CLR(a,n) (~(a & (1<<n)))


// 	PORTA,DDRA,PINA
#define ddr_a(r,c)  charlieplex[ (r*(3*4)) + ((c*3)+1) ]
#define pin_a(r,c)  charlieplex[ (r*(3*4)) + ((c*3)+2) ]
#define port_a(r,c) charlieplex[ (r*(3*4)) + ((c*3)+0) ]

#define HEX__(n) 0x##n##LU

/* 8-bit conversion function */
#define B8__(x)	((x&0x0000000FLU)?1:0)	\
			   +((x&0x000000F0LU)?2:0)	\
			   +((x&0x00000F00LU)?4:0)	\
			   +((x&0x0000F000LU)?8:0)	\
			   +((x&0x000F0000LU)?16:0)	\
			   +((x&0x00F00000LU)?32:0)	\
			   +((x&0x0F000000LU)?64:0)	\
			   +((x&0xF0000000LU)?128:0)

/* *** user macros ***/

/* for up to 8-bit binary constants */
#define B8(d) ((unsigned char)B8__(HEX__(d)))

/* for up to 16-bit binary constants, MSB first */
#define B16(dmsb,dlsb) (((unsigned short)B8(dmsb)<<	+ B8(dlsb)))

/* for up to 32-bit binary constants, MSB first */
#define B32(dmsb,db2,db3,dlsb) (((unsigned long)B8(dmsb)<<24)	 \
							 + ((unsigned long)B8(db2)<<16)		 \
							 + ((unsigned long)B8(db3)<<		 \
							 + B8(dlsb))

//  State Definitions for each individual LED
//  A2  A1  A0  A4  A3
//  PB0 PB1 PB2 PB3 PB4  
//  PA2 PA1 PA0 PA4 PA3
//
//	00 = LOW
//	01 = HIGH
//	11 = X (input)
static const unsigned char PROGMEM led30[] = 
{
	B8( 00) , B8(01111111 ),		// LED25
	B8( 11) , B8(00011111 ),		// LED19
	B8( 11) , B8(11000111 ),		// LED11
	B8( 11) , B8(11110001 ),		// LED1
	
	B8( 01) , B8(00111111 ),		// LED26
	B8( 11) , B8(01001111 ),		// LED20
	B8( 11) , B8(11010011 ),		// LED12
	B8( 11) , B8(11110100 ),		// LED2
	
	B8( 00) , B8(11110111 ),		// LED15
	B8( 00),  B8(11011111 ),		// LED21
	B8( 11) , B8(00110111 ),		// LED13
	B8( 11) , B8(11001101 ),		// LED3
	
	B8( 01) , B8(11110011 ),		// LED16
	B8( 01) , B8(11001111 ),		// LED22
	B8( 11) , B8(01110011 ),		// LED14
	B8( 11) , B8(11011100 ),		// LED4

	B8( 01) , B8(11111100 ),		// LED8
	B8( 00),  B8(11111101 ),		// LED7
	B8( 11) , B8(01111100 ),		// LED6
	B8( 11) , B8(00111101 ),		// LED5
};


// sample an ADC channel
void read_adc(knob_t *k)
{

	uint16_t val = ADCL;
	val += (ADCH << 8);
	// accumulate value
	k->accum += val;

	// take average when timer overflows
	if (adctimer == 0)
	{
		k->val = k->accum / NUM_ADC_SAMPLES;
		k->accum = 0;
	}
}

void init_interrupts() {
	//Turn on USART reception and | RX Interrupt
	UCSR0B = (1 << RXEN0) | (1 << RXCIE0);

	//8-bit, 1 stop, Asynch.
	UCSR0C = (0 << UMSEL0) | (0 << UPM00) | (0 << USBS0) | (3 << UCSZ00);

	/* Set the baud rate to 31250 for MIDI */

	// These were for the original synth, changed for the l1 human badge
	//UBRR0L = 0x27; // For 20MHz Clock
	//UBRR0L = 0x13;		// FOr 10MHz Clock

	UBRR0H = high((CPU_FREQUENCY / (BAUD_RATE * 16)) - 1);
	UBRR0L = low((CPU_FREQUENCY / (BAUD_RATE * 16)) - 1);

	/* Enable USART Receive interrupt */
	enable_USART_interrupts();

}

void init_io( void )
{
	//b0 - b3 of PORT C is output
	DDRC = 0xFF;

	// for led
	DDRE = 0xff;

	//b4 0 v7 of PORT D is input (MIDI Channel selection)
	DDRD &= 0b00001111;

	//enable internal pull-up resistors for MIDI Channel selection bits
	PORTD |= 0b11110000;
}

void init_timers( void )
{

	//8-bit timer 0 for decay, sweep, vibrato effects?
	//Enable Overflow interrupts for Timer 0
	TIMSK = 0b00000001;

	//Normal counter operation
	TCCR1A = 0b00000000;

	//Divide by 1024 prescalar
	TCCR1B = 0b00000101;

	//Start terminal count at zero
	TCNT0 = 0x00;

	//16-bit timer 1 for main frequency generation
	TIMSK |= _BV(TOIE1) | _BV( OCIE1A ) ;
	//TIMSK |= 0b00100000; // Enable A and B compare interrupts

	TCCR1A = 0b00000001;

	// Prescaler 1, Fast PWM
	TCCR1B = 0b00010001;

	//Start count at zero now
	TCNT1H = 0;
	TCNT1L = 0;
}

void init_adc( void ) 
{
	// set up ADC
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); // 1/128 prescaler
	ADCSRA |= _BV(ADEN); 							// enable ADC
}

int main(void) 
{
	/* Disable interrupts at first */
	cli();

	/* Setup I/O Pins */
	init_io();


	/* Setup Timers */
	init_timers();

	/* Enable USART and Pin Interrupts */
	init_interrupts();


	PORTB = 0xFF;

	OCR1A = 500;

	/* ADC setup */
	init_adc();

	/*
		Pitch bend needs to start at 64 (middle value) or pitch will start out
		two half steps too low (0 = max bend downward)
	 */
	current_midi_pb_l = current_midi_pb_h = 64;

	num_bytes = 0;
	num_ccs = 0;
	num_pbs = 0;
	byte_ready = 0;
	midi_channel = 0;
	master_volume = 127;

	amplitude = 0;
	fine_pitch_bend = 0;
	note_on_gate = 0;

	sweep_enabled = 1;
	sweep_direction = SWEEP_UP;
	sweep_amount = 0;
	sweep_loop_enabled = 1;
	fake_16_timer = 0;

	/* Finally, enable global interrupts */
	sei();

	//play(song[1]);

  	adctimer = 0;

	/*Main Loop*/
	while (1) {

	    uint8_t adcchan = 0;

	    adctimer++;
	    adctimer &= (NUM_ADC_SAMPLES-1);

	    // read ADC channels
	    for (adcchan = 0; adcchan < NUM_ADC_CHANNELS; adcchan++) {

			ADMUX = _BV(REFS0) | adcchan;
			ADCSRA |= _BV(ADSC);

//			loop_until_bit_is_clear(ADCSRA, ADSC);
//			read_adc(knobs+adcchan);


	    }

		check_byte_received();


	    // update values after we've read averaged values from all knobs
	    if ( adctimer == 0 ) {

			fine_pitch_bend = knobs[PITCH_KNOB].val;	

		}
	
	
		scrollLEDS();
	
		
	}

	return 0;
}
unsigned short daftIndex = 0;

/* USART Received byte interrupt (get MIDI byte)*/
ISR(USART0_RX_vect) {
	byte_received = UDR0;
	byte_ready = 1;
	PORTE ^=0x1;
}

ISR(TIMER1_COMPA_vect)
{

	PORTE ^=0x1;

	if(note_on_gate == 1) {
		fivebit_counter ++;

		//Reset counter if 'overflowed'
		if(fivebit_counter> 31) fivebit_counter = 0;

		//Each amplitude b0-3 is XNORed with b4 to create output
		unsigned char strobe = (fivebit_counter & 0b00010000) >> 4;
		if(strobe == 1) {
			amplitude = ~(fivebit_counter ^ 0b00001111);
		} else {
			amplitude = ~(fivebit_counter ^ 0b00000000);
		}
		//amplitude &= 0b11111111;
		//amplitude |= (strobe << 4);
		
		PORTC = amplitude;
	}

}

ISR(TIMER1_OVF_vect) 
{

	/* Sweep */
	if((sweep_enabled == 1) && (sweep_amount> 0) && (note_on_gate == 1)) {
		fake_16_timer ++;
		if(fake_16_timer >= 2) {

			unsigned int sweep_mod = sweep_amount * note_table[playing_midi_note] / (8 * playing_midi_note);

			//Sweep down mode
			if(sweep_direction == SWEEP_DOWN) {
				if(frequency < MAXIMUM_FREQ) {
					frequency += sweep_mod;
				}
				else
				{
					if(sweep_loop_enabled == 1) {
						frequency = note_table[playing_midi_note];
					}
					else {
						amplitude = 0;
					}
				}
			}
			//Sweep up mode
			else
			{
				if(frequency> MINIMUM_FREQ+sweep_mod) {
					frequency -= sweep_mod;
				}
				else
				{
					if(sweep_loop_enabled == 1) {
						frequency = note_table[playing_midi_note];
					}
					else {
						amplitude = 0;
					}
				}
			}

			update_frequency(frequency);
			fake_16_timer = 0;
		}

	}

}

unsigned short counter = 2000;

void check_byte_received() 
{
#if 1

	if( selfPlay ) {
		if( counter == 0 ) {
			byte_received = pgm_read_byte(&black[daftIndex]);
			daftIndex ++; 
			if( daftIndex == sizeof( daft ) ) {
				daftIndex = 0;

			}
			byte_ready = 1;
			counter = 2000;

		} else {
			counter -- ;
		}
		}
#endif

	//Is there a byte waiting in the buffer?
	if (byte_ready == 1) {
		//Is this a stupid byte like Clock or Active sensing?
		if (byte_received < 0xF0) {
			//Is this a status byte? ...
			if (byte_received >= 0x80) {
				unsigned char temp_midi_channel = byte_received & 0x0F;
				//Is this for one of our channels?
				if ( 1 ) // (temp_midi_channel == midi_channel) 
				{
					current_midi_channel = temp_midi_channel;

					//What kind of status byte is this?
					unsigned char status_type = (byte_received & 0xF0);
					switch (status_type) {
					case (MIDI_STATUS_TYPE_NOTEON):
						current_midi_status = MIDI_STATUS_NOTEON;
					break;
					case (MIDI_STATUS_TYPE_NOTEOFF):
						current_midi_status = MIDI_STATUS_NOTEOFF;
					break;
					case (MIDI_STATUS_TYPE_CC):
						current_midi_status = MIDI_STATUS_CC;
					break;
					case (MIDI_STATUS_TYPE_PB):
						current_midi_status = MIDI_STATUS_PB;
					break;
					default:
						current_midi_status = MIDI_STATUS_NONE;
					}
				} else {
					current_midi_status = 0;
				}
			}
			// ... or is it a data byte?
			else {
				switch (current_midi_status) {
				case (MIDI_STATUS_NOTEON):

					//Is this a velocity byte?
					if (num_bytes > 0) {

						//If the velocity sent was 0, then this is really a NOTE-OFF
						if (byte_received > 0) {
							//current_midi_velocity = byte_received;
							playing_midi_note = current_midi_note;
							note_on();
						} else {
							if (current_midi_note == playing_midi_note) {
								//current_midi_velocity = 0;
								note_off();
							}
						}
						num_bytes = 0;
					}

				//Or is this a note data byte?
					else {
						current_midi_note = byte_received;
						num_bytes++;
					}

				//Clear the byte so we don't process it twice
				clear_byte_received();

				break;

				case (MIDI_STATUS_NOTEOFF):
					//Is this a velocity byte?
					if (num_bytes > 0) {
						//Who cares? We aren't implementing aftertouch
						num_bytes = 0;
					} else {
						/* This is a note byte. Let's see if it's the same as the currently
						 * playing note. Only then will we note_off()
						 */

						if (byte_received == playing_midi_note) {
							note_off();
						}
						num_bytes++;
					}

				//Clear the byte so we don't process it twice
				clear_byte_received();

				break;

				case (MIDI_STATUS_CC):
					//Did we already get a CC Status byte?
					if (num_ccs > 0) {
						current_midi_ccdata = byte_received;
						process_cc();
					}
				//Or is this a new CC status byte?
					else {
						current_midi_cc = byte_received;
						num_ccs++;
					}
				break;

				case (MIDI_STATUS_PB):
					//How many PB related bytes have we gotten?
					switch (num_pbs) {
					case (0):
						//First byte is 7 LSB
						//Don't care about it for now
						//current_midi_pb_l = byte_received;

						num_pbs++;
					break;
					case (1):
						//Second byte has 7 MSB
						current_midi_pb_h = byte_received;
						//Combine to get 14 bytes 0 - 13
						//current_midi_pb = ((current_midi_pb_h << 7)|(current_midi_pb_l << 0));
						bend_pitch();

					break;
					}
				break;
				}
			}

		}
		byte_ready = 0;
	}
}

void enable_USART_interrupts() {
	UCSR0A = (1 << RXCIE0);
}

void disable_USART_interrupts() {
	UCSR0A = (0 << RXCIE0);
}

void note_on() {
	num_bytes = 0;
	//Reset main timer1
	TCNT1 = 0;
	//Set timer count corresponding to midi note and thus musical note
	frequency = note_table[current_midi_note];
	update_frequency(frequency);
	note_on_gate = 1;
}

void note_off() {
	num_bytes = 0;
	note_on_gate = 0;
}

void process_cc() {
	num_ccs = 0;

	switch (current_midi_cc) {

	case MASTER_VOLUME_CC:
		master_volume = current_midi_ccdata;
		break;
	case FINE_PITCH_CC:
		fine_pitch_bend = ((note_table[playing_midi_note - 1]
		                               - note_table[playing_midi_note]) * current_midi_ccdata) / 192;
		update_frequency(note_table[playing_midi_note]);
		break;

	case SWEEP_ENABLED_CC:
		if (current_midi_ccdata > 64)
			sweep_enabled = 1;
		else
			sweep_enabled = 0;
		break;

	case SWEEP_DIRECTION_CC:
		if (current_midi_ccdata > 64)
			sweep_direction = SWEEP_UP;
		else
			sweep_direction = SWEEP_DOWN;
		break;

	case SWEEP_AMOUNT_CC:
		sweep_amount = current_midi_ccdata;
		OCR1B = sweep_amount * 1024;
		break;
	case SWEEP_LOOP_ENABLED_CC:
		if (current_midi_ccdata > 64)
			sweep_loop_enabled = 1;
		else
			sweep_loop_enabled = 0;
		break;

	}
}

void update_frequency(unsigned int new_frequency) 
{
	OCR1A = new_frequency + fine_pitch_bend;
}

void bend_pitch() {
	num_pbs = 0;

	if (current_midi_pb_h > 63) {
		distance
		= ((note_table[playing_midi_note]
		               - note_table[playing_midi_note + 2])
		               * (current_midi_pb_h - 63)) / 64;
		update_frequency(note_table[playing_midi_note] - distance);
	} else if ((current_midi_pb_h < 63) && (playing_midi_note > 1)) {
		distance = ((note_table[playing_midi_note - 2]
		                        - note_table[playing_midi_note]) * (64 - current_midi_pb_h))
		                        / 64;
		update_frequency(note_table[playing_midi_note] + distance);
	}

}

void clear_byte_received() {
	byte_received = 0;
}

void check_channel_set() {
	midi_channel = 0;
	//Get 4-bit (0-16) MIDI CHannel from PORTD b4-b7)
	//midi_channel |= (~PIND & 0xF0) >> 4;

}


#define OCTAVE_OFFSET (0)
#define TONE_PIN 13
#define isdigit(n) (n >= '0' && n <= '9')

unsigned char pgm_read_byte1( const unsigned char *p) 
{
	return *p;
}

void play(const unsigned char *p)
{
  unsigned short default_dur = 4;
  unsigned short default_oct = 6;
  unsigned short bpm = 63;
  unsigned short num;
 unsigned long wholenote;
  unsigned long duration;
  unsigned char note;
  unsigned char scale;
  
	
  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while( pgm_read_byte1(p) != ':' ) {

	p++;					// ignore name
  }
  	p++;                     // skip ':'
		
  // get default duration
  if(pgm_read_byte1(p) == 'd') {
    p++; p++;              // skip "d="
    num = 0;
    while(isdigit(pgm_read_byte1(p))) {
      num = (num * 10) + ((*p++) - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }


  // get default octave
  if(pgm_read_byte1(p) == 'o') {
    p++; p++;              // skip "o="
    num = pgm_read_byte1(p) - '0';p++;
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }


  // get BPM
  if(pgm_read_byte1(p) == 'b') {
    p++; p++;              // skip "b="
    num = 0;
    while(isdigit(pgm_read_byte1(p))) {
      num = (num * 10) + (pgm_read_byte1(p) - '0'); p++;
    }
	
    bpm = num;
    p++;                   // skip colon
  }

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

  // begin note loop
  while( pgm_read_byte1(p) ) {
    
	// first, get note duration, if available
    num = 0;
    
	while( isdigit(pgm_read_byte1(p)) ) {
      num = (num * 10) + ((*p++) - '0');
    }
    
    if( num ) { 
	  duration = wholenote / num;
    } else {
	  duration = wholenote / default_dur;  // we will need to check if we are a dotted note after
	}
	
    // get the note
    switch( pgm_read_byte1(p) ) {
	  
      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 10;
        break;
      case 'f':
        note = 9;
        break;
      case 'g':
        note = 7;
        break;
      case 'a':
        note = 5;
        break;
      case 'b':
        note = 3;
        break;
      case 'p':
      default:
        note = 0;
    }


    p++;

    // get optional '#' sharp
    if(pgm_read_byte1(p) == '#') {
      note++;
      p++;
    }

    // get optional '.' dotted note
    if(pgm_read_byte1(p) == '.') {
      duration += duration/2;
      p++;
    }
  
    // get scale
    if(isdigit(pgm_read_byte1(p))) {
      scale = pgm_read_byte1(p) - '0';
      p++;
    } else {
      scale = default_oct;
    }


	 //note = 11;
	 //scale  = 3+4;

    scale += OCTAVE_OFFSET;

    if(pgm_read_byte1(p) == ',')
      p++;       // skip comma for next note (or we may be at the end)

	duration /= 2;
	if( duration > 180 ) duration = 180;


    if( note ) {

	  // play the note

	  	current_midi_note = (((scale - 4) * 12) + note);

//		frequency = note_table[current_midi_note];

		note_on();

	 	_delay_ms( duration );
//		frequency = 0;
//	 	note_off();
  
	} else {
	
	 	note_off();
	  _delay_ms( duration );
	  
    }
  }
	
}

// simple sine wave output
uint8_t sine[]={
  0x80,0x83,0x86,0x89,0x8c,0x8f,0x92,0x95,0x98,0x9c,0x9f,0xa2,0xa5,0xa8,0xab,0xae,
  0xb0,0xb3,0xb6,0xb9,0xbc,0xbf,0xc1,0xc4,0xc7,0xc9,0xcc,0xce,0xd1,0xd3,0xd5,0xd8,
  0xda,0xdc,0xde,0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xed,0xef,0xf0,0xf2,0xf3,0xf5,
  0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfc,0xfd,0xfe,0xfe,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xfe,0xfd,0xfc,0xfc,0xfb,0xfa,0xf9,0xf8,0xf7,
  0xf6,0xf5,0xf3,0xf2,0xf0,0xef,0xed,0xec,0xea,0xe8,0xe6,0xe4,0xe2,0xe0,0xde,0xdc,
  0xda,0xd8,0xd5,0xd3,0xd1,0xce,0xcc,0xc9,0xc7,0xc4,0xc1,0xbf,0xbc,0xb9,0xb6,0xb3,
  0xb0,0xae,0xab,0xa8,0xa5,0xa2,0x9f,0x9c,0x98,0x95,0x92,0x8f,0x8c,0x89,0x86,0x83,
  0x80,0x7c,0x79,0x76,0x73,0x70,0x6d,0x6a,0x67,0x63,0x60,0x5d,0x5a,0x57,0x54,0x51,
  0x4f,0x4c,0x49,0x46,0x43,0x40,0x3e,0x3b,0x38,0x36,0x33,0x31,0x2e,0x2c,0x2a,0x27,
  0x25,0x23,0x21,0x1f,0x1d,0x1b,0x19,0x17,0x15,0x13,0x12,0x10,0x0f,0x0d,0x0c,0x0a,
  0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x03,0x02,0x01,0x01,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x05,0x06,0x07,0x08,
  0x09,0x0a,0x0c,0x0d,0x0f,0x10,0x12,0x13,0x15,0x17,0x19,0x1b,0x1d,0x1f,0x21,0x23,
  0x25,0x27,0x2a,0x2c,0x2e,0x31,0x33,0x36,0x38,0x3b,0x3e,0x40,0x43,0x46,0x49,0x4c,
  0x4f,0x51,0x54,0x57,0x5a,0x5d,0x60,0x63,0x67,0x6a,0x6d,0x70,0x73,0x76,0x79,0x7c};

// final sample that goes tote DAC
uint16_t sample;


// variables for oscillators
uint16_t accumulator = 0;  // large number holds phase
uint8_t indexi = 0;        // index for wave lookup (the upper 8 bits of the accumulator)

// timer 1 is audio interrupt timer
ISR(TIMER3_COMPA_vect) 
{
  // calculate frequency mod
  accumulator = accumulator + frequency;  // add in pith, the higher the number, the faster it rolls over, the more cycles per second
  indexi = accumulator >> 8;   // use top 8 bits as wavetable index

  PORTC = sine[indexi];    // get sample from wave table
}

// Write to LED Array - Takes in a Row and Column
// this routine is written to help understand how the process works, not fast.
void SetRC( unsigned char R, unsigned char C ) 
{	
	unsigned char b1,b2;
	unsigned char port_a, ddr_a;

	if ( C  > 4  ) C = 0;
	if ( R  > 5  ) R = 0;

	// get current values, Can't cache PORTA/DDRA since the sound interrupt can change, 
	// should cache DDRD6/PORTD6 instead
	port_a = PORTA;
	ddr_a = DDRA;

	//	read two values from array, we use pgm_read_byte because the data is in program memory space
	// its 4*2 because there are 4 leds in each column, and two bytes per led
	b1 = pgm_read_byte(&led30[ (R*(4*2))+(C*2)   ] ) ;
	b2 = pgm_read_byte(&led30[((R*(4*2))+(C*2))+1] ) ;

	//	b1 = led30[ (R*(4*2))+(C*2)   ]  ;
	//	b2 = led30[((R*(4*2))+(C*2))+1];
	
	// if 10 == 11 then set to don't care (input ) tristate for PB0
	if( GET_BIT( b1,1 )==1 && GET_BIT( b1,0 )==1 ) {

		CLR_BIT(ddr_a,2 );

	} else { 

		// otherwise its an output
		SET_BIT(ddr_a ,2 );
	}

	// if 76 == 11 then set to don't care (input ) tristate  for PB1
	if( GET_BIT( b2,7 )==1 && GET_BIT( b2,6 )==1 ) {

		CLR_BIT(ddr_a,1 );

	} else { 

		// otherwise its an output
		SET_BIT(ddr_a ,1 );
	}

	// if 54 == 11 then set to don't care (input ) tristate for PB2
	if( GET_BIT( b2,5 )==1 && GET_BIT( b2,4 )==1 ) {

		CLR_BIT(ddr_a,0 );

	} else { 

		// otherwise its an output
		SET_BIT(ddr_a ,0 );
	}

	// if 32 == 11 then set to don't care (input ) tristate for PB3
	if( GET_BIT( b2,3 )==1 && GET_BIT( b2,2 )==1 ) {

		CLR_BIT(ddr_a,4 );

	} else { 

		// otherwise its an output
		SET_BIT(ddr_a ,4 );
	}

	// if 10 == 11 then set to don't care (input ) tristate for PB4
	if( GET_BIT( b2,1 )==1 && GET_BIT( b2,0 )==1 ) {

		CLR_BIT(ddr_a,3 );

	} else { 

		// otherwise its an output
		SET_BIT(ddr_a ,3 );
	}

	/// all of the DDR's are set


	// if 10 == 00 then set to low for PB0
	if( GET_BIT( b1,1 )==0 && GET_BIT( b1,0 )==0 ) {

		CLR_BIT(port_a,2); //set to off

	// if 32 == 01 then set to high for PB0
	} else if( GET_BIT( b1,1 )==0 && GET_BIT( b1,0 )==1 ) { 

		// set to on
		SET_BIT(port_a ,2 );
	}

	// if 76 == 00 then set to low for PB1
	if( GET_BIT( b2,7 )==0 && GET_BIT( b2,6 )==0 ) {

		CLR_BIT(port_a,1); //set to off

	// if 32 == 01 then set to high for PB1
	} else if( GET_BIT( b2,7 )==0 && GET_BIT( b2,6 ) ==1 ) { 

		// set to on
		SET_BIT(port_a ,1 );
	}


	// if 54 == 00 then set to low for PB2
	if( GET_BIT( b2,5 )==0 && GET_BIT( b2,4 )==0 ) {

		CLR_BIT(port_a,0); //set to off

	// if 32 == 01 then set to high for PB2
	} else if( GET_BIT( b2,5 )==0 && GET_BIT( b2,4 )==1 ) { 

		// set to on
		SET_BIT(port_a ,0 );
	}

	// if 32 == 00 then set to low for PB3
	if( GET_BIT( b2,3 )==0 && GET_BIT( b2,2 )==0 ) {

		CLR_BIT(port_a,4); //set to off

	// if 32 == 01 then set to high for PB3
	} else if( GET_BIT( b2,3 ) ==0 && GET_BIT( b2,2 )==1 ) { 

		// set to on
		SET_BIT(port_a ,4 );
	}

	// if 10 == 00 then set to low for PB4
	if( GET_BIT( b2,1 )==0 && GET_BIT( b2,0 )==0 ) {

		CLR_BIT(port_a,3); //set to off

	// if 32 == 01 then set to high for PB4
	} else if( GET_BIT( b2,1)==0 && GET_BIT( b2,0 )==1 ) { 

		// set to on
		SET_BIT(port_a ,3 );
	}

	//set all at once.
	PORTA = port_a;
	DDRA = ddr_a;
}

void scrollLEDS(void)
{
	static int c = 0;
	static int r = 0;

	static unsigned short count = 0;

	count ++ ;

	if ( count < 800  ) return;

	count = 0;

	SetRC(r,c);

	r++ ;

	if ( r == 5 )  {
		r = 0;
		c++ ;
	}

	if ( c == 4 ) { 
		c = 0;
	}
	 
}
