#include <stdint.h>
#include <avr/pgmspace.h>
#include "display.h"
#include "mygccdef.h"
#include "main.h"

uint8_t     display_buffer[1024];
uint8_t     u8CursorX, u8CursorY;

// System 3x6 (char #22 to #96) 
const uint8_t Fonts[] PROGMEM = {
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

const uint8_t BigFonts[] PROGMEM = {
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

// Sprites
const int8_t tdown[] PROGMEM = { 0,0, 0,2,  1,0, 1,1,  1,2,  1,3,  2,2, 255  }; // Falling trigger
const int8_t tup[]   PROGMEM = { 0,0, 0,-2, 1,0, 1,-1, 1,-2, 1,-3, 2,-2, 255 }; // Rising trigger
const int8_t tdual[] PROGMEM = { 0,0, 0,-2, 1,0, 1,-1, 1,-2, 1,-3, 2,-2, 0,2,  1,0, 1,1,  1,2,  1,3,  2,2, 255 }; // Dual edge

// Clear display buffer
void clr_display(void) {
    for(uint16_t i=0; i<1024; i++) display_buffer[i]=0;
    lcd_goto(0,0);
}

void GLCD_setting(void) {
    if(testbit(Display, flip)) {
        LcdInstructionWrite(LCD_SET_SCAN_NOR);   // direction
        LcdInstructionWrite(LCD_SET_SEG_REMAP1);
    }
    else {
        LcdInstructionWrite(LCD_SET_SCAN_FLIP);   // direction
        LcdInstructionWrite(LCD_SET_SEG_REMAP0);
    }
    if(testbit(Display, disp_inv)) LcdInstructionWrite(LCD_DISP_REV);   // invert
    else LcdInstructionWrite(LCD_DISP_NOR);   // no invert
}

// Set pixel on display buffer
void set_pixel(uint8_t x, uint8_t y) {
    display_buffer[((uint16_t)(y<<4)&0xFF80) + x] |= (uint8_t)(0x01 << (y & 0x07));
}

// Set pixel on display buffer
void clr_pixel(uint8_t x, uint8_t y) {
    display_buffer[((uint16_t)(y<<4)&0xFF80) + x] &= ~(uint8_t)(0x01 << (y & 0x07));
}

void sprite(uint8_t x, uint8_t y, const int8_t *ptr) {
    int8_t a, b;
    do {
        a=pgm_read_byte(ptr++);
        b=pgm_read_byte(ptr++);
        if((uint8_t)a==255) return;
        set_pixel(x+a,y+b);
    } while(1);
}
    
// OR byte on display buffer
void write_display(uint8_t data) {
    display_buffer[((uint16_t)(u8CursorY<<7)) + (u8CursorX++)] |= data;
}

//-----------------------------------------------------------------------
void lcd_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color) {
    uint8_t i,dxabs,dyabs,x,y;
    int8_t dx,dy,stepx,stepy;
    dx=(int8_t)x2-x1;      // the horizontal distance of the line
    dy=(int8_t)y2-y1;      // the vertical distance of the line
    if(dy<0) { dyabs=-dy; stepy=-1; }
    else { dyabs=dy; stepy=1; }
    if(dx<0) { dxabs=-dx; stepx=-1; }
    else {dxabs=dx; stepx=1; }
    x=(uint8_t)(dyabs>>1);
    y=(uint8_t)(dxabs>>1);
    if(color) set_pixel(x1,y1); else clr_pixel(x1,y1);
    if (dxabs>=dyabs) { // the line is more horizontal than vertical
        for(i=0;i<dxabs;i++) {
            y+=dyabs;
            if (y>=dxabs) {
                y-=dxabs;
                y1+=stepy;
            }
            x1+=stepx;
            if(color) set_pixel(x1,y1); else clr_pixel(x1,y1);
        }
    }
    else {  // the line is more vertical than horizontal
        for(i=0;i<dyabs;i++) {
            x+=dxabs;
            if (x>=dyabs) {
                x-=dyabs;
                x1+=stepx;
            }
            y1+=stepy;
            if(color) set_pixel(x1,y1); else clr_pixel(x1,y1);
        }
    }
}

/*-------------------------------------------------------------------------------
Print a char on the LCD
	GLCD_Putchar (uint8_t u8Char)
		u8Char = char to display
-------------------------------------------------------------------------------*/
void GLCD_Putchar(char u8Char) {
    uint16_t pointer;
	uint8_t data,u8CharColumn=0;
	pointer = (unsigned int)(Fonts)+(u8Char-22)*(3);
   	/* Draw a char */
	while (u8CharColumn < 3)	{
        data = pgm_read_byte_near(pointer++);
		if(testbit(Misc,negative)) data = ~(data|128);
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
		if(testbit(Misc,negative)) data = 127;
		write_display(data);
	}
    if(u8CursorX>=128) {    // Next line
        u8CursorX = 0; u8CursorY++;
    }
}

/*-------------------------------------------------------------------------------
Print a char on the LCD
	GLCD_Putchar (uint8_t u8Char)
		u8Char = char to display
-------------------------------------------------------------------------------*/
void GLCD_Bigchar (char u8Char) {
	uint8_t i=0;
    uint16_t pointer;
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
		pointer = (unsigned int)(BigFonts)+(u8Char)*20;
		// Upper side
		u8CursorY--;
		while (i < 10) { write_display (pgm_read_byte_near(pointer++)); i++; }
		i=0;
		// Lower Side
		u8CursorY++;
		u8CursorX-=10;
		while (i < 10) { write_display(pgm_read_byte_near(pointer++)); i++; }
        u8CursorX+=2;
	}
}

