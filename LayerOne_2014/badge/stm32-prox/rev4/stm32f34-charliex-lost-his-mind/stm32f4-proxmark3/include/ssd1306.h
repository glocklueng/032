#ifndef _SSD1306_H
#define _SSD1306_H

#define SSD1306_SETCONTRAST             0x81
#define SSD1306_DISPLAYALLON_RESUME     0xA4
#define SSD1306_DISPLAYALLON            0xA5
#define SSD1306_NORMALDISPLAY           0xA6
#define SSD1306_INVERTDISPLAY           0xA7
#define SSD1306_DISPLAYOFF              0xAE
#define SSD1306_DISPLAYON               0xAF

#define SSD1306_SETDISPLAYOFFSET        0xD3
#define SSD1306_SETCOMPINS              0xDA

#define SSD1306_SETVCOMDETECT           0xDB

#define SSD1306_SETDISPLAYCLOCKDIV      0xD5
#define SSD1306_SETPRECHARGE            0xD9
#define SSD1306_ENABLE_CHARGE_PUMP      0x8D

#define SSD1306_SETMULTIPLEX            0xA8
#define SSD1306_SETSTARTLINE            0x40

#define SSD1306_MEMORYMODE              0x20
#define SSD1306_HV_COLUMN_ADDRESS       0x21
#define SSD1306_HV_PAGE_ADDRESS         0x22
#define SSD1306_PAM_PAGE_START          0xB0

#define SSD1306_COMSCANINC              0xC0
#define SSD1306_COMSCANDEC              0xC8

#define SSD1306_SEGREMAP                0xA0

#define SSD1306_CHARGEPUMP              0x8D

#define SSD1306_EXTERNALVCC             0x1
#define SSD1306_SWITCHCAPVCC            0x2

// Scrolling #defines
#define SSD1306_SCROLL_ACTIVATE                         0x2F
#define SSD1306_SCROLL_DEACTIVATE                       0x2E
#define SSD1306_SCROLL_SET_VERTICAL_SCROLL_AREA         0xA3
#define SSD1306_SCROLL_HORIZONTAL_RIGHT                 0x26
#define SSD1306_SCROLL_HORIZONTAL_LEFT                  0x27
#define SSD1306_SCROLL_VERTICAL_AND_HORIZONTAL_RIGHT    0x29
#define SSD1306_SCROLL_VERTICAL_AND_HORIZONTAL_LEFT     0x2A
// commands SSD1306 controller
#define LCD_SET_COL_HI      0x10
#define LCD_SET_COL_LO      0x00
#define LCD_SET_LINE        0x40
#define LCD_SET_CONTRAST    0x81
#define LCD_SET_SEG_REMAP0  0xA0
#define LCD_SET_SEG_REMAP1  0xA1
#define LCD_EON_OFF         0xA4
#define LCD_EON_ON          0xA5
#define LCD_DISP_NOR        0xA6 // invert display (normal)
#define LCD_DISP_REV        0xA7 // invert display
#define LCD_MULTIPLEX       0xA8
#define LCD_CHARGE_PUMP     0x8D
#define LCD_PUMP_OFF        0x10
#define LCD_PUMP_ON         0x14
#define LCD_DISP_OFF        0xAE
#define LCD_DISP_ON         0xAF
#define LCD_SET_PAGE        0xB0
#define LCD_SET_SCAN_FLIP   0xC0
#define LCD_SET_SCAN_NOR    0xC8
#define LCD_SET_OFFSET      0xD3
#define LCD_SET_RATIO_OSC   0xD5
#define LCD_SET_CHARGE      0xD9
#define LCD_SET_PADS        0xDA
#define LCD_SET_VCOM        0xDB
#define LCD_NOP             0xE3
#define LCD_SCROLL_RIGHT    0x26
#define LCD_SCROLL_LEFT     0x27
#define LCD_SCROLL_VR       0x29
#define LCD_SCROLL_VL       0x2A
#define LCD_SCROLL_OFF      0x2E
#define LCD_SCROLL_ON       0x2F
#define LCD_SCROLL_ON       0x2F
#define LCD_VERT_SCROLL_A   0xA3
#define LCD_MEM_ADDRESSING  0x20
#define LCD_SET_COL_ADDR    0x21
#define LCD_SET_PAGE_ADDR   0x22

void InitOLED ( void );
void FlushOLED ( void );
void OLEDInit ( void );
void OLEDDrawGraph ( void );
void OLEDSetCursor ( int x,int y );
void OLEDLine ( int x0, int y0, int x1, int y1,int col );
void OLEDText8x6 ( short x,short y,const char *string,unsigned char f_colour, unsigned char b_colour );
void OLEDText6x6 ( short x,short y,const char *string,char f_colour,char b_colour );
void OLEDPixelQuad ( int x, int y, unsigned char colour );
void OLEDFlatLine ( unsigned char colour,short x,unsigned short length,int y );
void OLEDDrawCircle ( int xc,int yc, unsigned int radius , unsigned char c );
void OLEDDrawOutlineCircle ( int xc,int yc, unsigned int radius , unsigned char c );
void OLEDOutlineRect ( unsigned char colour, int x,int y, int width,int height );
void OLEDTest(void);

#endif /* _SSD1306_H */