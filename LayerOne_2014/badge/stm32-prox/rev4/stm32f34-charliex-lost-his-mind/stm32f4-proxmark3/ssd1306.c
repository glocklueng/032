/// SSD1306 driver

#include <stdlib.h>
#include <string.h>

#include "stm32f4xx.h"

#include "proxmark3.h"
#include "hw_config.h"
#include "l1_board.h"

#define GDISP_SCREEN_HEIGHT     (64)
#define GDISP_SCREEN_WIDTH      (128)

#define SCREEN_WIDTH            GDISP_SCREEN_WIDTH
#define SCREEN_HEIGHT           GDISP_SCREEN_HEIGHT

#define m_clip_x_max            GDISP_SCREEN_WIDTH
#define m_clip_y_max            GDISP_SCREEN_HEIGHT
#define m_clip_x_min            0
#define m_clip_y_min            0

#ifndef GDISP_INITIAL_CONTRAST
#define GDISP_INITIAL_CONTRAST  100
#endif
#ifndef GDISP_INITIAL_BACKLIGHT
#define GDISP_INITIAL_BACKLIGHT 100
#endif

#ifdef SSD1306_PAGE_PREFIX
#define SSD1306_PAGE_WIDTH      (GDISP_SCREEN_WIDTH+1)
#define SSD1306_PAGE_OFFSET     1
#else
#define SSD1306_PAGE_WIDTH      GDISP_SCREEN_WIDTH
#define SSD1306_PAGE_OFFSET     0
#endif

#undef BOARD_REVISION

// Buffer for OLED
static unsigned char sVideoRAM[GDISP_SCREEN_HEIGHT/8 * SSD1306_PAGE_WIDTH ];

#define xyaddr(x, y)            (SSD1306_PAGE_OFFSET + (x) + ((y)>>3)*SSD1306_PAGE_WIDTH)
#define xybit(y)                (1<<((y)&7))

#if BOARD_REVISION != 3
// These should be hardware SPI in board 3

#define LCD_SCLK_HIGH()         GPIO_WriteBit(OLED2_PORT,SCLK_DB0_PIN,Bit_SET)
#define LCD_SCLK_LOW()          GPIO_WriteBit(OLED2_PORT,SCLK_DB0_PIN,Bit_RESET)

#define LCD_SDIN_HIGH()         GPIO_WriteBit(OLED2_PORT,SDIN_DB1_PIN,Bit_SET)
#define LCD_SDIN_LOW()          GPIO_WriteBit(OLED2_PORT,SDIN_DB1_PIN,Bit_RESET)

#endif

// Cursor
static unsigned char    u8CursorX, u8CursorY;
static unsigned char    Misc = 0;

#define             NEGATIVE        ( 1 )

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


/*** The 8x6 hard coded font
 *
 * This font is always available
 *
 **/
