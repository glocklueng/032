/**
 * \file glcd.c
 * Graphic LCD handler
 * 
 * Code for interfacing and handling the Nokia 6610/7250 graphic LCD
 * 
 * Mictronics yamppPod firmware
 * Copyright (C) 2008 by Michael Wolf
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

//#include "hardware_conf.h"
//#include "firmware_conf.h"
//#include "trace.h"
//#include "spi.h"
//#include "delay.h"

#include "integer.h"
#include "glcd.h"
#include <proxmark3.h>
#include "apps.h"

//#define EPSON_LCD
#define PHILIPS_LCD

#define DEFAULT_CONTRAST    ( 0xC0 )

/* check which font file should be used, define default */
#if defined(ADVOCUT)
    #include "fonts/AdvoCut.h"
#elif defined(AUXDOTBIT)
    #include "fonts/AuxDotBit.h"
#elif defined(BAUER)
    #include "fonts/Bauer.h"
#elif defined(DEFAULT)
    #include "fonts/Default.h"
#elif defined(FREON)
    #include "fonts/Freon.h"
#elif defined(MS_SANS_SERIF)
    #include "fonts/MS_Sans_Serif.h"
#elif defined(OEM_6x8)
    #include "fonts/OEM_6x8.h"
#elif defined(OEM_8x14)
    #include "fonts/OEM_8x14.h"
#elif defined(SYSTEMATIC)
    #include "fonts/Systematic.h"
#elif defined(TAHOMA)
    #include "fonts/Tahoma.h"
#elif defined(ZELDADX)
    #include "fonts/ZeldaDX.h"
#elif defined(ANSI_8x8)
    #include "fonts/ANSI_8x8.h"
#elif defined(ANSI_8x9)
    #include "fonts/ANSI_8x9.h"        
#else
    #include "../fonts/Default.h"
#endif

/* include font file for time display purpose */
#include "../fonts/Tiny_Time.h"

/**
 * \name Various variables for LCD control
 */
//@{
volatile uint8_t lcd_xpos = 0;       //!< Actual pixel position X axis
volatile uint8_t lcd_ypos = 0;       //!< Actual pixel position Y axis
volatile uint8_t lcd_font_height;    //!< Font set height
volatile uint8_t lcd_font_width;     //!< Font set width
volatile uint8_t lcd_font_firstchar; //!< First char in font set
volatile uint8_t lcd_font_vertical;  //!< Font set vertical byte order
volatile uint8_t lcd_invert;         //!< Flag to indicate use of character inversion
volatile uint16_t lcd_text_color;    //!< Actual LCD text color
volatile uint16_t lcd_back_color;    //!< Actual LCD background color
const char *lcd_font;                //!< Pointer to actual LCD font set
//@}

// Epson S1D15G10 Command Set

#define DISON       0xaf   
#define DISOFF      0xae   
#define DISNOR      0xa6  
#define DISINV      0xa7  
#define COMSCN      0xbb   
#define DISCTL      0xca   
#define SLPIN       0x95   
#define SLPOUT      0x94   
#define PASET       0x75   
#define CASET       0x15   
#define DATCTL      0xbc   
#define RGBSET8     0xce   
#define RAMWR       0x5c   
#define RAMRD       0x5d   
#define PTLIN       0xa8   
#define PTLOUT      0xa9   
#define RMWIN       0xe0   
#define RMWOUT      0xee   
#define ASCSET      0xaa   
#define SCSTART     0xab   
#define OSCON       0xd1   
#define OSCOFF      0xd2   
#define PWRCTR      0x20   
#define VOLCTR      0x81   
#define VOLUP       0xd6   
#define VOLDOWN     0xd7   
#define TMPGRD      0x82   
#define EPCTIN      0xcd   
#define EPCOUT      0xcc   
#define EPMWR       0xfc   
#define EPMRD       0xfd   
#define EPSRRD1     0x7c   
#define EPSRRD2     0x7d   
#define NOP         0x25    

/**
 * Reset LCD
 * 
 * Hardware reset the LCD Display
 * 
*/  
void lcd_reset(void)
{
    spi_com(SPI_LCD_MODE, CMD | LCD_CMD_SWRESET,0);       // Reset
    SpinDelay(1000);
}

void lcd_send(unsigned int data)
{
  unsigned int data_;
  data_ = (data& ~(unsigned int)0x0100);

  // 9th bit set for data, clear for command
  spi_com(SPI_LCD_MODE, data_,0);
}

