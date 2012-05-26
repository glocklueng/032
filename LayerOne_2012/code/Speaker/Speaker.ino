void loop_scroll(void);
void setup_scroll(void);
void Clear(unsigned short val);

////////////////////////////

#if !defined(digitalPinToPortReg)
#if !(defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) )


// Standard Arduino Pins
#define digitalPinToPortReg(P) \
(((P) >= 0 && (P) <= 7) ? &PORTD : (((P) >= 8 && (P) <= 13) ? &PORTB : &PORTC))
#define digitalPinToDDRReg(P) \
(((P) >= 0 && (P) <= 7) ? &DDRD : (((P) >= 8 && (P) <= 13) ? &DDRB : &DDRC))
#define digitalPinToPINReg(P) \
(((P) >= 0 && (P) <= 7) ? &PIND : (((P) >= 8 && (P) <= 13) ? &PINB : &PINC))
#define digitalPinToBit(P) \
(((P) >= 0 && (P) <= 7) ? (P) : (((P) >= 8 && (P) <= 13) ? (P) - 8 : (P) - 14))

#if defined(__AVR_ATmega8__)

// 3 PWM
#define digitalPinToTimer(P) \
(((P) ==  9 || (P) == 10) ? &TCCR1A : (((P) == 11) ? &TCCR2 : 0))
#define digitalPinToTimerBit(P) \
(((P) ==  9) ? COM1A1 : (((P) == 10) ? COM1B1 : COM21))
#else

// 6 PWM
#define digitalPinToTimer(P) \
(((P) ==  6 || (P) ==  5) ? &TCCR0A : \
        (((P) ==  9 || (P) == 10) ? &TCCR1A : \
        (((P) == 11 || (P) ==  3) ? &TCCR2A : 0)))
#define digitalPinToTimerBit(P) \
(((P) ==  6) ? COM0A1 : (((P) ==  5) ? COM0B1 : \
        (((P) ==  9) ? COM1A1 : (((P) == 10) ? COM1B1 : \
        (((P) == 11) ? COM2A1 : COM2B1)))))
#endif

#else
// Arduino Mega Pins
#define digitalPinToPortReg(P) \
(((P) >= 22 && (P) <= 29) ? &PORTA : \
        ((((P) >= 10 && (P) <= 13) || ((P) >= 50 && (P) <= 53)) ? &PORTB : \
        (((P) >= 30 && (P) <= 37) ? &PORTC : \
        ((((P) >= 18 && (P) <= 21) || (P) == 38) ? &PORTD : \
        ((((P) >= 0 && (P) <= 3) || (P) == 5) ? &PORTE : \
        (((P) >= 54 && (P) <= 61) ? &PORTF : \
        ((((P) >= 39 && (P) <= 41) || (P) == 4) ? &PORTG : \
        ((((P) >= 6 && (P) <= 9) || (P) == 16 || (P) == 17) ? &PORTH : \
        (((P) == 14 || (P) == 15) ? &PORTJ : \
        (((P) >= 62 && (P) <= 69) ? &PORTK : &PORTL))))))))))

#define digitalPinToDDRReg(P) \
(((P) >= 22 && (P) <= 29) ? &DDRA : \
        ((((P) >= 10 && (P) <= 13) || ((P) >= 50 && (P) <= 53)) ? &DDRB : \
        (((P) >= 30 && (P) <= 37) ? &DDRC : \
        ((((P) >= 18 && (P) <= 21) || (P) == 38) ? &DDRD : \
        ((((P) >= 0 && (P) <= 3) || (P) == 5) ? &DDRE : \
        (((P) >= 54 && (P) <= 61) ? &DDRF : \
        ((((P) >= 39 && (P) <= 41) || (P) == 4) ? &DDRG : \
        ((((P) >= 6 && (P) <= 9) || (P) == 16 || (P) == 17) ? &DDRH : \
        (((P) == 14 || (P) == 15) ? &DDRJ : \
        (((P) >= 62 && (P) <= 69) ? &DDRK : &DDRL))))))))))

