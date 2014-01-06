/// SSD1306 driver

#include <stdlib.h>
#include <string.h>

#include "stm32f10x_lib.h"

#include "hw_config.h"
#include "l1_board.h"

#define GDISP_SCREEN_HEIGHT		(64)
#define GDISP_SCREEN_WIDTH		(128)

#ifndef GDISP_INITIAL_CONTRAST
	#define GDISP_INITIAL_CONTRAST	100
#endif
#ifndef GDISP_INITIAL_BACKLIGHT
	#define GDISP_INITIAL_BACKLIGHT	100
#endif

#ifdef SSD1306_PAGE_PREFIX
	#define SSD1306_PAGE_WIDTH		(GDISP_SCREEN_WIDTH+1)
	#define SSD1306_PAGE_OFFSET		1
#else
	#define SSD1306_PAGE_WIDTH		GDISP_SCREEN_WIDTH
	#define SSD1306_PAGE_OFFSET		0
#endif

// Buffer for OLED
static unsigned char sVideoRAM[GDISP_SCREEN_HEIGHT/8 * SSD1306_PAGE_WIDTH ];

#define xyaddr(x, y)			(SSD1306_PAGE_OFFSET + (x) + ((y)>>3)*SSD1306_PAGE_WIDTH)
#define xybit(y)				(1<<((y)&7))

#define	LCD_SCLK_HIGH() 		GPIO_WriteBit(OLED2_PORT,SCLK_DB0_PIN,Bit_SET)
#define	LCD_SCLK_LOW() 			GPIO_WriteBit(OLED2_PORT,SCLK_DB0_PIN,Bit_RESET)

#define	LCD_SDIN_HIGH() 		GPIO_WriteBit(OLED2_PORT,SDIN_DB1_PIN,Bit_SET)
#define	LCD_SDIN_LOW() 			GPIO_WriteBit(OLED2_PORT,SDIN_DB1_PIN,Bit_RESET)




// Cursor
static unsigned char    u8CursorX, u8CursorY;
static unsigned char	Misc = 0;

#define				NEGATIVE		( 1 )


