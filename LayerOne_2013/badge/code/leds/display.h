/* ----------------------------------------------------------------------------
	128x64 Graphic LCD management for SSD1306 driver

	FILE NAME 	: display.h

	DESCRIPTION	: The purpose of this function is to manage a graphic LCD
			  by providing function for control and display text and graphic

	AUTHOR		: Gabriel Anzziani
    www.gabotronics.com

*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#define Display     GPIO7   // Display options

// Display bits     (GPIO7) // Display options
#define grid0       0       // Grid settings (2 bits)
#define grid1       1
#define elastic     2       // Average on successive traces
#define disp_inv    3       // Invert display
#define flip        4       // Flip display
#define persistent  5       // Persistent Display
#define line        6       // Continuous Drawing
#define showset     7       // Show scope settings (time/div volts/div)

#define Misc        GPIOC   // Miscellaneous bits

// Misc             (GPIOC) // Miscellaneous bits
#define keyrep      0       // Automatic key repeat
#define negative    1       // Print Negative font
#define bigfont     2       // Small or large font, this bit is normally 0
#define fullprint   2       // Small or large font, this bit is normally 0
#define redraw      3       // Redraw screen
#define sacquired   4       // Data has been acquired (for slow sampling)
#define slowacq     5       // Acquired one set of samples a slow sampling rates
#define lowbatt     6       // VCC is below 3.15V
#define autosend    7       // Continuously send data to UART


#define MENUPULL 0x18

#define     LCD_RESET       1           // RESET
#define		LCD_RS		    3           // Data / Instruction
#define		LCD_CS		    0           // Chip Select
#define		LCD_CTRL        VPORT0.OUT

// commands SSD1306 controller
#define LCD_SET_COL_HI		0x10
#define LCD_SET_COL_LO		0x00
#define LCD_SET_LINE		0x40
#define LCD_SET_CONTRAST	0x81
#define LCD_SET_SEG_REMAP0  0xA0
#define LCD_SET_SEG_REMAP1	0xA1
#define LCD_EON_OFF			0xA4
#define LCD_EON_ON			0xA5
#define LCD_DISP_NOR		0xA6
#define LCD_DISP_REV		0xA7
#define LCD_MULTIPLEX       0xA8
#define LCD_CHARGE_PUMP    	0x8D
#define LCD_PUMP_OFF    	0x10
#define LCD_PUMP_ON     	0x14
#define LCD_DISP_OFF 		0xAE
#define LCD_DISP_ON			0xAF
#define LCD_SET_PAGE		0xB0
#define LCD_SET_SCAN_FLIP	0xC0
#define LCD_SET_SCAN_NOR	0xC8
#define LCD_SET_OFFSET		0xD3
#define LCD_SET_RATIO_OSC	0xD5
#define LCD_SET_CHARGE  	0xD9
#define LCD_SET_PADS    	0xDA
#define LCD_SET_VCOM    	0xDB
#define LCD_NOP     		0xE3
#define LCD_SCROLL_RIGHT	0x26
#define LCD_SCROLL_LEFT		0x27
#define LCD_SCROLL_VR	    0x29
#define LCD_SCROLL_VL		0x2A
#define LCD_SCROLL_OFF		0x2E
#define LCD_SCROLL_ON   	0x2F
#define LCD_SCROLL_ON   	0x2F
#define LCD_VERT_SCROLL_A  	0xA3
#define LCD_MEM_ADDRESSING 	0x20
#define LCD_SET_COL_ADDR	0x21
#define LCD_SET_PAGE_ADDR	0x22

extern const uint8_t BigFonts[];
extern uint8_t display_buffer[1024];
extern uint8_t u8CursorX, u8CursorY;
extern const int8_t tdown[];
extern const int8_t tup[];
extern const int8_t tdual[];

#define lcd_goto(x,y) { u8CursorX=(x); u8CursorY=(y); }

/* EXTERN Function Prototype(s) */
void GLCD_LcdInit (void);
void GLCD_setting(void);
void GLCD_LcdOff(void);
void GLCD_Print (const char *);
void lcd_putsp(const char *);
void lcd_puts (const char *);
void GLCD_Putchar (char);
void printN(uint8_t Data);
void clr_display(void);
void clr_menu(void);
void show_display(void);
void set_pixel(uint8_t x, uint8_t y);
void sprite(uint8_t x, uint8_t y, const int8_t *ptr);
void lcd_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void write_display(uint8_t data);
void zero_display();
void printV(int16_t Data, uint8_t gain);
void printF(uint8_t x, uint8_t y, int32_t Data);
void tiny_printp(uint8_t x, uint8_t y, const char *ptr);
void LcdInstructionWrite (unsigned char);

#endif