/**
 * Set Contrast
 * 
 * Set a new Contrast level
 * \param   ctr     New Contrast level
 *      
*/
void lcd_setcontrast(uint8_t ctr)
{
    if(ctr < 20 || ctr > 80)
        ctr = DEFAULT_CONTRAST;
    spi_com(SPI_LCD_MODE, CMD | LCD_CMD_SETCON,0);  // set contrast cmd.
    spi_com(SPI_LCD_MODE, DATA | ctr,1);
}

void spi_command(unsigned int dout)
{

    while ( ( AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TXEMPTY)   == 0); // wait for SPI ready state

    AT91C_BASE_SPI->SPI_TDR = (dout& ~(unsigned int)0x0100);	// send data

 //   while ( !( AT91C_BASE_SPI->SPI_SR & AT91C_SPI_RDRF ) );   // wait for incoming data

}
void spi_data(unsigned int dout)
{

    while ( ( AT91C_BASE_SPI->SPI_SR & AT91C_SPI_TXEMPTY)  ==0 ); // wait for SPI ready state

    AT91C_BASE_SPI->SPI_TDR = dout  |0x0100 ;	// send data

   // while ( !( AT91C_BASE_SPI->SPI_SR & AT91C_SPI_RDRF ) );   // wait for incoming data

}


/**
 * Init LCD
 * 
 * Reset and initialize the LCD and setup the requested colormode.
 * 
*/
void lcd_init(void)
{
  
#ifdef PHILIPS_LCD
  int i;

    spi_com(SPI_LCD_MODE, CMD | LCD_CMD_SWRESET,0);       // Reset
    SpinDelay(100);
    spi_com(SPI_LCD_MODE, CMD | LCD_CMD_SLEEPOUT,0);      // Sleep_out
    spi_com(SPI_LCD_MODE, CMD | LCD_CMD_DISPON,0);        // display on
    spi_com(SPI_LCD_MODE, CMD | LCD_CMD_BSTRON,0);        // booster on

    spi_com(SPI_LCD_MODE, CMD | LCD_CMD_MADCTL,0);        // Memory data access control
    spi_com(SPI_LCD_MODE, DATA | MADCTL_HORIZ,0);         // X Mirror and BGR format (needed)

    spi_com(SPI_LCD_MODE, CMD | LCD_CMD_COLMOD,0);        // Colour mode
    spi_com(SPI_LCD_MODE, DATA | 0x05,0);                 // 16-bit colour mode select

    spi_com(SPI_LCD_MODE, CMD | LCD_CMD_NORON,0);         // Normal mode

    lcd_setcontrast(DEFAULT_CONTRAST);
#elif defined(EPSON_LCD)

#define delay_ms SpinDelay

  SetupSpi(   SPI_LCD_MODE );

  spi_command(DISCTL);  // display control
  spi_data(0x03);
  spi_data(32);
  spi_data(12);
  spi_data(0x00);

  spi_command(COMSCN);  // comscn
    spi_data(0x01);

  spi_command(OSCON);  // oscon

  spi_command(SLPOUT);  // sleep out

  spi_command(VOLCTR);  // electronic volume, this is kinda contrast/brightness
  spi_data(5);//ff);       //  this might be different for individual LCDs
  spi_data(0x01);//01);     //

  spi_command(PWRCTR);  // power ctrl
  spi_data(0x0f);      //everything on, no external reference resistors
  SpinDelay(100);

  spi_command(DISINV);  // display mode


  spi_command(DATCTL);  // datctl
  spi_data(0x00);
  spi_data(0);
  spi_data(0x01);
  spi_data(0x00);
  
  
    spi_command(RGBSET8);   // setup color lookup table
    // color table
    //RED
    spi_data(0);
    spi_data(2);
    spi_data(4);
    spi_data(6);
    spi_data(8);
    spi_data(10);
    spi_data(12);
    spi_data(15);
    // GREEN
    spi_data(0);
    spi_data(2);
    spi_data(4);
    spi_data(6);
    spi_data(8);
    spi_data(10);
    spi_data(12);
    spi_data(15);
    //BLUE
    spi_data(0);
    spi_data(4);
    spi_data(9);
    spi_data(15);


  spi_command(NOP);  // nop

  spi_command(PASET);   // page start/end ram
  spi_data(2);            // for some reason starts at 2
  spi_data(131);          
  
  spi_command(CASET);   // column start/end ram
  spi_data(0);          
  spi_data(131);


  spi_command(RAMWR);    // write some stuff (background)
  for (i = 0; i < 18000; i++){
    spi_data(28);  // 28 is green
  }
  

  spi_command(DISON);   // display on

  delay_ms(200);
  spi_command(RGBSET8);   // setup color lookup table
    // color table
    //RED
    spi_data(0);
    spi_data(2);
    spi_data(4);
    spi_data(6);
    spi_data(8);
    spi_data(10);
    spi_data(12);
    spi_data(15);
    // GREEN
    spi_data(0);
    spi_data(2);
    spi_data(4);
    spi_data(6);
    spi_data(8);
    spi_data(10);
    spi_data(12);
    spi_data(15);
    //BLUE
    spi_data(0);
    spi_data(4);
    spi_data(9);
    spi_data(15);


  spi_command(NOP);  // nop

  spi_command(PASET);   // page start/end ram
  spi_data(2);            // for some reason starts at 2
  spi_data(131);          
  
  spi_command(CASET);   // column start/end ram
  spi_data(0);          
  spi_data(131);


  spi_command(RAMWR);    // write some stuff (background)
  for (i = 0; i < 18000; i++){
    spi_data(28);  // 28 is green
  }
  

  spi_command(DISON);   // display on

  delay_ms(200);

 for (i = 0; i < 160; i++){   // this loop adjusts the contrast, change the number of iterations to get
   spi_command(VOLUP);                // desired contrast.  This might be different for individual LCDs
   delay_ms(2);
 }
// for(;;);
 
#endif
  
    SpinDelay(5); // give LCD some time
    
    // reset pixel pointer
    lcd_xpos = PIXEL_OFFSET;
    lcd_ypos = PIXEL_OFFSET;
    // store font specific variables
    lcd_font_height = flash_font[1];
    lcd_font_width = flash_font[0];
    lcd_font_firstchar = flash_font[3];
    lcd_font_vertical = flash_font[2];
    lcd_font = flash_font;
    // set default colors
    lcd_text_color = WHITE;
    lcd_back_color = BLACK;
}