// System 3x6 (char #22 to #96) 
static const unsigned char Fonts[] = {
    0x30,0x0C,0x02, // long /               // 0x16
    0x38,0x20,0x38, // u                    // 0x17
    0x0A,0x0F,0x02, // up arrow             // 0x18
    0x05,0x0F,0x04, // down arrow           // 0x19
    0x00,0x00,0x38, // first part of m      // 0x1A
    0x18,0x08,0x38, // second part of m     // 0x1B
    0x30,0x0C,0x02, // first part of /div   // 0x1C
    0x28,0x3E,0x00, // second part of /div  // 0x1D
    0x18,0x20,0x18, // third part of /div   // 0x1E
    0x38,0x26,0x38, // delta                // 0x1F
    0x00,0x00,0x00, // Space   // 0x20
    0x00,0x2E,0x00, // !
    0x06,0x00,0x06, // "
    0x3E,0x14,0x3E, // #
    0x14,0x3F,0x0A, // $
    0x12,0x08,0x24, // %
    0x14,0x2A,0x34, // &
    0x00,0x06,0x00, // '
    0x1C,0x22,0x00, // (
    0x22,0x1C,0x00, // )
    0x1C,0x1C,0x1C, // *
    0x08,0x1C,0x08, // +
    0x40,0x20,0x00, // ,
    0x08,0x08,0x08, // -
    0x00,0x20,0x00, // .
    0x10,0x08,0x04, // /
    0x1C,0x22,0x1C, // 0    // 0x30
    0x24,0x3E,0x20, // 1
    0x32,0x2A,0x24, // 2
    0x22,0x2A,0x14, // 3
    0x0E,0x08,0x3E, // 4
    0x2E,0x2A,0x12, // 5
    0x3E,0x2A,0x3A, // 6
    0x02,0x3A,0x06, // 7
    0x3E,0x2A,0x3E, // 8
    0x2E,0x2A,0x3E, // 9
    0x00,0x14,0x00, // :
    0x20,0x14,0x00, // ;
    0x08,0x14,0x22, // <
    0x14,0x14,0x14, // = 
    0x22,0x14,0x08, // >
    0x02,0x2A,0x04, // ?
    0x1C,0x26,0x2E, // @    // 0x40
    0x3C,0x0A,0x3C, // A
    0x3E,0x2A,0x14, // B
    0x1C,0x22,0x22, // C
    0x3E,0x22,0x1C, // D
    0x3E,0x2A,0x22, // E
    0x3E,0x0A,0x02, // F
    0x1C,0x22,0x1A, // G
    0x3E,0x08,0x3E, // H
    0x22,0x3E,0x22, // I
    0x10,0x20,0x1E, // J
    0x3E,0x08,0x36, // K
    0x3E,0x20,0x20, // L
    0x3E,0x04,0x3E, // M
    0x3E,0x02,0x3E, // N
    0x3E,0x22,0x3E, // O
    0x3E,0x0A,0x04, // P    // 0x50
    0x1C,0x22,0x3C, // Q
    0x3E,0x0A,0x34, // R
    0x24,0x2A,0x12, // S
    0x02,0x3E,0x02, // T
    0x3E,0x20,0x3E, // U
    0x1E,0x20,0x1E, // V
    0x3E,0x10,0x3E, // W
    0x36,0x08,0x36, // X
    0x0E,0x30,0x0E, // Y
    0x32,0x2A,0x26, // Z
    0x00,0x3E,0x22, // [
    0x04,0x08,0x10, // /*\*/
    0x22,0x3E,0x00, // ]
    0x04,0x02,0x04, // ^
    0x40,0x40,0x40, // _
    0x02,0x04,0x00, // `   // 0x60
    0x34,0x2c,0x38, // a
    0x3e,0x24,0x18, // b
    0x18,0x24,0x24, // c
    0x18,0x24,0x3e, // d
    0x18,0x34,0x2c, // e
    0x08,0x3c,0x0a, // f
    0x58,0x54,0x3c, // g
    0x3e,0x04,0x38, // h
    0x00,0x3a,0x00, // i
    0x40,0x3a,0x00, // j
    0x3e,0x18,0x24, // k
    0x00,0x1e,0x20, // l
    0x3c,0x0c,0x3c, // m
    0x3c,0x04,0x38, // n
    0x18,0x24,0x18, // o
    0x7c,0x24,0x18, // p
    0x18,0x24,0x7c, // q
    0x38,0x04,0x04, // r
    0x28,0x3c,0x14, // s
    0x04,0x3e,0x24, // t
    0x3c,0x20,0x3c, // u
    0x1c,0x20,0x1c, // v
    0x3c,0x30,0x3c, // w
    0x24,0x18,0x24, // x
    0x0c,0x50,0x3c, // y
    0x34,0x3c,0x2c, // z
    0x08,0x36,0x22, // {
    0x00,0x36,0x00, // |
    0x22,0x36,0x08, // }
    0x04,0x06,0x02, // ~
    0x3e,0x3e,0x3e, // DEL - full block
};

static const unsigned char BigFonts[]  = {
    0xF8,0xFE,0xFF,0x07,0x03,0x03,0x07,0xFF,0xFE,0xF8,0x0F,0x3F,0x7F,0x70,0x60,0x60,0x70,0x7F,0x3F,0x0F,
    0x00,0x10,0x18,0x1C,0xFE,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x00,0x00,0x00,
    0x0C,0x0E,0x0F,0x07,0x03,0x83,0xC7,0xFF,0xFE,0x7C,0x70,0x78,0x7C,0x7E,0x6F,0x67,0x63,0x61,0x60,0x60,
    0x0C,0x0E,0x0F,0xC7,0xC3,0xE7,0xFF,0xFE,0xBC,0x00,0x18,0x38,0x78,0x70,0x60,0x60,0x71,0x7F,0x3F,0x1F,
    0x00,0x80,0xC0,0xF0,0x78,0x1E,0xFF,0xFF,0xFF,0x00,0x0E,0x0F,0x0F,0x0D,0x0C,0x0C,0x7F,0x7F,0x7F,0x0C,
    0xF0,0xFF,0xFF,0x7F,0x63,0x63,0xE3,0xE3,0xC3,0x83,0x0C,0x3C,0x7C,0x70,0x60,0x60,0x70,0x7F,0x3F,0x1F,
    0xF8,0xFE,0xFF,0xC7,0xC3,0xC3,0xC7,0xCF,0x8E,0x0C,0x0F,0x3F,0x7F,0x71,0x60,0x60,0x71,0x7F,0x3F,0x1F,
    0x03,0x03,0x03,0x83,0xE3,0xFB,0x7F,0x1F,0x07,0x03,0x00,0x00,0x7C,0x7F,0x7F,0x03,0x00,0x00,0x00,0x00,
    0x1C,0x3E,0xFF,0xE7,0xC3,0xC3,0xE7,0xFF,0x3E,0x1C,0x1E,0x3F,0x7F,0x73,0x61,0x61,0x73,0x7F,0x3F,0x1E,
    0x7C,0xFE,0xFF,0xC7,0x83,0x83,0xC7,0xFF,0xFE,0xF8,0x18,0x38,0x79,0x71,0x61,0x61,0x71,0x7F,0x3F,0x0F
};


