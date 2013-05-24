#include "leds.h"


// this is a nice sw spi and its been tested on the AVR with the cylon 4

static void swspi_init()
{
    SWSPI_DIR |= SWSPI_SS | SWSPI_SCK | SWSPI_MOSI;
    SWSPI_DIR &= ~SWSPI_MISO;

#if defined(USE_INTERRUPT)

    SWSPI_DIR &= ~SWSPI_IRQ;
    EICRA = ( ( 1<<ISC01 ) | ( 1<<ISC00) ); // The rising edge of INT0 generates asynchronously an interrupt request.
    EIMSK = 1<<INT0; // Enable INT0.

#endif

}

static uint8_t swspi_shift( uint8_t data )
{
    uint8_t i;
    for ( i = 0; i < 8; ++i ) {
        SWSPI_PORT &= ~SWSPI_SCK;
        if ( data & 0x80 ) {
            SWSPI_PORT |= SWSPI_MOSI;
        } else {
            SWSPI_PORT &= ~SWSPI_MOSI;
        }
        data <<= 1;
        if ( SWSPI_IN & SWSPI_MISO ) {
            data |= 1;
        }
        SWSPI_PORT |= SWSPI_SCK;
    }
    SWSPI_PORT &= ~SWSPI_SCK;
    SWSPI_PORT |= SWSPI_MOSI;
    return data; // Takes about 8 us to 9 us.
}

void SWSPI_init()
{
	swspi_init();

    SWSPI_PORT &= ~SWSPI_SCK;
    SWSPI_PORT |= SWSPI_MOSI;
    SWSPI_PORT |= SWSPI_SS;
}

void SWSPI_write( uint8_t address, uint8_t value )
{
    SWSPI_PORT &= ~SWSPI_SS;
    swspi_shift( 0x80 | ( address << 1 ) );
    swspi_shift( value );
    SWSPI_PORT |= SWSPI_SS;
}

uint8_t SWSPI_read( uint8_t address )
{
    uint8_t value;
    SWSPI_PORT &= ~SWSPI_SS;
    swspi_shift( address << 1 );
    value = swspi_shift( 0 );
    SWSPI_PORT |= SWSPI_SS;
    return value;
}