#define digitalPinToPINReg(P) \
(((P) >= 22 && (P) <= 29) ? &PINA : \
        ((((P) >= 10 && (P) <= 13) || ((P) >= 50 && (P) <= 53)) ? &PINB : \
        (((P) >= 30 && (P) <= 37) ? &PINC : \
        ((((P) >= 18 && (P) <= 21) || (P) == 38) ? &PIND : \
        ((((P) >= 0 && (P) <= 3) || (P) == 5) ? &PINE : \
        (((P) >= 54 && (P) <= 61) ? &PINF : \
        ((((P) >= 39 && (P) <= 41) || (P) == 4) ? &PING : \
        ((((P) >= 6 && (P) <= 9) || (P) == 16 || (P) == 17) ? &PINH : \
        (((P) == 14 || (P) == 15) ? &PINJ : \
        (((P) >= 62 && (P) <= 69) ? &PINK : &PINL))))))))))

#define digitalPinToBit(P) \
(((P) >=  7 && (P) <=  9) ? (P) - 3 : \
        (((P) >= 10 && (P) <= 13) ? (P) - 6 : \
        (((P) >= 22 && (P) <= 29) ? (P) - 22 : \
        (((P) >= 30 && (P) <= 37) ? 37 - (P) : \
        (((P) >= 39 && (P) <= 41) ? 41 - (P) : \
        (((P) >= 42 && (P) <= 49) ? 49 - (P) : \
        (((P) >= 50 && (P) <= 53) ? 53 - (P) : \
        (((P) >= 54 && (P) <= 61) ? (P) - 54 : \
        (((P) >= 62 && (P) <= 69) ? (P) - 62 : \
        (((P) == 0 || (P) == 15 || (P) == 17 || (P) == 21) ? 0 : \
        (((P) == 1 || (P) == 14 || (P) == 16 || (P) == 20) ? 1 : \
        (((P) == 19) ? 2 : \
        (((P) == 5 || (P) == 6 || (P) == 18) ? 3 : \
        (((P) == 2) ? 4 : \
        (((P) == 3 || (P) == 4) ? 5 : 7)))))))))))))))

// 15 PWM
#define digitalPinToTimer(P) \
(((P) == 13 || (P) ==  4) ? &TCCR0A : \
        (((P) == 11 || (P) == 12) ? &TCCR1A : \
        (((P) == 10 || (P) ==  9) ? &TCCR2A : \
        (((P) ==  5 || (P) ==  2 || (P) ==  3) ? &TCCR3A : \
        (((P) ==  6 || (P) ==  7 || (P) ==  8) ? &TCCR4A : \
        (((P) == 46 || (P) == 45 || (P) == 44) ? &TCCR5A : 0))))))
#define digitalPinToTimerBit(P) \
(((P) == 13) ? COM0A1 : (((P) ==  4) ? COM0B1 : \
        (((P) == 11) ? COM1A1 : (((P) == 12) ? COM1B1 : \
        (((P) == 10) ? COM2A1 : (((P) ==  9) ? COM2B1 : \
        (((P) ==  5) ? COM3A1 : (((P) ==  2) ? COM3B1 : (((P) ==  3) ? COM3C1 : \
        (((P) ==  6) ? COM4A1 : (((P) ==  7) ? COM4B1 : (((P) ==  8) ? COM4C1 : \
        (((P) == 46) ? COM5A1 : (((P) == 45) ? COM5B1 : COM5C1))))))))))))))

#endif
#endif

#if !defined(digitalWriteFast)
#define digitalWriteFast(P, V) \
if (__builtin_constant_p(P) && __builtin_constant_p(V)) { \
                if (digitalPinToTimer(P)) \
                        bitClear(*digitalPinToTimer(P), digitalPinToTimerBit(P)); \
                bitWrite(*digitalPinToPortReg(P), digitalPinToBit(P), (V)); \
        } else { \
                digitalWrite((P), (V)); \
        }