unsigned char testbit( unsigned char mask, unsigned char bit ) 
{
  return mask & _BV( bit ) ;
}

static void OLEDSpiInit(void)
{
        LCD_SDIN_HIGH() ;       
        LCD_SCLK_HIGH();

        DelayuS(50);
}

static void OLEDSpiSendByte(unsigned char byte)
{
  unsigned char  i ;
  unsigned char  tbyte = byte ;       
  
  LCD_SCLK_HIGH( );
  
  for(i = 0 ; i < 8 ; i++) {
	 
	if( tbyte & 0x80 ) {            
	  LCD_SDIN_HIGH() ;                       
	} else {
	  LCD_SDIN_LOW() ;         
	}
	
	LCD_SCLK_LOW() ;                    
	LCD_SCLK_HIGH( );  
	
	tbyte  <<= 1;           
  }
}

void InitOLED( void )
{
  
  u8CursorX = 0;
  u8CursorY = 0;
  
  memset(sVideoRAM,0,sizeof(sVideoRAM));
 
	// Fill in the prefix command byte on each page line of the display buffer
	// We can do it during initialisation as this byte is never overwritten.
  
#ifdef SSD1306_PAGE_PREFIX
	{
		unsigned	i;

		for(i=0; i < GDISP_SCREEN_HEIGHT/8 * SSD1306_PAGE_WIDTH; i+=SSD1306_PAGE_WIDTH)
			RAM(g)[i] = SSD1306_PAGE_PREFIX;
	}
#endif
   DelaymS(500);
	OLEDInit();
		
}
void OLEDClear( void ) 
{
	// Reset Cursor  
  u8CursorX = 0;
  u8CursorY = 0;
  
  memset(sVideoRAM,0,sizeof(sVideoRAM));
}

/*-------------------------------------------------------------------------------
Send instruction to the LCD
	LcdInstructionWrite (uint8_t u8Instruction)
		u8Instruction = Instruction to send to the LCDCHSIZE 2 2469
-------------------------------------------------------------------------------*/
void LcdInstructionWrite (unsigned char u8Instruction)
{
  GPIO_WriteBit(OLED_PORT,CS_PIN,Bit_RESET);			// Select
  GPIO_WriteBit(OLED_PORT,DC_PIN,Bit_RESET);             // Instruction mode
	
  OLEDSpiSendByte(u8Instruction);  
}

void InvertOLED( unsigned char state ) 
{
  LcdInstructionWrite(state?SSD1306_INVERTDISPLAY:SSD1306_NORMALDISPLAY);
}

