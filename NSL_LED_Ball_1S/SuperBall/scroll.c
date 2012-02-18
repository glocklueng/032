#include "SuperBall.h"

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
	textXPos[0] = 8;

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
			textXPos[i] =18;
		}
	}	
}