/**
 * Set LCD window
 * 
 * Set the write window for any following RAM write funtions
 * \param   xs  Left edge of window 
 * \param   ys  Top edge of window 
 * \param   xe  Right edge of window 
 * \param   ye  Bottom edge of window
 *  
*/
void lcd_window(uint8_t xs, uint8_t ys, uint8_t xe, uint8_t ye)
{
    spi_com(SPI_LCD_MODE,CMD | LCD_CMD_CASET,0);
    spi_com(SPI_LCD_MODE,DATA | xs,0);
    spi_com(SPI_LCD_MODE,DATA | xe,0);
    spi_com(SPI_LCD_MODE,CMD | LCD_CMD_PASET,0);
    spi_com(SPI_LCD_MODE,DATA | ys,0);
    spi_com(SPI_LCD_MODE,DATA | ye,1);
}

/**
 * Clear LCD
 * 
 * Clear the whole LCD Display
 * DMA transfer is used to speed things up a lot.
 * 
*/  
void lcd_clear(void)
{
    int nbytes;

    lcd_window(0,0,131,131);
    spi_com(SPI_LCD_MODE, CMD | LCD_CMD_RAMWR,0);       // write memory

    nbytes = 132*132;

    nbytes--;
    while (nbytes--)
    {
        spi_com(SPI_LCD_MODE, DATA | (lcd_back_color>>8),0);
        spi_com(SPI_LCD_MODE, DATA | (lcd_back_color&0xFF),0);
    }
   
    spi_com(SPI_LCD_MODE, CMD | LCD_CMD_MADCTL,0);        // Memory data access control
    spi_com(SPI_LCD_MODE, DATA | MADCTL_HORIZ,1);         // X Mirror and BGR format (needed)
}

/**
 * Set X and Y pixel position
 * 
 * Move pixel pointer to X,Y position
 *
 * \param   x   New X position
 * \param   y   New Y position
 *  
*/
void lcd_gotoxy (uint8_t x, uint8_t y)
{

    lcd_xpos = x + PIXEL_OFFSET;
    lcd_ypos = y + PIXEL_OFFSET;

    if (lcd_xpos > WINDOW_WIDTH)
        lcd_xpos = WINDOW_WIDTH;
    if (lcd_ypos > WINDOW_HEIGHT)
        lcd_ypos = WINDOW_HEIGHT;
        
}