void OLEDInit(void)	
{
  int y;
  
  unsigned short test;
  unsigned short test2;
  
    // Recommended power up sequence
    OLEDSpiInit();
	DelayuS(4);
	
	
	GPIO_WriteBit(OLED_PORT,RES_PIN,Bit_RESET); 
	DelayuS(4);
	
	GPIO_WriteBit(OLED_PORT,RES_PIN,Bit_SET); 
	DelayuS(4);

    // Recommended initialization sequence
    LcdInstructionWrite(LCD_DISP_OFF);
    LcdInstructionWrite(LCD_SET_RATIO_OSC);
    LcdInstructionWrite(0x80);
    LcdInstructionWrite(LCD_MULTIPLEX);
    LcdInstructionWrite(0x3F);
    LcdInstructionWrite(LCD_SET_OFFSET);
    LcdInstructionWrite(0x00);
    LcdInstructionWrite(LCD_SET_LINE);
    LcdInstructionWrite(LCD_CHARGE_PUMP);
    LcdInstructionWrite(LCD_PUMP_ON);
	
//set at main LcdInstructionWrite(LCD_SET_SEG_REMAP1);
//set at main LcdInstructionWrite(LCD_SET_SCAN_NOR);
    LcdInstructionWrite(LCD_SET_PADS);
    LcdInstructionWrite(0x12);
    LcdInstructionWrite(LCD_SET_CONTRAST);
    LcdInstructionWrite(0xFF);
    LcdInstructionWrite(LCD_SET_CHARGE);
    LcdInstructionWrite(0xF1);
    LcdInstructionWrite(LCD_SET_VCOM);
    LcdInstructionWrite(0x40);
    LcdInstructionWrite(LCD_EON_OFF);
    LcdInstructionWrite(LCD_DISP_NOR);
    LcdInstructionWrite(LCD_MEM_ADDRESSING);
    LcdInstructionWrite(0x00);          // Horizontal Addressing mode
    LcdInstructionWrite(LCD_DISP_ON);

	test2 = 40;
		
	while( test2--  ) {
	  
		test = 100;
	
		while( test-- )  {
		  
	  		OLEDSetPixel(rand()%128,rand()%64,rand()%2);
		}
		OLEDDraw();
		OLEDClear();
	}
	
	DelaymS( 500 );
	
	// Push cleared buffer
	OLEDDraw();
	
	
	OLEDPutstr("LAYERONE 2014 PROTOTYPE\n");
	OLEDDraw();

	OLEDPutstr("OLED TEST\n");
	OLEDDraw();

	for( y = 0 ; y < 10 ; y++ ) {
	  
		InvertOLED(1);
		DelaymS(10*y);
		InvertOLED(0);
		DelaymS(10*y);
	}
	
	OLEDPutstr("AUDIO TEST\n");
	OLEDDraw();
	
	for( y=0;y<400;y++ ) {
		SetSpeaker( y * 100 );
	}

	while(y--){	
		SetSpeaker( y * 100 );
	}
	
	OLEDPutstr("LED TEST\n");
	OLEDDraw();
	
	for( y=0;y<20;y++ ) {
		
	  LEDSet(0);
		DelaymS(100);
		
		LEDSet(1);
		DelaymS(10*y);
	}
	
	OLEDPutstr("DONE\n");
	OLEDDraw();
		
}

void OLEDDraw(void) 
{
  unsigned short i;
  
  LcdInstructionWrite(LCD_SET_COL_LO | 0x0);  
  LcdInstructionWrite(LCD_SET_COL_HI | 0x0); 
  LcdInstructionWrite(SSD1306_SETSTARTLINE | 0x0); 
  
  GPIO_WriteBit(OLED_PORT,CS_PIN,Bit_SET);			// Select
  GPIO_WriteBit(OLED_PORT,DC_PIN,Bit_SET);             // Instruction mode
  GPIO_WriteBit(OLED_PORT,CS_PIN,Bit_RESET);			// Select
  
  for ( i=0; i<(GDISP_SCREEN_WIDTH*GDISP_SCREEN_HEIGHT/8); i++) {
	OLEDSpiSendByte(sVideoRAM[i]);
  }
  // i wonder why we have to do this (check datasheet)
  if (GDISP_SCREEN_HEIGHT == 32) {
	for (unsigned short i=0; i<(GDISP_SCREEN_WIDTH*GDISP_SCREEN_HEIGHT/8); i++) {
	  OLEDSpiSendByte(0);
	}
  }
  
  GPIO_WriteBit(OLED_PORT,CS_PIN,Bit_SET);
}


void OLEDSetPixel(unsigned char x, unsigned char y, unsigned char color) 
{
  if ((x >= GDISP_SCREEN_WIDTH) || (y >= GDISP_SCREEN_HEIGHT) )
    return;

  // x is which column
  if (color ) 
    sVideoRAM[x+ (y/8)*GDISP_SCREEN_WIDTH] |= _BV((y%8));  
  else
    sVideoRAM[x+ (y/8)*GDISP_SCREEN_WIDTH] &= ~_BV(((y%8))); 
}

