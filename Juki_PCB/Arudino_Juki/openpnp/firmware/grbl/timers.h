
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

unsigned long avr_millis();
void setup_timer( void );
void avr_delay(unsigned long ms);