static const unsigned char font8x6[354] = {
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
 *
 * This font is always available
 *
 **/
const unsigned char font6x6[] = {
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

unsigned char testbit ( unsigned char mask, unsigned char bit )
{
	return mask & _BV ( bit ) ;
}

static void OLEDSpiInit ( void )
{
#if BOARD_REVISION == 3

	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;

#else

	LCD_SDIN_HIGH() ;
	LCD_SCLK_HIGH();

	DelayuS ( 50 );

#endif

#if BOARD_REVISION == 3

	/* Configure SPI1 pins: SCK, MISO */
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 ;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	//GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Mode 0
	/* SPI1 Periph clock enable */
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_SPI1, ENABLE );
	SPI_I2S_DeInit ( SPI1 );
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; // 72MHz/8 = 9MHz, /4 not use
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init ( SPI1, &SPI_InitStructure );

	/* SPI1 enable */
	SPI_Cmd ( SPI1, ENABLE );
#endif
}


static void OLEDSpiSendInstruction ( unsigned char byte )
{
#if BOARD_REVISION == 3
	// Set !CS , deselect DC
	GPIO_WriteBit ( OLED_CS_PORT, CS_PIN, Bit_RESET );
	GPIO_WriteBit ( OLED_PORT, DC_PIN, Bit_RESET );

	SPI_I2S_SendData ( SPI1, byte );

	while ( SPI_I2S_GetFlagStatus ( SPI1, SPI_I2S_FLAG_TXE ) == RESET );

	while ( !SPI_I2S_GetFlagStatus ( SPI1, SPI_I2S_FLAG_BSY ) == RESET );

	// Deselect !CS
	GPIO_WriteBit ( OLED_CS_PORT, CS_PIN, Bit_SET );
#endif

}

static void OLEDSpiSendByte ( unsigned char byte )
{
#if BOARD_REVISION == 3

	// Set !CS , select DC
	GPIO_WriteBit ( OLED_CS_PORT, CS_PIN, Bit_RESET );
	GPIO_WriteBit ( OLED_PORT, DC_PIN, Bit_SET );

	SPI_I2S_SendData ( SPI1, byte );

	while ( SPI_I2S_GetFlagStatus ( SPI1, SPI_I2S_FLAG_TXE ) == RESET );

	while ( !SPI_I2S_GetFlagStatus ( SPI1, SPI_I2S_FLAG_BSY ) == RESET );

	GPIO_WriteBit ( OLED_CS_PORT, CS_PIN, Bit_SET );


#else

	unsigned char  i ;
	unsigned char  tbyte = byte ;

	LCD_SCLK_HIGH( );

	for ( i = 0 ; i < 8 ; i++ ) {

		if ( tbyte & 0x80 ) {
			LCD_SDIN_HIGH() ;
		}

		else {
			LCD_SDIN_LOW() ;
		}

		LCD_SCLK_LOW() ;
		LCD_SCLK_HIGH( );

		tbyte  <<= 1;
	}

#endif
}

void InitOLED ( void )
{

	u8CursorX = 0;
	u8CursorY = 0;

	memset ( sVideoRAM,0,sizeof ( sVideoRAM ) );

	// Fill in the prefix command byte on each page line of the display buffer
	// We can do it during initialisation as this byte is never overwritten.

#ifdef SSD1306_PAGE_PREFIX
	{
		unsigned    i;

		for ( i=0; i < GDISP_SCREEN_HEIGHT/8 * SSD1306_PAGE_WIDTH; i+=SSD1306_PAGE_WIDTH )
		{ RAM ( g ) [i] = SSD1306_PAGE_PREFIX; }
	}
#endif
	DelaymS ( 500 );
	OLEDInit();

	OLEDClear();
	OLEDDraw();
}

void OLEDClear ( void )
{
	// Reset Cursor
	u8CursorX = 0;
	u8CursorY = 0;

	memset ( sVideoRAM,0,sizeof ( sVideoRAM ) );
}

/*-------------------------------------------------------------------------------
Send instruction to the LCD
    LcdInstructionWrite (uint8_t u8Instruction)
        u8Instruction = Instruction to send to the LCDCHSIZE 2 2469
-------------------------------------------------------------------------------*/
void LcdInstructionWrite ( unsigned char u8Instruction )
{
#if BOARD_REVISION == 3

	OLEDSpiSendInstruction ( u8Instruction ) ;

#else

	GPIO_WriteBit ( OLED_CS_PORT,CS_PIN,Bit_RESET );      // Select
	GPIO_WriteBit ( OLED_PORT,DC_PIN,Bit_RESET );          // Instruction mode

	OLEDSpiSendByte ( u8Instruction );

	while ( !SPI_I2S_GetFlagStatus ( SPI1, SPI_I2S_FLAG_BSY ) == RESET );

	// Chip not selected
	GPIO_WriteBit ( OLED_CS_PORT,CS_PIN,Bit_SET );
#endif

}

void InvertOLED ( unsigned char state )
{
	LcdInstructionWrite ( state?SSD1306_INVERTDISPLAY:SSD1306_NORMALDISPLAY );
}

void OLEDInit ( void )
{

	// Recommended power up sequence
	OLEDSpiInit();
	DelayuS ( 4 );
	GPIO_WriteBit ( OLED_PORT,RES_PIN,Bit_RESET );
	DelayuS ( 4 );

	GPIO_WriteBit ( OLED_PORT,RES_PIN,Bit_SET );
	DelayuS ( 4 );



	// Recommended initialization sequence
	LcdInstructionWrite ( LCD_DISP_OFF );
	LcdInstructionWrite ( LCD_SET_RATIO_OSC );
	LcdInstructionWrite ( 0x80 );
	LcdInstructionWrite ( LCD_MULTIPLEX );
	LcdInstructionWrite ( 0x3F );
	LcdInstructionWrite ( LCD_SET_OFFSET );
	LcdInstructionWrite ( 0x00 );
	LcdInstructionWrite ( LCD_SET_LINE );
	LcdInstructionWrite ( LCD_CHARGE_PUMP );
	LcdInstructionWrite ( LCD_PUMP_ON );

	//set at main LcdInstructionWrite(LCD_SET_SEG_REMAP1);
	//set at main LcdInstructionWrite(LCD_SET_SCAN_NOR);
	LcdInstructionWrite ( LCD_SET_PADS );
	LcdInstructionWrite ( 0x12 );
	LcdInstructionWrite ( LCD_SET_CONTRAST );
	LcdInstructionWrite ( 0xFF );
	LcdInstructionWrite ( LCD_SET_CHARGE );
	LcdInstructionWrite ( 0xF1 );
	LcdInstructionWrite ( LCD_SET_VCOM );
	LcdInstructionWrite ( 0x40 );
	LcdInstructionWrite ( LCD_EON_OFF );
	LcdInstructionWrite ( LCD_DISP_NOR );
	
	// horiz flip LCD_SET_SEG_REMAP0 / LCD_SET_SEG_REMAP1
	LcdInstructionWrite ( LCD_SET_SEG_REMAP0);
	
	// vertical flip LCD_SET_SCAN_NOR / LCD_SET_SCAN_FLIP
	LcdInstructionWrite ( LCD_SET_SCAN_FLIP );
	LcdInstructionWrite ( LCD_MEM_ADDRESSING );
	LcdInstructionWrite ( 0x00 );       // Horizontal Addressing mode
	LcdInstructionWrite ( LCD_DISP_ON );

	
}

void OLEDTest( void ) 
{
  	int y;

	
  	unsigned short test;
	unsigned short test2;
//  while( 1)
	{
		test2 = 40;


		while ( test2--  ) {

			test = 100;

			while ( test-- )  {

				OLEDSetPixel ( rand() %128,rand() %64,rand() %2 );
			}

			OLEDDraw();
			OLEDClear();
		}
	}
	DelaymS ( 500 );

	// Push cleared buffer
	OLEDDraw();


	OLEDPutstr ( "LAYERONE 2014 PROTOTYPE\n" );
	OLEDDraw();

	OLEDPutstr ( "OLED TEST\n" );
	OLEDDraw();

	for ( y = 0 ; y < 10 ; y++ ) {

		InvertOLED ( 1 );
		DelaymS ( 10*y );
		InvertOLED ( 0 );
		DelaymS ( 10*y );
	}

	OLEDPutstr ( "AUDIO TEST\n" );
	OLEDDraw();

	for ( y=0; y<400; y++ ) {
		SetSpeaker ( y * 100 );
	}

	while ( y-- ) {
		SetSpeaker ( y * 100 );
	}

	OLEDPutstr ( "LED TEST\n" );
	OLEDDraw();

	for ( y=0; y<20; y++ ) {

		LEDSet ( 1 );
		DelaymS ( 10*y );
		LEDSet ( 0 );
		DelaymS ( 10*y );
	}

	OLEDPutstr ( "RELAY TEST\n" );
	OLEDDraw();

	LEDSet ( 1 );
	HIGH ( RELAY );
	DelaymS ( 500 );

	LEDSet ( 0 );
	LOW ( RELAY );
	DelaymS ( 500 );

	LEDSet ( 1 );
	HIGH ( RELAY );
	DelaymS ( 500 );

	LEDSet ( 0 );
	LOW ( RELAY );
	DelaymS ( 700 );

	OLEDPutstr ( "DONE\n" );
	OLEDDraw();

}

void OLEDDraw ( void )
{
	unsigned short i;

	LcdInstructionWrite ( LCD_SET_COL_LO | 0x0 );
	LcdInstructionWrite ( LCD_SET_COL_HI | 0x0 );
	LcdInstructionWrite ( SSD1306_SETSTARTLINE | 0x0 );

	GPIO_WriteBit ( OLED_CS_PORT,CS_PIN,Bit_SET );    // Select
	GPIO_WriteBit ( OLED_PORT,DC_PIN,Bit_SET );           // Instruction mode
	GPIO_WriteBit ( OLED_CS_PORT,CS_PIN,Bit_RESET );  // Select

	for ( i=0; i< ( GDISP_SCREEN_WIDTH*GDISP_SCREEN_HEIGHT/8 ); i++ ) {
		OLEDSpiSendByte ( sVideoRAM[i] );
	}

	// i wonder why we have to do this (check datasheet)
	if ( GDISP_SCREEN_HEIGHT == 32 ) {
		for ( unsigned short i=0; i< ( GDISP_SCREEN_WIDTH*GDISP_SCREEN_HEIGHT/8 ); i++ ) {
			OLEDSpiSendByte ( 0 );

		}
	}

	// turn of !CS
	GPIO_WriteBit ( OLED_CS_PORT,CS_PIN,Bit_SET );
}

void OLEDSetPixel ( unsigned char x, unsigned char y, unsigned char color )
{
	if ( ( x >= GDISP_SCREEN_WIDTH ) || ( y >= GDISP_SCREEN_HEIGHT ) )
	{ return; }

	// HARDWARE DOES THIS
	//x = x - (GDISP_SCREEN_WIDTH-1);
	//y = (GDISP_SCREEN_HEIGHT-1) -  y;

	// x is which column
	if ( color )
	{ sVideoRAM[x+ ( y/8 ) *GDISP_SCREEN_WIDTH] |= _BV ( ( y%8 ) ); }

	else
	{ sVideoRAM[x+ ( y/8 ) *GDISP_SCREEN_WIDTH] &= ~_BV ( ( ( y%8 ) ) ); }
}

/**
 *     Plots a point (slow routine). 0,0 is the screen center instead of usual top left as 0,0
 *
 * @param x,y           - coordinates of point to plot
 * @param colour        - colour to use
 */
void    OLEDPixelQuad ( int x, int y, unsigned char colour )
{
	OLEDSetPixel ( x  ,y  , colour );
	OLEDSetPixel ( x+1,y  , colour );
	OLEDSetPixel ( x+1,y+1, colour );
	OLEDSetPixel ( x  ,y+1, colour );
}

void FlushOLED ( void )
{
}

// put byte on display buffer
// for this to flip properly, needs 7-y, width-x and data to be reversed
void write_display ( unsigned char data )
{
	sVideoRAM[ ( ( unsigned short ) ( ( u8CursorY ) <<7 ) ) + u8CursorX++] = data;
}

/*-------------------------------------------------------------------------------
    Print a char on the LCD
        OLEDBigchar (unsigned char u8Char)
            u8Char = char to display
-------------------------------------------------------------------------------*/
void OLEDBigchar ( unsigned char u8Char )
{

	unsigned char  i=0;
	uint16_t *pointer;

	if ( u8Char=='.' ) {        // Small point to Save space
		write_display ( 0x60 );
		write_display ( 0x60 );
		u8CursorX+=2;
	}

	else
		if ( u8Char=='-' ) {   // Negative sign
			write_display ( 0x03 );
			write_display ( 0x03 );
			write_display ( 0x03 );
			write_display ( 0x03 );
			u8CursorX+=2;
		}

		else
			if ( u8Char==' ' ) {   // Space
				u8CursorX+=6;
			}

			else {                      // Number

				pointer = ( uint16_t* ) BigFonts[u8Char*20];
				// Upper side
				u8CursorY--;

				while ( i < 10 ) {

					write_display ( *pointer ) ;
					pointer++ ;
					i++;
				}

				i=0;
				// Lower Side
				u8CursorY++;
				u8CursorX-=10;

				while ( i < 10 ) {
					write_display ( *pointer );
					pointer++;
					i++;
				}

				u8CursorX+=2;
			}
}

void OLEDPutchar ( char u8Char )
{
	uint8_t *pointer;
	uint8_t data,u8CharColumn=0;
	pointer = ( uint8_t* ) ( Fonts )+ ( u8Char-22 ) * ( 3 );

	/* Draw a char */
	while ( u8CharColumn < 3 )  {

		data = ( *pointer++ );

		if ( testbit ( Misc,NEGATIVE ) )
		{ data = ~ ( data|128 ); }

		write_display ( data );

		u8CharColumn++;
	}

	// Special characters
	if ( u8Char==0x1C ) {    // Begin long 'd' character
		write_display ( 0x30 );
	}

	else
		if ( u8Char==0x1D ) { // Complete long 'd' character
			write_display ( 0x38 );
			u8CursorX++;
		}

		else
			if ( u8Char==0x1A ) { // Complete long 'm' character
				write_display ( 0x08 );
			}

			else
				if ( u8CursorX < 128 ) { // if not then insert a space before next letter
					data = 0;

					if ( testbit ( Misc,NEGATIVE ) ) { data = 127; }

					write_display ( data );
				}

	if ( u8CursorX>=128 ) { // Next line
		u8CursorX = 0;
		u8CursorY++;
	}
}

void OLEDPutstr ( const char *ptr )
{
	char c;

	if ( u8CursorY > 6 )
	{ u8CursorY = 0 ; }

	while ( ( c=* ( ptr++ ) ) != 0x00 ) {
		if ( c == '\n' ) {
			u8CursorX = 0;
			u8CursorY++;
		}

		else
			if ( c == '\r' ) {
				u8CursorX = 0;
			}

			else  {
				OLEDPutchar ( c );
			}
	}
}

void OLEDPutBigstr ( const char *ptr )
{
	char c;

	while ( ( c=* ( ptr++ ) ) != 0x00 ) {
		if ( c == '\n' ) {
			u8CursorX = 0;
			u8CursorY++;
		}

		else
			if ( c == '\r' ) {
				u8CursorX = 0;
			}

			else  {
				OLEDBigchar ( c );
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
void OLEDDisplayPicture ( const uint8_t *pointer )
{
	uint8_t data=0, count=0;
	uint16_t i;
	uint8_t *p;

	p = sVideoRAM;

	for ( i = 0; i < 1024; i++ ) {
		if ( count==0 ) {
			data = * ( pointer++ );

			if ( data==* ( pointer++ ) ) {
				count = * ( pointer++ );
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

void OLEDDrawXY ( int x,int y )
{

}

// Graphs
unsigned char graphx[ GDISP_SCREEN_WIDTH ] ;

void OLEDDrawGraph ( void )
{
	int x;

	for (  x =0 ; x < GDISP_SCREEN_WIDTH ; x ++ ) {

		OLEDSetPixel ( x,graphx[x],1 );
	}
}

void OLEDSetCursor ( int x,int y )
{
	u8CursorX = x;
	u8CursorY = y;
}

/**
 * Compute outcodes for clipper.
 */
static int BuildOutcodes ( int x, int y )
{
	register int code = 0;

	if ( y >= m_clip_y_max )
	{ code |= 1; }        // top

	else
		if ( y < m_clip_y_min ) { code |= 2; } // bottom

	if ( x >= m_clip_x_max ) { code |= 4; }     // right

	else
		if ( x < m_clip_x_min ) { code |= 8; } // left

	return ( code );
}

char clipLine ( int x1, int y1, int x2, int y2,
                int *x3, int *y3, int *x4, int *y4 )
{
	int code1, code2, codeout;
	bool accept = false, done = false ;

	//the region outcodes for the endpoints
	code1 = BuildOutcodes ( x1, y1 );
	code2 = BuildOutcodes ( x2, y2 );

//In theory, this can never end up in an infinite loop, it'll always come in one of the trivial cases eventually
	do {
		//accept because both endpoints are in screen or on the border, trivial accept
		if ( ! ( code1 | code2 ) ) {
			accept = done = 1;
		}

		else
			if ( code1 & code2 ) {
				//the line isn't visible on screen, trivial reject
				done = 1;
				//if no trivial reject or accept, continue the loop
			}

			else  {

				int x, y;

				codeout = code1 ? code1 : code2;

				if ( codeout & 1 ) { //top
					x = x1 + ( x2 - x1 ) * ( m_clip_y_max - y1 ) / ( y2 - y1 );
					y = m_clip_y_max - 1;
				}

				else
					if ( codeout & 2 ) { //bottom
						x = x1 + ( x2 - x1 ) * -y1 / ( y2 - y1 );
						y = 0;
					}

					else
						if ( codeout & 4 ) { //right
							y = y1 + ( y2 - y1 ) * ( m_clip_x_max - x1 ) / ( x2 - x1 );
							x = m_clip_x_max - 1;
						}

						else { //left
							y = y1 + ( y2 - y1 ) * -x1 / ( x2 - x1 );
							x = 0;
						}

				if ( codeout == code1 ) { //first endpoint was clipped
					x1 = x;
					y1 = y;
					code1 = BuildOutcodes ( x1, y1 );
				}

				else { //second endpoint was clipped
					x2 = x;
					y2 = y;
					code2 = BuildOutcodes ( x2, y2 );
				}
			}
	}
	while ( done == 0 );

	if ( accept ) {

		*x3 = x1;
		*x4 = x2;
		*y3 = y1;
		*y4 = y2;

		return 1;

	}

	else {

		x3 = x4 = y3 = y4 = 0;

		return 0;
	}
}


void OLEDLine ( int x0, int y0, int x1, int y1,int col )
{
	int step = abs ( y1 - y0 ) > abs ( x1 - x0 ) ;
	int t;

	if ( step ) {
		t = x0;
		x0 = y0;
		y0 = t;
		t = x1;
		x1 = y1;
		y1 = t;
	}

	if ( x0 > x1 ) {
		t = x0;
		x0 = x1;
		x1 = t;
		t = y0;
		y0 = y1;
		y1 = t;
	}

	int dx, dy;

	dx = x1 - x0;
	dy = abs ( y1 - y0 );

	int err = dx / 2;
	int ystep;

	if ( y0 < y1 ) {
		ystep = 1;
	}

	else {
		ystep = -1;
	}

	for ( ; x0<x1; x0++ ) {
		if ( step ) {
			OLEDSetPixel ( y0, x0,col );
		}

		else {
			OLEDSetPixel ( x0, y0,col );
		}

		err -= dy;

		if ( err < 0 ) {
			y0 += ystep;
			err += dx;
		}
	}
}

/**
 * Draw an 8 bit string using the builtin 8x6 font (this font is always available)  possibly using transparency
 *
 * @param x     - X position on screen, will be clipped.
 * @param y     - Y position on screen, will be clipped.
 * @param string- 8 bit ASCIZ string
 * @param f_colour - foreground colour to use 0/1
 * @param b_colour - background colour to use 0/1
 */
void OLEDText8x6 ( short x,short y,const char *string,unsigned char f_colour, unsigned char b_colour )
{
	short nm,i,j,oldx=x;
	unsigned char  cur;
	int l_adr;
	int l=0;

	if ( x == -1 ) {
		// center it
		x =  ( ( SCREEN_WIDTH/2 )- ( ( strlen ( string ) * 7 ) /2 ) );
	}

	while ( *string!='\0' ) {                                 /* until the end of the string */
		if ( *string=='\n' ) {
			x=oldx;
			y+=7;
			l=0;
			string++;
		}

		nm= ( ( *string++ )-' ' );                              /* Starting from space */

		if ( nm > 58 )
		{ nm-=' '; }                                          /* to uppercase 58='Z'-' ' */

		nm *= 6;                                            /* 6 bytes per char */

		for ( i=0,l_adr=0; i<6; i++,l_adr++ ) {
			cur=font8x6[nm++];

			for ( j=0; j<8; j++ ) {
				if ( cur& ( 0x80>>j ) ) {
					OLEDSetPixel ( x+j+l,y+i,f_colour );
				}

				else {
					// if both colours match make it transparent
					if ( f_colour != b_colour ) {
						OLEDSetPixel ( x+j+l,y+i,b_colour );
					}
				}
			}
		}

		//adr+=7;
		l+=7;/* Next word */
	}
}

/**
 * Draw an 8 bit string using the builtin 6x6 font (this font is always available)  possibly using transparency
 *
 * @param x     - X position on screen, will be clipped.
 * @param y     - Y position on screen, will be clipped.
 * @param string- 8 bit ASCIZ string
 * @param f_colour - foreground colour to use
 * @param b_colour - background colour to use (if fore and back same colour, draw transparent)
 */
void OLEDText6x6 ( short x,short y,const char *string,char f_colour,char b_colour )
{
	short nm,i,j,oldx=x;
	unsigned char  cur;
	int l = 0;


	while ( *string!='\0' ) {                                   /* until the end of the string */

		if ( *string=='\n' ) {
			x=oldx;
			y+=6;
			string++;
		}

		else {

			nm= ( ( *string++ )-' ' );                /* Starting from space */

			if ( nm>58 ) { nm-=' '; }                     /* to uppercase 58='Z'-' ' */

			nm*=6;                                    /* 6 bytes per char */

			for ( i=0; i<6; i++ ) {
				cur=font6x6[nm++];

				for ( j=0; j<6; j++ ) {
					if ( cur& ( 0x80>>j ) ) {
						OLEDSetPixel ( x+j+l,y+i,f_colour );
					}

					else {
						/* if both colours match make it transparent */
						if ( f_colour != b_colour ) {
							OLEDSetPixel ( x+j+l,y+i,b_colour );
						}
					}
				}
			}

			l+=5;                                   /* Next word */
		}
	}
}

/**
 *   Draw a horizontal line
 *
 * @param colour - colour
 * @param x       - x position
 * @param length      - line length
 * @param y       - y position
 *
 */
void OLEDFlatLine ( unsigned char colour,short x,unsigned short length,int y )
{
	unsigned short i;

	for ( i = 0 ; i < length ; i++ ) {
		OLEDSetPixel ( x+i, y, colour );
	}
}

/**
 *  Draw a filled solid colour rectangle
 *
 * @param us_colour - colour
 * @param x      - x position
 * @param y      - y position
 * @param width  - rect width
 * @param height - rect height
 *
 */
void OLEDFilledRect ( unsigned char colour, int x,int y, unsigned int width,unsigned int height )
{
	unsigned int i;
	int diff;
	int w,h;

	// if completely offscreen
	if ( ( signed ) ( x + width ) < 0  ) {
		return;
	}

	if ( ( signed ) ( y + height ) < 0  ) {
		return;
	}

	// Calculate width from base
	if ( x < 0 ) {
		diff = 0 -x ;

		width = w  = width  - diff;

		if ( w < 1 )
		{ return ; }

		x = 0;
	}

	if ( y < 0 ) {
		diff = 0 - y ;

		height = h  = height - diff;

		if ( h< 1 )
		{ return; }

		y = 0;
	}

	height = MIN ( ( height+y ),m_clip_y_max-1 );


	for ( int j=y; j<height; j++ ) {
		for ( i = 0 ; i < width ; i++ ) {

			OLEDSetPixel ( x+i, j, colour );
		}
	}
}


/**
 *  Draw an outlined single colour rectangle as a frame
 *
 * @param colour    - colour
 * @param x      - x position
 * @param y      - y position
 * @param width  - frame rect width
 * @param height     - frame rect height
 *
 */
void OLEDOutlineRect ( unsigned char colour, int x,int y, int width,int height )
{
	int i;

	if ( height <1 )
	{ return ; }

	if ( width <1 )
	{ return ; }

	OLEDFlatLine ( colour,x,width,y );

	for ( i=y+1; i<y+ ( height-1 ); i++ ) {
		OLEDSetPixel ( x,i,colour );
		OLEDSetPixel ( x+ ( width-1 ),i,colour );
	}

	OLEDFlatLine ( colour,x,width,y+ ( height-1 ) );
}

#define fill_circle(x,y,c) x1 = xc - x; x2 = xc + x; OLEDFlatLine(c,x1,x2-x1,y);


/**
 *  Draw a filled single colour circle
 *
 * @param colour    - colour
 * @param xc        - x centre position
 * @param yc        - y centre position
 * @param radius    - radius of circle
 * @param c         - RGB colour
 *
 */

void DrawCircle ( int xc,int yc, unsigned int radius , unsigned char c )
{
	int x, y, p;
	int x1,x2;

	if ( radius < 1 ) {
		return;
	}

	x = 0;
	y = radius;
	p = 3-2*radius;

	while ( x <= y ) {

		fill_circle ( x,yc+y,c );
		fill_circle ( x,yc-y,c );
		fill_circle ( y,yc+x,c );
		fill_circle ( y,yc-x,c );

		if ( p < 0 ) {
			p = p + 4 * x + 6;
		}

		else {
			p = p + 4 * ( x - y ) + 10;
			y --;
		}

		x++;
	}
}
#define fill_circlea(x,y,c) x1 = xc - x; x2 = xc + x; OLEDFlatLine(c,x1,x2-x1,y);


/**
 *  Draw a filled single colour circle
 *
 * @param colour    - colour
 * @param xc        - x centre position
 * @param yc        - y centre position
 * @param radius    - radius of circle
 * @param c         - RGB colour
 *
 */

void OLEDDrawCircle ( int xc,int yc, unsigned int radius , unsigned char c )
{
	int x, y, p;
	int x1,x2;

	if ( radius < 1 ) {
		return;
	}

	x = 0;
	y = radius;
	p = 3-2*radius;

	while ( x <= y ) {

		fill_circlea ( x,yc+y,c );
		fill_circlea ( x,yc-y,c );
		fill_circlea ( y,yc+x,c );
		fill_circlea ( y,yc-x,c );

		if ( p < 0 ) {
			p = p + 4 * x + 6;
		}

		else {
			p = p + 4 * ( x - y ) + 10;
			y --;
		}

		x++;
	}
}
/**
 *  Draw an outlined single colour circle
 *
 * @param colour    - colour
 * @param xc        - x centre position
 * @param yc        - y centre position
 * @param radius    - radius of circle
 * @param c         - RGB colour
 *
 */
void OLEDDrawOutlineCircle ( int xc,int yc, unsigned int radius , unsigned char c )
{
	int x, y, p;

	if ( !radius ) {
		return;
	}

	x = 0;
	y = radius;
	p = 3 - 2 * radius;

	while ( x <= y ) {

		OLEDSetPixel ( xc+x,yc+y,c );
		OLEDSetPixel ( xc-x,yc+y,c );
		OLEDSetPixel ( xc+x,yc-y,c );
		OLEDSetPixel ( xc-x,yc-y,c );
		OLEDSetPixel ( xc+y,yc+x,c );
		OLEDSetPixel ( xc-y,yc+x,c );
		OLEDSetPixel ( xc+y,yc-x,c );
		OLEDSetPixel ( xc-y,yc-x,c );

		if ( p < 0 ) {
			p = p + 4 * x + 6;
		}

		else {
			p = p + 4 * ( x - y ) + 10;
			y --;
		}

		x++;
	}
}
