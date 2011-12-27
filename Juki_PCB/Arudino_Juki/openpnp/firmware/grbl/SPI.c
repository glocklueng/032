/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "SPI.h"
/*
	// setup vacuum sensor TAC
	LIMIT_DDR &= ~(_BV( TACSENSE ) );
	LIMIT_PORT |= (_BV( TACSENSE ) );

	// Vacuum on/off control
	HEAD_DDR  |= _BV( VACUUM );
	HEAD_PORT |= _BV( VACUUM );
*/

#define SET_OUTPUT( ddr, bit ) ddr &= ~(  1<< bit ) 
#define SET_INPUT( ddr, bit )  ddr |=  (  1<< bit ) 
#define SET_HIGH( port, bit )  port|=  (  1<< bit ) 
#define SET_LOW( port, bit )  port &= ~(  1<< bit )

#define LSBFIRST 0 

void SPI_begin() 
{
  // Set direction register for SCK and MOSI pin.
  // MISO pin automatically overrides to INPUT.
  // When the SS pin is set as OUTPUT, it can be used as
  // a general purpose output port (it doesn't influence
  // SPI operations).

  
 // pinMode( SCK, OUTPUT);
 // pinMode(MOSI, OUTPUT);
 // pinMode(  SS, OUTPUT);
  
 // digitalWrite(SCK, LOW);
 // digitalWrite(MOSI, LOW);
  //digitalWrite(SS, HIGH);

  // Warning: if the SS pin ever becomes a LOW INPUT then SPI 
  // automatically switches to Slave, so the data direction of 
  // the SS pin MUST be kept as OUTPUT.
  SPCR |= _BV(MSTR);
  SPCR |= _BV(SPE);
}

void SPI_end() {
  SPCR &= ~_BV(SPE);

byte SPIClass_transfer(byte _data) {
  SPDR = _data;
  while (!(SPSR & _BV(SPIF)))
    ;
  return SPDR;
}

void SPIClass_attachInterrupt() {
  SPCR |= _BV(SPIE);
}

void SPIClass_detachInterrupt() {
  SPCR &= ~_BV(SPIE);
}
}

void SPI_setBitOrder(uint8_t bitOrder)
{
  if(bitOrder == LSBFIRST) {
    SPCR |= _BV(DORD);
  } else {
    SPCR &= ~(_BV(DORD));
  }
}

void SPI_setDataMode(uint8_t mode)
{
  SPCR = (SPCR & ~SPI_MODE_MASK) | mode;
}

void SPI_setClockDivider(uint8_t rate)
{
  SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
  SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((rate >> 2) & SPI_2XCLOCK_MASK);
}