#endif

#if !defined(pinModeFast)
#define pinModeFast(P, V) \
if (__builtin_constant_p(P) && __builtin_constant_p(V)) { \
                bitWrite(*digitalPinToDDRReg(P), digitalPinToBit(P), (V)); \
        } else {  \
                pinMode((P), (V)); \
        } 
#endif

#if !defined(digitalReadFast)
#define digitalReadFast(P) ( (int) __digitalReadFast__((P)) )
#define __digitalReadFast__(P ) \
(__builtin_constant_p(P) ) ? ( \
                digitalPinToTimer(P) ? ( \
                       bitClear(*digitalPinToTimer(P), digitalPinToTimerBit(P)) ,  \
                             bitRead(*digitalPinToPINReg(P), digitalPinToBit(P))) : \
                  bitRead(*digitalPinToPINReg(P), digitalPinToBit(P)))  : \
                digitalRead((P))
#endif

#if !defined(digitalWriteFast2)
#define digitalWriteFast2(P, V) \
if (__builtin_constant_p(P) && __builtin_constant_p(V)) { \
                bitWrite(*digitalPinToPortReg(P), digitalPinToBit(P), (V)); \
        } else { \
                digitalWrite((P), (V)); \
        }
#endif

#if !defined(pinModeFast2)
#define pinModeFast2(P, V) \
if (__builtin_constant_p(P) && __builtin_constant_p(V)) { \
                if (digitalPinToTimer(P)) \
                        bitClear(*digitalPinToTimer(P), digitalPinToTimerBit(P)); \
                bitWrite(*digitalPinToDDRReg(P), digitalPinToBit(P), (V)); \
        } else {  \
                pinMode((P), (V)); \
        } 
#endif

#if !defined(digitalReadFast2)
#define digitalReadFast2(P) ( (int) __digitalReadFast2__((P)) )
#define __digitalReadFast2__(P ) \
(__builtin_constant_p(P) ) ? ( \
                ( bitRead(*digitalPinToPINReg(P), digitalPinToBit(P))) ) : \
                digitalRead((P))
#endif

//////////////////////////////////

/* Ports and Pins
 Direct port access is much faster than digitalWrite.
 You must match the correct port and pin as shown in the table below.
 
 Arduino Pin        Port        Pin Shield
 13 (SCK)           PORTB       5    8
 12 (MISO)          PORTB       4    9
 11 (MOSI)          PORTB       3   10
 10 (SS)            PORTB       2   11
 9                 PORTB        1   12
 8                 PORTB        0   13
 
 7                 PORTD       7  0
 6                 PORTD       6  1
 5                 PORTD       5  2
 4                 PORTD       4  3
 3                 PORTD       3  4
 2                 PORTD       2  5
 1 (TX)            PORTD       1  6
 0 (RX)            PORTD       0  7
 
 A5 (Analog)        PORTC       5
 A4 (Analog)        PORTC       4
 A3 (Analog)        PORTC       3
 A2 (Analog)        PORTC       2
 A1 (Analog)        PORTC       1
 A0 (Analog)        PORTC       0
 */

// Defines for use with Arduino functions
#define clockpin    2 // CL
#define enablepin   3 // BL
#define latchpin    4 // XL
#define datapin     1 // SI

// Defines for direct port access
#define CLKPORT PORTD
#define ENAPORT PORTD
#define LATPORT PORTD
#define DATPORT PORTD

// Defines for PIN bits  in atmega
#define CLKPIN  2
#define ENAPIN  3
#define LATPIN  4
#define DATPIN  1

#define TRANS4     10 // GND without mod!
#define TRANS3     13
#define TRANS2     12
#define TRANS1     11 

int gCount = 500;

// Number of OctoBrites / TLC5947 devices
#define NumOctoBrites  ( 1 )

// Array storing color values
//  GREY: LEDChannels[x][0]   Range: {0 to 4095}
uint16_t LEDChannels[NumOctoBrites*24] = {0};

