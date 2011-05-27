#include <avr/io.h>
#include <inttypes.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include "timer0.h"

TIMER_CALLBACK_FUNC CallbackFunc[TIMER0_NUM_CALLBACKS];

#define NULL 0
#define FALSE   0
#define TRUE    (!FALSE)

// Value definition:
// 0      The timer has expired
// 1-254  The timer is counting down
// 255    Free timer

// mt char CountDownTimers[TIMER0_NUM_COUNTDOWNTIMERS];
uint8_t CountDownTimers[TIMER0_NUM_COUNTDOWNTIMERS];

/*****************************************************************************
*
*   Function name : Timer0_Init
*
*   Returns :       None
*
*   Parameters :    None
*
*   Purpose :       Initialize Timer/Counter 0
*
*****************************************************************************/
void Timer0_Init(void)
{
    //mt char i;
	uint8_t i;

    // Initialize array of callback functions
    for (i=0; i<TIMER0_NUM_CALLBACKS; i++)
        CallbackFunc[i] = NULL;

    // Initialize countdown timers
    for (i=0; i<TIMER0_NUM_COUNTDOWNTIMERS; i++)
        CountDownTimers[i] = 255;


	// Initialize Timer0.
	// Used to give the correct time-delays in the song

    // Enable timer0 compare interrupt
	TIMSK = (1<<OCIE0);

    // Sets the compare value
	OCR0 = 38;

	// Set Clear on Timer Compare (CTC) mode, CLK/256 prescaler
	TCCR0 = (1<<WGM01)|(0<<WGM00)|(4<<CS00);
}



/*****************************************************************************
*
*   Function name : TIMER0_COMP_interrupt
*
*   Returns :       None
*
*   Parameters :    None
*
*   Purpose :       Check if any functions are to be called
*
*****************************************************************************/
// mtA
// #pragma vector = TIMER0_COMP_vect
// __interrupt void TIMER0_COMP_interrupt(void)
//SIGNAL(SIG_OUTPUT_COMPARE0)
ISR(TIMER0_COMP_vect)
// mtE
{
    // mt char i;
	uint8_t i;
    
    for (i=0; i<TIMER0_NUM_CALLBACKS; i++)
        if (CallbackFunc[i] != NULL)
            CallbackFunc[i]();
    
    // Count down timers
    for (i=0; i<TIMER0_NUM_COUNTDOWNTIMERS; i++)
        if (CountDownTimers[i] != 255 && CountDownTimers[i] != 0)
            CountDownTimers[i]--;

}


/*****************************************************************************
*
*   Function name : Timer0_RegisterCallbackFunction
*
*   Returns :       None
*
*   Parameters :    pFunc
*
*   Purpose :       Set up functions to be called from the 
*                   TIMER0_COMP_interrupt
*
*****************************************************************************/
char Timer0_RegisterCallbackFunction(TIMER_CALLBACK_FUNC pFunc)
{
    // mt char i;
	uint8_t i;
    
    for (i=0; i<TIMER0_NUM_CALLBACKS; i++)
    {
        if (CallbackFunc[i] == pFunc)
            return TRUE;
    }
    
    for (i=0; i<TIMER0_NUM_CALLBACKS; i++)
    {
        if (CallbackFunc[i] == NULL)
        {
            CallbackFunc[i] = pFunc;
            return TRUE;
        }   
    }
    
    return FALSE;
}


/*****************************************************************************
*
*   Function name : Timer0_RemoveCallbackFunction
*
*   Returns :       None
*
*   Parameters :    pFunc
*
*   Purpose :       Remove functions from the list which is called int the
*                   TIMER0_COMP_interrupt
*
*****************************************************************************/
char Timer0_RemoveCallbackFunction(TIMER_CALLBACK_FUNC pFunc)
{
    // mt char i;
	uint8_t i;
    
    for (i=0; i<TIMER0_NUM_CALLBACKS; i++)
    {
        if (CallbackFunc[i] == pFunc)
        {
            CallbackFunc[i] = NULL;
            return TRUE;
        }
    }
        
    return FALSE;
}


char Timer0_AllocateCountdownTimer()
{
    // mt char i;
	uint8_t i;
	
    for (i=0; i<TIMER0_NUM_COUNTDOWNTIMERS; i++)
        if (CountDownTimers[i] == 255)
        {
            CountDownTimers[i] = 0;
            return i+1;
        }

    return 0;
}

void Timer0_SetCountdownTimer(char timer, char value)
{
    cli(); // mt __disable_interrupt();
    CountDownTimers[timer-1] = value;
    sei(); // mt __enable_interrupt();
}

char Timer0_GetCountdownTimer(char timer)
{
    char t;
    cli(); // mt __disable_interrupt();
    t = CountDownTimers[timer-1];
    sei(); // mt __enable_interrupt();
    
    return t;
}

void Timer0_ReleaseCountdownTimer(char timer)
{
    cli(); // mt __disable_interrupt();
    CountDownTimers[timer-1] = 255;
    sei(); // mt __enable_interrupt();
}

