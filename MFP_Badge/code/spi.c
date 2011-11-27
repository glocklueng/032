#include "optiloader.h"
#include "spi.h"

void SPI_attachInterrupt() {
  SPCR |= _BV(SPIE);
}

void SPI_detachInterrupt() {
  SPCR &= ~_BV(SPIE);
}

void SPI_setClockDivider(uint8_t rate)
{
  SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
  SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((rate >> 2) & SPI_2XCLOCK_MASK);
}

void SPI_begin( void )
{
	// setup spi mode
	pinMode(SCK, OUTPUT);
	pinMode(MOSI, OUTPUT);
	pinMode(SS, OUTPUT);

	digitalWrite(SCK, LOW);
	digitalWrite(MOSI, LOW);
	digitalWrite(SS, HIGH);

	// Warning: if the SS pin ever becomes a LOW INPUT then SPI 
	// automatically switches to Slave, so the data direction of 
	// the SS pin MUST be kept as OUTPUT.
	SPCR |= _BV(MSTR);
	SPCR |= _BV(SPE);
}

byte SPI_transfer(byte _data)
{
  SPDR = _data;
  while (!(SPSR & _BV(SPIF)))
    ;
  return SPDR;
}