void FlushOLED( void )
{
}

// OR byte on display buffer
void write_display(unsigned char data) 
{
    sVideoRAM[((unsigned short )(u8CursorY<<7)) + (u8CursorX++)] |= data;
}



/*-------------------------------------------------------------------------------
Print a char on the LCD
	OLEDBigchar (unsigned char u8Char)
		u8Char = char to display
-------------------------------------------------------------------------------*/
void OLEDBigchar (unsigned char u8Char) 
{
	
  unsigned char  i=0;
    unsigned short *pointer;
	
	if(u8Char=='.') {           // Small point to Save space
		write_display (0x60);
		write_display (0x60);
		u8CursorX+=2;
	}
	else if(u8Char=='-') {      // Negative sign
		write_display (0x03);
		write_display (0x03);
		write_display (0x03);
		write_display (0x03);
		u8CursorX+=2;
    }
	else if(u8Char==' ') {      // Space
		u8CursorX+=6;
    }
	else {                      // Number
		pointer = (unsigned short*)(BigFonts)+(u8Char)*20;
		// Upper side
		u8CursorY--;
		
		while (i < 10) { 
		  
		  write_display ( *pointer) ;
		  pointer++ ; 
		  i++; 
		}
		
		i=0;
		// Lower Side
		u8CursorY++;
		u8CursorX-=10;
		while (i < 10) { write_display((*pointer++)); i++; }
        u8CursorX+=2;
	}
}

void OLEDPutchar(char u8Char) 
{
    uint8_t *pointer;
	uint8_t data,u8CharColumn=0;
	pointer = (uint8_t*)(Fonts)+(u8Char-22)*(3);
	
   	/* Draw a char */
	while (u8CharColumn < 3)	{
	  
        data = (*pointer++);

		if(testbit(Misc,NEGATIVE)) 
		  data = ~(data|128);
		
		write_display(data);
		
		u8CharColumn++;
	}
    // Special characters
    if(u8Char==0x1C) {       // Begin long 'd' character
        write_display(0x30);
    }
    else if(u8Char==0x1D) {  // Complete long 'd' character
        write_display(0x38);
        u8CursorX++;
    }
    else if(u8Char==0x1A) {  // Complete long 'm' character
        write_display(0x08);
    }
    else if(u8CursorX < 128) {  // if not then insert a space before next letter
		data = 0;
		if(testbit(Misc,NEGATIVE)) data = 127;
		write_display(data);
	}
    if(u8CursorX>=128) {    // Next line
        u8CursorX = 0; u8CursorY++;
    }
}

void OLEDPutstr (const char *ptr)
{    
  char c;
  
  while ((c=*(ptr++)) != 0x00) {
	if( c == '\n' ){
	  u8CursorX = 0;
	  u8CursorY++;
	}else  {
		OLEDPutchar(c);
	}
  }
}



/*-------------------------------------------------------------------------------
Send an image to the LCD
	OLEDDisplayPicture (uint8_t *au8PictureData)
		au8PictureData = contains encoded data for picture

	Decode:
	Get one byte, put it to the output file, and now it's the 'last' byte. 
	Loop 
	Get one byte 
	Is the current byte equal to last? 
	Yes 
		Now get another byte, this is 'counter' 
		Put current byte in the output file 
		Copy 'counter' times the 'last' byte to the output file 
		Put last copied byte in 'last' (or leave it alone) 
	No 
		Put current byte to the output file 
		Now 'last' is the current byte 
	Repeat. 
-------------------------------------------------------------------------------*/
void OLEDDisplayPicture (const uint8_t *pointer) 
{
	uint8_t data=0, count=0;
    uint16_t i;
    uint8_t *p;
    
	p = sVideoRAM;
	
  	for (i = 0; i < 1024; i++) {
        if(count==0) {
			data = *(pointer++);
			if(data==*(pointer++)) {
				count = *(pointer++);
			}
			else {
				count = 1;
				pointer--;
			}
		}
		count--;
        *p++ = data;
	}
}