/**
 * Move to new line
 * 
 * Move pixel pointer to new text line
 *  
*/
void lcd_newline(void)
{

    lcd_xpos = PIXEL_OFFSET;
    if (lcd_ypos < 126)
        lcd_ypos += lcd_font_height;
}

/**
 * Clear line
 * 
 * Clear one line on lcd
 * 
 */
void lcd_clrline(uint8_t ypos)
{

    lcd_gotoxy (PIXEL_OFFSET, ypos * lcd_font_height);
    for (uint8_t i = 0; i < 15; i++) {
        lcd_putchar (' ');
    }
    lcd_gotoxy (PIXEL_OFFSET, ypos * lcd_font_height);
}

/**
 * Print string
 * 
 * Print string on LCD
 * 
 * \param   string  Text string
 * 
 */
void lcd_puts(const char * string)
{
    while (*string)
        lcd_putchar (*string++);
}

/**
 * Print one character on LCD
 * 
 * Print one character on LCD
 * 
 * \param   data    Character to print
 * 
 */
void lcd_putchar (char data)
{
/*
    // set LF/CR on column overflow
    if ((lcd_xpos + *(lcd_font+0)) >= WINDOW_WIDTH)
        lcd_newline ();
    // set HOME on row overflow
    if ((lcd_ypos + *(lcd_font+1)) > (WINDOW_HEIGHT + 2))
        lcd_ypos = PIXEL_OFFSET;
*/                
    switch (data) {
            // set LF
        case '\n':
            lcd_newline ();
            break;
            // set CR
        case '\r':
            lcd_xpos = PIXEL_OFFSET;
            break;
        case '\t':
            // use \t to swap normal and time font set
            if(lcd_font == flash_font) lcd_font = time_font;
            else lcd_font = flash_font;
            break;        
            // print char by default
        default:
            //data -= lcd_font_firstchar; // subtract offset to first char
            data -= *(lcd_font+3);
            if((signed char)data < 0) data = *(lcd_font+4); // on error use first char in font

            // set RAM window to print character
            lcd_window (lcd_xpos, lcd_ypos, lcd_xpos + (*(lcd_font+0) - 1), lcd_ypos + (*(lcd_font+1) - 1)); // set clearance window

            spi_com(SPI_LCD_MODE, CMD | LCD_CMD_RAMWR,0);    // write memory
      
            uint8_t temp_char;
            // write LCD_FONT_HEIGT*LCD_FONT_WIDTH bytes to LCD RAM
            for (uint8_t y = 0; y < *(lcd_font+1); y++) {
                // read font data from FLASH look-up table
                // +4 offset for font config bytes
                temp_char = *(lcd_font+((data * *(lcd_font+1)) + y + 4));

                if (lcd_invert)
                    temp_char ^= 0xFF;

                for (uint8_t x = 0; x < *(lcd_font+0); x++) {
                    // check if pixel should be set or cleared
                    if ((temp_char & (1<<7)) == (1<<7))
                    {
                        spi_com(SPI_LCD_MODE, DATA | lcd_text_color>>8,0);
                        spi_com(SPI_LCD_MODE, DATA | lcd_text_color,0);
                    }
                    else
                    {
                        spi_com(SPI_LCD_MODE, DATA | lcd_back_color>>8,0);
                        spi_com(SPI_LCD_MODE, DATA | lcd_back_color,0);
                    }
                    temp_char <<= 1;    // next pixel
                }
            }
            lcd_xpos += *(lcd_font+0);    // next char
    }   // end switch
    spi_com(SPI_LCD_MODE, CMD | LCD_CMD_MADCTL,0);        // Memory data access control
    spi_com(SPI_LCD_MODE, DATA | MADCTL_HORIZ,1);         // X Mirror and BGR format (needed)
}

/**
 * Set LCD text colors
 * 
 * Set background and text colors.
 * 
 * \param   back    Background color
 * \param   text    Text color
 * 
 */
void lcd_setcolor(uint16_t back, uint16_t text)
{
    lcd_back_color = back;
    lcd_text_color = text;
}

/**
 * Shut down the LCD display
 *  
*/
void lcd_off(void)
{
    lcd_clear();
    spi_com(SPI_LCD_MODE, CMD | LCD_CMD_DISPOFF,0);   // display off
    spi_com(SPI_LCD_MODE, CMD | LCD_CMD_BSTROFF,0);   // booster off
    spi_com(SPI_LCD_MODE, CMD | LCD_CMD_SLEEPIN,1);   // put LCD in sleep mode
    SpinDelay(10);
}   