/*-------------------------------------------------------------------------------
Print a string on the LCD from a string in program memory
	GLCD_Printf (uint8_t *au8Text) 
		*au8Text = string to display
-------------------------------------------------------------------------------*/
void lcd_putsp (const char *ptr) {
    char c;
    while ((c=pgm_read_byte(ptr++)) != 0x00) {
        GLCD_Putchar(c);
    }
}

// Print Number
void printN(uint8_t Data) {
    uint8_t D=0x30;
	while (Data>=100)	{ D++; Data-=100; }
    GLCD_Putchar(D); D=0x30;
	while (Data>=10)	{ D++; Data-=10; }
    GLCD_Putchar(D);
    GLCD_Putchar(Data+0x30);
}

// Print Fixed point Number
// 5 decimal digits
void printF(uint8_t x, uint8_t y, int32_t Data) {
	uint8_t D[4]={0,0,0,0},point=0,i;
 	uint16_t n;
    lcd_goto(x,y);
    if(Data<0) {
        Data=-Data;
        if(testbit(Misc,bigfont)) GLCD_Bigchar('-');
        else GLCD_Putchar('-');
    }
    else {
        if(testbit(Misc,bigfont)) GLCD_Bigchar(' ');
        else GLCD_Putchar(' ');
    }
	if(Data>=999900000L) n = 9999;
	else if(Data>=100000000L)  n = (uint16_t)(Data/100000);
	else if(Data>=10000000L) {
		n = (uint16_t)(Data/10000);
		point = 1;
	}
	else if(Data>=1000000L) {
		n = (uint16_t)(Data/1000);
		point = 2;
	}
	else {
		n = (uint16_t)(Data/100);
		point = 3;
	}

	while (n>=1000) { D[3]++; n-=1000; }
	while (n>=100)	{ D[2]++; n-=100; }
	while (n>=10)	{ D[1]++; n-=10; }
	while (n>=1)	{ D[0]++; n-=1; }

	for(i=3; i!=255; i--) {
		if(testbit(Misc,bigfont)) {
			GLCD_Bigchar(D[i]);
			if(point==i) GLCD_Bigchar('.');
		}
		else {
			GLCD_Putchar(0x30+D[i]);
			if(point==i) GLCD_Putchar('.');
		}
	}
}

// Print small font text at x,y from program memory
void tiny_printp(uint8_t x, uint8_t y, const char *ptr) {
    lcd_goto(x,y);
    lcd_putsp(ptr);
}

/*-------------------------------------------------------------------------------
Send an image to the LCD
	GLCD_DisplayPicture (uint8_t *au8PictureData)
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
void GLCD_DisplayPicture (const uint8_t *pointer) {
	uint8_t data=0, count=0;
    uint16_t i;
    uint8_t *p;
    p = display_buffer;
  	for (i = 0; i < 1024; i++) {
        if(count==0) {
			data = pgm_read_byte_near(pointer++);
			if(data==pgm_read_byte_near(pointer++)) {
				count = pgm_read_byte_near(pointer++);
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
