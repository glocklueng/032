//***************************************************************************
//
//  File........: sound.c
//
//  Author(s)...: ATMEL Norway
//
//  Target(s)...: ATmega169
//
//  Compiler....: GCC 3.3.1; avr-libc 1.0
//
//  Description.: AVR Butterfly sound routines
//
//  Revisions...: 1.0
//
//  YYYYMMDD - VER. - COMMENT                                       - SIGN.
//
//  20030116 - 1.0  - Created                                       - LHM
//  20031009          port to gcc/avr-libc                          - M.Thomas
//  20040123          added temp.-var.                              - n.n./mt
//  20051024          extracted from main butterfly code            - hb
//
//***************************************************************************

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "sound.h"
#include "timer0.h"

#include "furelise.h"
#include "mozart.h"
#include "minuet.h"
#include "auldlangsyne.h"
#include "sirene1.h"
#include "sirene2.h"
#include "whistle.h"
#include "invent8.h"
#include "fugue1.h"

#define WHICHSONG 7
const int *Songs[] PROGMEM   = { FurElise, Mozart, Minuet, AuldLangSyne, Sirene1, Sirene2, Whistle, Invent8, Fugue, 0};

/******************************************************************************
*
*   A song is defined by a table of notes. The first byte sets the tempo. A 
*   high byte will give a low tempo, and opposite. Each tone consists of two 
*   bytes. The first gives the length of the tone, the other gives the frequency. 
*   The frequencies for each tone are defined in the "sound.h". Timer0 controls 
*   the tempo and the length of each tone, while Timer1 with PWM gives the 
*   frequency. The second last byte is a "0" which indicates the end, and the
*   very last byte makes the song loop if it's "1", and not loop if it's "0".
*
******************************************************************************/

const char PLAYING[] PROGMEM   = "PLAYING";
const int *pSong;	// mt point to a ram location (pointer array Songs)

static char Volume = 80;
static char Duration = 0;
static char Tone = 0;
static char Tempo;

#define FALSE   0
#define TRUE    (!FALSE)

#define sbiBF(port,bit)  (port |= (1<<bit))   //set bit in port
#define cbiBF(port,bit)  (port &= ~(1<<bit))   //set bit in port

/*****************************************************************************
*
*   Function name : Sound_Init
*
*   Returns :       None
*
*   Parameters :    None
*
*   Purpose :       Set up Timer1 with PWM
*
*****************************************************************************/
void Sound_Init(void)
{
    TCCR1A = (1<<COM1A1);// | (1<<COM1A0); // Set OC1A when upcounting, clear when downcounting
    TCCR1B = (1<<WGM13);        // Phase/Freq-correct PWM, top value = ICR1
    
    sbiBF(TCCR1B, CS10);             // start Timer1, prescaler(1)    
    
	// mtA
    OCR1AH = 0; //OCR1AH = 0;     // Set a initial value in the OCR1A-register
    OCR1AL = Volume; //OCR1AL = Volume;     // This will adjust the volume on the buzzer, lower value => higher volume
    // mtE
    
    Timer0_RegisterCallbackFunction(Play_Tune);     // Timer/Counter0 keeps the right beat
}

int main(void) {
    sei();
    //CLKPR = (1<<CLKPCE);        // set Clock Prescaler Change Enable
    // set prescaler = 8, Inter RC 8Mhz / 8 = 1Mhz
    //CLKPR = (1<<CLKPS1) | (1<<CLKPS0);

    sbiBF(DDRD,5);  // set OC1A as output
    sbiBF(PORTD,5);  // set OC1A high
    Timer0_Init();
    Duration = 0;
    pSong=(int*)pgm_read_word(&Songs[WHICHSONG]);
    Tone = 1;
    Sound_Init();
    return 0;
}


/*****************************************************************************
*
*   Function name : Play_Tune
*
*   Returns :       None
*
*   Parameters :    None
*
*   Purpose :       Plays the song
*
*****************************************************************************/
void Play_Tune(void)
{
    unsigned int temp_tone;	// mt 200301
    int temp_hi;
    
    char loop;
    
    if(!Tone)
    {
        Duration = 0;   
        // mt Tempo = *(pSong + 0);
        Tempo = (uint8_t)pgm_read_word(pSong + 0);
        Tone = 1;   //Start the song from the beginning
    }
    
    if(!Tempo)
    {
        if(Duration)        // Check if the lenght of the tone has "expired"
        {   
            Duration--;
        }
        // mt: else if(*(pSong + Tone))    // If not the end of the song
		else if(pgm_read_word(pSong + Tone))  // If not the end of the song
        {
            // mt: Duration = ( DURATION_SEED / *(pSong + Tone) );  // store the duration
            Duration = ( DURATION_SEED / pgm_read_word(pSong + Tone) );  // store the duration
        
			Tone++;                     // point to the next tone in the Song-table        

			temp_tone=pgm_read_word(pSong + Tone); // mt 200301
			// mt: if( (*(pSong + Tone) == p) | (*(pSong + Tone) == P) ) // if pause
			// if( (pgm_read_word(pSong + Tone) == p) | (pgm_read_word(pSong + Tone) == P) ) // if pause
            if( (temp_tone == p) | (temp_tone == P) ) // if pause
                cbiBF(TCCR1B, CS10);             // stop Timer1, prescaler(1)    
            else 
                sbiBF(TCCR1B, CS10);             // start Timer1, prescaler(1)  
                
            cli(); // mt __disable_interrupt();
            
            // mt temp_hi = *(pSong + Tone);      // read out the PWM-value
			// temp_hi = pgm_read_word(pSong + Tone);      // read out the PWM-value
			temp_hi = temp_tone;			// mt 200301
            temp_hi >>= 8;                  // move integer 8 bits to the rigth
                
            TCNT1H = 0;                     // reset TCNT1H/L
            TCNT1L = 0;
            
            ICR1H = temp_hi;                // load ICR1H/L
            // mt: ICR1L = *(pSong + Tone);        
			// ICR1L = pgm_read_word(pSong + Tone);
			ICR1L = temp_tone;
            
            sei(); // mt: __enable_interrupt();
            
            Tone++;                     // point to the next tone in the Song-table
        }
        else    // the end of song
        {
            Tone++;         // point to the next tone in the Song-table        
            
            // mt: loop = *(pSong + Tone); // get the byte that tells if the song should loop or not
			loop = (uint8_t)pgm_read_word(pSong + Tone); // get the byte that tells if the song should loop or not
            
            if( loop )  
            {
                Tone = 1;
            }
            else        // if not looping the song
            {
                Tone = 0;
                //gPlaying = FALSE;
                cbiBF(TCCR1B, 0);                     // stop Playing
                Timer0_RemoveCallbackFunction(Play_Tune);
                TCCR1A = 0;
                TCCR1B = 0;
                sbiBF(PORTB, 5);              // set OC1A high
            }
        }
        
        // mt: Tempo = *(pSong + 0);
		Tempo = (uint8_t)pgm_read_word(pSong + 0);
    }
    else
        Tempo--;
 
}    