uint16_t FrameBuffer[NumOctoBrites*24][9] = {0};

// Variables for sample function
float offset = 0;


// Set pins to outputs and initial states
void setup() {
  
  
  for ( int i = 0 ; i < 13 ;i ++ ) {
   // pinMode(i, INPUT);     
   // digitalWrite(i, HIGH);
  }
  
  // transistors
  pinMode(TRANS1, OUTPUT);
  pinMode(TRANS2, OUTPUT);
  pinMode(TRANS3, OUTPUT);
  pinMode(TRANS4, OUTPUT);

  
  
  // TLC Chip
  pinMode(datapin, OUTPUT);
  pinMode(latchpin, OUTPUT);
  pinMode(enablepin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  
  digitalWrite(latchpin, LOW);
  digitalWrite(enablepin, LOW);
  
  setup_scroll();
  
}

// Read all bits in the LEDChannels array and send them on the selected pins
void WriteLEDArray() {

  static unsigned int tempOne = 0;

  for (int i = 0; i < (NumOctoBrites * 16); i++) {

    tempOne = LEDChannels[i];

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
  LATPORT &= ~(1 << LATPIN);
}
// Read all bits in the LEDChannels array and send them on the selected pins
void WriteFrameBuffer() {

  static unsigned int tempOne = 0;

  for (int j = 0; j < (9); j++) {
    for (int i = 0; i < (NumOctoBrites * 16); i++) {
  
    rowSelect(j);

      tempOne = FrameBuffer[i][j];
  
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
    LATPORT &= ~(1 << LATPIN);
    
  }
}

// Sample function to draw a scanning pattern with fading
void LEDscan(int red, float degreeoffset) {

  float brightnessfactor = 0;
  
  float scanindex = (1.0f + sin(degreeoffset*3.14159f/180.0f)) * ((float)(NumOctoBrites * 16) / 2.0f);
  
    for(int LEDindex = 0; LEDindex < (NumOctoBrites * 16); LEDindex++) {
      
      brightnessfactor = exp(0.0f - fabs(scanindex - ((float)LEDindex + 0.5f)) * 1.3f);
      
      LEDChannels[LEDindex] = brightnessfactor *= red ;  
    }
    
    WriteLEDArray();  
}

void rowSelect( unsigned char row )
{
  digitalWriteFast(TRANS1, (row&1));
  digitalWriteFast(TRANS2, (row>>1)&1);  
  digitalWriteFast(TRANS3, (row>>2)&1);
  digitalWriteFast(TRANS4, (row>>3)&1);
}

void loop() {

  static unsigned char row = 0;
  
 // Clear(0);
 // loop_scroll();
  memset(FrameBuffer,0xaa,sizeof(FrameBuffer)*sizeof(uint16_t));
  WriteFrameBuffer();

  return;
  
  
  for (offset = 0; offset < 360; offset += 1) {

    LEDscan(4095, offset);
    
    for(int i = 0 ; i < 9 ; i ++ ) {
      // wrap the row
      for(int j =0 ; j < 14+i ; j++) 
        rowSelect(i);
    }
  }
}

#define SUPERBALL_WIDTH  ( 16 )
#define SUPERBALL_HEIGHT (  9 )

void SetPoint( unsigned short x, unsigned short y,unsigned short val)
{
	FrameBuffer[ (  x * SUPERBALL_WIDTH ) ][ ( SUPERBALL_HEIGHT - y )] = val;
}

void Clear(unsigned short val)
{
    for (int j = 0; j < (9); j++) {
      for(unsigned short LEDindex = 0; LEDindex < (1  * 16); LEDindex++) {
        FrameBuffer[LEDindex][j] = val;
      }
    }
}


#define USE_SCROLLER 1
#define USE_FONT  1
/*
                                                                                  
										 ad88888ba                                        88  88                          
										d8"     "8b                                       88  88                          
										Y8,                                               88  88                          
										`Y8aaaaa,     ,adPPYba,  8b,dPPYba,   ,adPPYba,   88  88   ,adPPYba,  8b,dPPYba,  
										  `"""""8b,  a8"     ""  88P'   "Y8  a8"     "8a  88  88  a8P_____88  88P'   "Y8  
										        `8b  8b          88          8b       d8  88  88  8PP"""""""  88          
										Y8a     a8P  "8a,   ,aa  88          "8a,   ,a8"  88  88  "8b,   ,aa  88          
										 "Y88888P"    `"Ybbd8"'  88           `"YbbdP"'   88  88   `"Ybbd8"'  88          
*/


/* 
 * 8x6 font
 */

static const unsigned char PROGMEM font8x6[354] = {
	0x00,0x00,0x00,0x00,0x00,0x00, 0x30,0x30,0x30,0x00,0x30,0x00,
	0xd8,0x90,0x00,0x00,0x00,0x00, 0x6c,0xfe,0x6c,0xfe,0x6c,0x00,
	0x7e,0xd0,0x7c,0x16,0xfc,0x00, 0xcc,0x18,0x30,0x60,0xcc,0x00,
	0x60,0x90,0x74,0xd8,0x6c,0x00, 0x30,0x20,0x00,0x00,0x00,0x00,
	0x60,0xc0,0xc0,0xc0,0x60,0x00, 0x18,0x0c,0x0c,0x0c,0x18,0x00,
	0x6c,0x38,0xfe,0x38,0x6c,0x00, 0x00,0x30,0xfc,0x30,0x00,0x00,
	0x00,0x00,0x00,0x30,0x20,0x00, 0x00,0x00,0x7c,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xc0,0x00, 0x0c,0x18,0x30,0x60,0xc0,0x00,
	0x78,0x84,0x84,0x84,0x78,0x00, 0x04,0x0c,0x14,0x04,0x04,0x00,
	0x78,0x84,0x18,0x20,0x7c,0x00, 0x78,0x04,0x38,0x04,0x78,0x00,
	0x84,0x84,0xfc,0x04,0x04,0x00, 0xfc,0x80,0xf8,0x04,0xf8,0x00,
	0x30,0x40,0xf8,0x84,0x78,0x00, 0xfc,0x04,0x08,0x10,0x10,0x00,
	0x78,0x84,0x78,0x84,0x78,0x00, 0x78,0x84,0x7c,0x08,0x30,0x00,
	0x00,0x30,0x00,0x30,0x00,0x00, 0x00,0x30,0x00,0x30,0x20,0x00,
	0x30,0x60,0xc0,0x60,0x30,0x00, 0x00,0x7c,0x00,0x7c,0x00,0x00,
	0x30,0x18,0x0c,0x18,0x30,0x00, 0x78,0xcc,0x18,0x00,0x18,0x00,
	0x78,0xcc,0xdc,0xc0,0x7c,0x00, 0x30,0x48,0x84,0xfc,0x84,0x00,
	0xf8,0x84,0xf8,0x84,0xf8,0x00, 0x7c,0x80,0x80,0x80,0x7c,0x00,
	0xf8,0x84,0x84,0x84,0xf8,0x00, 0xfc,0x80,0xf0,0x80,0xfc,0x00,
	0xfc,0x80,0xf0,0x80,0x80,0x00, 0x7c,0x80,0x84,0x84,0x7c,0x00,
	0x84,0x84,0xfc,0x84,0x84,0x00, 0x10,0x10,0x10,0x10,0x10,0x00,
	0x04,0x04,0x04,0x84,0x78,0x00, 0x84,0x88,0xf0,0x88,0x84,0x00,
	0x80,0x80,0x80,0x80,0xfc,0x00, 0x82,0xc6,0xaa,0x92,0x82,0x00,
	0x84,0xc4,0xa4,0x94,0x8c,0x00, 0x78,0x84,0x84,0x84,0x78,0x00,
	0xf8,0x84,0xf8,0x80,0x80,0x00, 0x78,0x84,0x84,0x78,0x1c,0x00,
	0xf8,0x84,0xf8,0x90,0x88,0x00, 0x7c,0x80,0x78,0x04,0xf8,0x00,
	0xfe,0x10,0x10,0x10,0x10,0x00, 0x84,0x84,0x84,0x84,0x78,0x00,
	0x84,0x84,0x84,0x48,0x30,0x00, 0x82,0x82,0x92,0xaa,0x44,0x00,
	0x84,0x48,0x30,0x48,0x84,0x00, 0x88,0x88,0x50,0x20,0x20,0x00,
	0xf8,0x10,0x20,0x40,0xf8,0x00
};

/*** The 6x6 hard coded font
 * *
 **/
const unsigned char PROGMEM font6x6[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x80,0x00,0xA0,0xA0,0x00,0x00,
	0x00,0x00,0x50,0xF8,0x50,0xF8,0x50,0x00,0x70,0xA0,0x70,0x28,0x70,0x00,0xC8,0xD0,
	0x20,0x58,0x98,0x00,0x30,0x50,0x60,0x70,0x98,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,
	0x40,0x80,0x80,0x80,0x40,0x00,0x80,0x40,0x40,0x40,0x80,0x00,0x50,0x20,0xF8,0x20,
	0x50,0x00,0x20,0x20,0xF8,0x20,0x20,0x00,0x00,0x00,0x00,0x40,0x40,0x80,0x00,0x00,
	0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x08,0x10,0x20,0x40,0x80,0x00,
	0x60,0x90,0x90,0x90,0x60,0x00,0x40,0xC0,0x40,0x40,0xE0,0x00,0xE0,0x10,0x60,0x80,
	0xF0,0x00,0xE0,0x10,0x60,0x10,0xE0,0x00,0xA0,0xA0,0xE0,0x20,0x20,0x00,0xF0,0x80,
	0xF0,0x10,0xF0,0x00,0xF0,0x80,0xF0,0x90,0xF0,0x00,0xF0,0x10,0x20,0x40,0x40,0x00,
	0xF0,0x90,0xF0,0x90,0xF0,0x00,0xF0,0x90,0xF0,0x10,0x10,0x00,0x00,0x80,0x00,0x80,
	0x00,0x00,0x00,0x40,0x00,0x40,0x40,0x80,0x20,0x40,0x80,0x40,0x20,0x00,0x00,0xF0,
	0x00,0xF0,0x00,0x00,0x80,0x40,0x20,0x40,0x80,0x00,0xE0,0x20,0x60,0x00,0x40,0x00,
	0x70,0x88,0xB8,0xB8,0x80,0x78,0x60,0x90,0xF0,0x90,0x90,0x00,0xE0,0x90,0xE0,0x90,
	0xE0,0x00,0x70,0x80,0x80,0x80,0x70,0x00,0xE0,0x90,0x90,0x90,0xE0,0x00,0xF0,0x80,
	0xE0,0x80,0xF0,0x00,0xF0,0x80,0xE0,0x80,0x80,0x00,0xF0,0x80,0xB0,0x90,0xF0,0x00,
	0x90,0x90,0xF0,0x90,0x90,0x00,0xE0,0x40,0x40,0x40,0xE0,0x00,0x10,0x10,0x10,0x90,
	0x60,0x00,0x90,0xA0,0xC0,0xA0,0x90,0x00,0x80,0x80,0x80,0x80,0xF0,0x00,0x88,0xD8,
	0xA8,0x88,0x88,0x00,0x90,0xD0,0xB0,0x90,0x90,0x00,0x60,0x90,0x90,0x90,0x60,0x00,
	0xE0,0x90,0xE0,0x80,0x80,0x00,0x60,0x90,0x90,0xA0,0x50,0x00,0xE0,0x90,0xE0,0x90,
	0x90,0x00,0x70,0x80,0x60,0x10,0xE0,0x00,0xF8,0x20,0x20,0x20,0x20,0x00,0x90,0x90,
	0x90,0x90,0x60,0x00,0x90,0x90,0x90,0x60,0x60,0x00,0x88,0x88,0xA8,0xD8,0x88,0x00,
	0x90,0x90,0x60,0x90,0x90,0x00,0x88,0x88,0x50,0x20,0x20,0x00,0xF0,0x20,0x40,0x80,
	0xF0,0x00,0xC0,0x80,0x80,0x80,0xC0,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0xC0,0x40,
	0x40,0x40,0xC0,0x00,0x20,0x50,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x00,
	0x40,0xA0,0x40,0x00,0x00,0x00,0x00,0x70,0x90,0x90,0x70,0x00,0x80,0xE0,0x90,0x90,
	0xE0,0x00,0x00,0x70,0x80,0x80,0x70,0x00,0x10,0x70,0x90,0x90,0x70,0x00,0x00,0x60,
	0xB0,0xC0,0x70,0x00,0x60,0x40,0xE0,0x40,0x40,0x00,0x00,0x70,0x90,0x70,0x10,0xE0,
	0x80,0xE0,0x90,0x90,0x90,0x00,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x00,0x20,0x20,
	0xA0,0x40,0x80,0x80,0xA0,0xC0,0xA0,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0xD0,
	0xA8,0xA8,0x88,0x00,0x00,0xE0,0x90,0x90,0x90,0x00,0x00,0x60,0x90,0x90,0x60,0x00,
	0x00,0xE0,0x90,0x90,0xE0,0x80,0x00,0x70,0x90,0x90,0x70,0x10,0x00,0xA0,0xD0,0x80,
	0x80,0x00,0x00,0x70,0xC0,0x30,0xE0,0x00,0x40,0xE0,0x40,0x40,0x40,0x00,0x00,0x90,
	0x90,0x90,0x60,0x00,0x00,0x90,0x90,0xA0,0x40,0x00,0x00,0x88,0xA8,0xA8,0xD8,0x00,
	0x00,0x90,0x60,0x60,0x90,0x00,0x00,0x90,0x90,0x70,0x10,0xE0,0x00,0xF0,0x20,0x40,
	0xF0,0x00,0x40,0x80,0x40,0x80,0x40,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x40,
	0x80,0x40,0x80,0x00,0x00,0x50,0xA0,0x00,0x00,0x00,0x20,0x50,0x88,0x88,0xF8,0x00,
	0x04,0x02,0x04,0x06,0x06,0x06,0x06,0x02,0x03,0x03,0x06,0x06,0x03,0x05,0x02,0x06,
	0x05,0x04,0x05,0x05,0x04,0x05,0x05,0x05,0x05,0x05,0x02,0x03,0x04,0x05,0x04,0x04,
	0x06,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x04,0x05,0x05,0x05,0x06,0x05,0x05,
	0x05,0x05,0x05,0x05,0x06,0x05,0x05,0x06,0x05,0x06,0x05,0x03,0x06,0x03,0x06,0x07,
	0x04,0x05,0x05,0x05,0x05,0x05,0x04,0x05,0x05,0x02,0x04,0x04,0x02,0x06,0x05,0x05,
	0x05,0x05,0x05,0x05,0x04,0x05,0x05,0x06,0x05,0x05,0x05,0x03,0x02,0x03,0x05,0x06
};



/** 
 * Get the length of a \0 terminated string in progmem
 *
 * @param str   - ASCIZ string
 * @return      - length
 *
 * 
 */
unsigned short pstrlen(const unsigned char * const str)
{
    const unsigned char *s;
    for (s = str; pgm_read_byte(s); ++s);
    return(s - str);
}


/** 
 * Draw an 8 bit string using the builtin 8x6 font (this font is always available) PGMCODE!
 *
 * @param x     - X position on screen, will be clipped.
 * @param y     - Y position on screen, will be clipped.
 * @param string- 8 bit ASCIZ string
 *
 * Note: font and string are in progmem!  x == -999, will center X
 */
void Text8x6(short x,short y,const unsigned char *string)
{
	short nm,i,j,oldx=x;
	unsigned char  cur;
	int l_adr;
	int l=0;

	if( x == -999 ) {
		// center it
		x =  ( (SUPERBALL_WIDTH/2)-((pstrlen(string) * 7)/2) );
	}
	

	// off right side
	if( x >= SUPERBALL_WIDTH ) return;

	//completely off top edge
	if( y < -7  ) return;
	
	// off bottom edge
	if( y >= SUPERBALL_HEIGHT ) return;

	while(pgm_read_byte(string)!='\0')											/* until the end of the string */
	{
		if(pgm_read_byte(string)=='\n'){
			x=oldx;y+=7;
			l=0;
			string++;
		}
		
		nm=((pgm_read_byte(string++))-' ');									/* Starting from space */
		
		if( nm > 58 ) {
			nm-=' ';
		}																		/* to uppercase 58='Z'-' ' */

		nm *= 6;																/* 6 bytes per char */

		for(i=0,l_adr=0;i<6;i++,l_adr++)
		{
			cur=pgm_read_byte(&font8x6[nm++]);

			for(j=0;j<8;j++) {
				if(cur&(0x80>>j)) {
					if (( x+j+l) >= 0 && (x+j+l) < (SUPERBALL_WIDTH))
						if( (y+i) >= 0 && (y+i) < SUPERBALL_HEIGHT-1 )
							SetPoint(x+j+l,y+i, 4095>>1);
				}
			}
		}
              
		l+=7;/* Next word */
	}
}


/** 
 * Draw an 8 bit string using the builtin 6x6 font PGMCODE
 *
 * @param x     - X position on screen, will be clipped.
 * @param y     - Y position on screen, will be clipped.
 * @param string- 8 bit ASCIZ string

 */
void Text6x6(short x,short y,const unsigned char *string)
{
	short nm,i,j,oldx=x;
	unsigned char  cur;
	int l = 0;
	
	if( x == -999 ) {
		// center it
		x =  ( (SUPERBALL_WIDTH/2)-((pstrlen(string) * 6)/2) );
	}
	
	while(pgm_read_byte(string)!='\0')										/* until the end of the string */
	{
		
		if(pgm_read_byte(string)=='\n') {
			x=oldx;
			y+=6;

			string++;
		} else {
			
			nm=((pgm_read_byte(string++))-' ');                     /* Starting from space */
			
			if(nm>58) nm-=' ';                        /* to uppercase 58='Z'-' ' */
			nm*=6;                                    /* 6 bytes per char */
			for(i=0;i<6;i++)
			{
				cur=pgm_read_byte(&font6x6[nm++]);
				for(j=0;j<6;j++) {
					if(cur&(0x80>>j)) {
					if (( x+j+l) >= 0 && (x+j+l) < (SUPERBALL_WIDTH))
						if( (y+i) >= 0 && (y+i) < SUPERBALL_HEIGHT-1 )
							SetPoint(x+j+l,y+i , 4095>>1);

					}
				}
			}
			l+=5;                                   /* Next word */
		}
	}
}


static short textXPos[3] , textLength[3];

static const PROGMEM unsigned char text[] =  "NULLSPACE LABS 032.LA ";

void setup_scroll(void)
{
	// Start off edge of display
	textXPos[0] = 1;

	Clear(0);

	// pixel text length + a bit
	textLength[0] = ( pstrlen( text  ) * 9 ) + 18;

	// how long to run for (frames)
	gCount = 500;
}

void loop_scroll(void)
{
	unsigned char i;

	Text8x6(textXPos[0], 2,text);

	for ( i = 0 ;i  < 3;  i++ ){

		textXPos[i] -= (i+1) ;

		if ( textXPos[i] < -textLength[i]  )  {
			textXPos[i] = 2;
		}
	}	
}



