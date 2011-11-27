#include "optiloader.h"

boolean target_poweroff (void);
boolean target_poweron (void);
void start_pmode (void);
void end_pmode (void);
void loop(void);
void setup (void);

void pinMode( unsigned char pin, unsigned char io)
{

}

void digitalWrite( unsigned char pin, unsigned char state  )
{

}

unsigned char digitalRead( unsigned char pin ) 
{

	switch( pin	)
	{
		case BUTTON:
			return PORTB&1;
	}
	return 0;	
}

void tone( unsigned int pin, uint16_t hz, uint16_t len)
{

}

void error(const char *string) { 
  digitalWrite(LED_ERR, HIGH);  
  while(1) {
    tone(PIEZOPIN, 4000, 500);
  }
}

int main(void)
{
	setup();
	loop();

	return 0;
}

// Standalone AVR ISP programmer
// August 2011 by Limor Fried / Ladyada / Adafruit
// Jan 2011 by Bill Westfield ("WestfW")
//
// this sketch allows an Arduino to program a flash program
// into any AVR if you can fit the HEX file into program memory
// No computer is necessary. Two LEDs for status notification
// Press button to program a new chip. Piezo beeper for error/success 
// This is ideal for very fast mass-programming of chips!
//
// It is based on AVRISP
//
// using the following pins:
// 10: slave reset
// 11: MOSI
// 12: MISO
// 13: SCK
//  9: 8 MHz clock output - connect this to the XTAL1 pin of the AVR
//     if you want to program a chip that requires a crystal without
//     soldering a crystal in
// ----------------------------------------------------------------------

// Variables
static int pmode=0;
static byte pageBuffer[128];		       /* One page of flash */




void setup (void)
{
  //Serial.begin(57600);			/* Initialize serial for status msgs */
  //Serial.println("\nAdaBootLoader Bootstrap programmer (originally OptiLoader Bill Westfield (WestfW))");

  pinMode(PIEZOPIN, OUTPUT);

  pinMode(LED_PROGMODE, OUTPUT);
  pulse(LED_PROGMODE,2);
  pinMode(LED_ERR, OUTPUT);
  pulse(LED_ERR, 2);

  pinMode(BUTTON, INPUT);     // button for next programming
  digitalWrite(BUTTON, HIGH); // pullup
  
  pinMode(CLOCK, OUTPUT);
  // setup high freq PWM on pin 9 (timer 1)
  // 50% duty cycle -> 8 MHz
  OCR1A = 0;
  ICR1 = 1;
  // OC1A output, fast PWM
  TCCR1A = _BV(WGM11) | _BV(COM1A1);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10); // no clock prescale
  
}

void loop (void) {
  //Serial.println("\nType 'G' or hit BUTTON for next chip");
  while (1) {
    if  ((! digitalRead(BUTTON)))
      break;  
  }
    
  target_poweron();			/* Turn on target power */

  uint16_t signature;
  image_t *targetimage;
        
  if (! (signature = readSignature()))		// Figure out what kind of CPU
    error("Signature fail");
  if (! (targetimage = findImage(signature)))	// look for an image
    error("Image fail");
  
  eraseChip();

  if (! programFuses(targetimage->image_progfuses))	// get fuses ready to program
    error("Programming Fuses fail");
  
  if (! verifyFuses(targetimage->image_progfuses, targetimage->fusemask) ) {
    error("Failed to verify fuses");
  } 

  end_pmode();
  start_pmode();

  byte *hextext = targetimage->image_hexcode;  
  uint16_t pageaddr = 0;
  uint8_t pagesize = pgm_read_byte(&targetimage->image_pagesize);
  uint16_t chipsize = pgm_read_word(&targetimage->chipsize);
        
  ////Serial.println(chipsize, DEC);
  while (pageaddr < chipsize) {
     byte *hextextpos = readImagePage (hextext, pageaddr, pagesize, pageBuffer);
          
     boolean blankpage = true;
     for (uint8_t i=0; i<pagesize; i++) {
       if (pageBuffer[i] != 0xFF) blankpage = false;
     }          
     if (! blankpage) {
       if (! flashPage(pageBuffer, pageaddr, pagesize))	
	 error("Flash programming failed");
     }
     hextext = hextextpos;
     pageaddr += pagesize;
  }
  
  // Set fuses to 'final' state
  if (! programFuses(targetimage->image_normfuses))
    error("Programming Fuses fail");
    
  end_pmode();
  start_pmode();
  
  //Serial.println("\nVerifing flash...");
  if (! verifyImage(targetimage->image_hexcode) ) {
    error("Failed to verify chip");
  } else {
    //Serial.println("\tFlash verified correctly!");
  }

  if (! verifyFuses(targetimage->image_normfuses, targetimage->fusemask) ) {
    error("Failed to verify fuses");
  } else {
    //Serial.println("Fuses verified correctly!");
  }
  target_poweroff();			/* turn power off */
  tone(PIEZOPIN, 4000, 200);
}



void start_pmode (void)
 {
  pinMode(13, INPUT); // restore to default

  SPI_begin();
  SPI_setClockDivider(SPI_CLOCK_DIV128); 
  
  debug("...spi_init done");
  // following delays may not work on all targets...
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, HIGH);
  pinMode(SCK, OUTPUT);
  digitalWrite(SCK, LOW);
  _delay_ms(50);
  digitalWrite(RESET, LOW);
  _delay_ms(50);
  pinMode(MISO, INPUT);
  pinMode(MOSI, OUTPUT);
  debug("...spi_transaction");
  spi_transaction(0xAC, 0x53, 0x00, 0x00);
  debug("...Done");
  pmode = 1;
}

void end_pmode (void)
{
  SPCR = 0;				/* reset SPI */
  digitalWrite(MISO, 0);		/* Make sure pullups are off too */
  pinMode(MISO, INPUT);
  digitalWrite(MOSI, 0);
  pinMode(MOSI, INPUT);
  digitalWrite(SCK, 0);
  pinMode(SCK, INPUT);
  digitalWrite(RESET, 0);
  pinMode(RESET, INPUT);
  pmode = 0;
}


/*
 * target_poweron
 * begin programming
 */
boolean target_poweron ()
{
  pinMode(LED_PROGMODE, OUTPUT);
  digitalWrite(LED_PROGMODE, HIGH);
  digitalWrite(RESET, LOW);  // reset it right away.
  pinMode(RESET, OUTPUT);
  _delay_ms(100);
  //Serial.print("Starting Program Mode");
  start_pmode();
  //Serial.println(" [OK]");
  return true;
}

boolean target_poweroff ()
{
  end_pmode();
  digitalWrite(LED_PROGMODE, LOW);
  return true;
}





