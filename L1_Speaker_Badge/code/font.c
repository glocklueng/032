#include "common.h"

#if USE_SCROLLER

static short textXPos[3] , textLength[3] ;;
static const unsigned char text[] =  "    LAYERONE 2011 - SPEAKER BADGE";
static const unsigned char text1[] = "    NULLSPACE LABS";
static const unsigned char text2[] = "    032.LA DESIGN BY KRS TWSS";

void setup_scroll(void)
{
	// Start off edge of display
	textXPos[0] = -5;
	textXPos[1] = -5;
	textXPos[2] = -5;

	buffer[0].Clear();

	// pixel text length + a bit
	textLength[0] = ( pstrlen( text  ) * 9 ) + 18;
	textLength[1] = ( pstrlen( text1 ) * 9 ) + 18;
	textLength[2] = ( pstrlen( text2 ) * 9 ) + 18;

	gCount = 500;
}

void loop_scroll(void)
{
	unsigned char i;

	buffer[0].Clear();

	Text8x6(textXPos[0], 0,text);

	Text8x6(textXPos[1], 6,text1);

	Text8x6(textXPos[2],12,text2);

	buffer[0].RefreshAll( 45 );

	for ( i = 0 ;i  < 3;  i++ ){

		textXPos[i] -= (i+1) ;

		if ( textXPos[i] < -textLength[i]  )  {
			textXPos[i] = -5;
		}
	}	
}

#if !USE_FONT
#undef USE_FONT
#define USE_FONT	 (1)
#endif


#endif


#if USE_FONT


/* 
 * 8x6 font
 */

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

unsigned short pstrlen(const unsigned char * str)
{
    const unsigned char *s;
    for (s = str; *s; ++s);
    return(s - str);
}


/** 
 * Draw an 8 bit string using the builtin 8x6 font (this font is always available)
 *
 * @param x     - X position on screen, will be clipped.
 * @param y     - Y position on screen, will be clipped.
 * @param string- 8 bit ASCIZ string
 */
void Text8x6(short x,short y,const unsigned char *string)
{
	short nm,i,j,oldx=x;
	unsigned char  cur;
	int l_adr;
	int l=0;

	if( x == -1 ) {
		// center it
		x =  ( (PEGGY2_WIDTH/2)-((pstrlen(string) * 7)/2) );
	}
	

	// off right side
	if( x > PEGGY2_WIDTH ) return;

	//completely off top edge
	if( y < -7  ) return;
	
	// off bottom edge
	if( y > PEGGY2_HEIGHT ) return;

	while(*string!='\0')										/* until the end of the string */
	{
		if(*string=='\n'){
			x=oldx;y+=7;
			l=0;
			string++;
		}
		
		nm=((*string++)-' ');									/* Starting from space */
		
		if( nm > 58 ) 
			nm-=' ';											/* to uppercase 58='Z'-' ' */
		nm *= 6;												/* 6 bytes per char */

		for(i=0,l_adr=0;i<6;i++,l_adr++)
		{
			cur=font8x6[nm++];

			for(j=0;j<8;j++) {
				if(cur&(0x80>>j)) {
					if (( x+j+l) >= 0 && (x+j+l) < (PEGGY2_WIDTH-1))
						if( (y+i) >= 0 && (y+i) < PEGGY2_HEIGHT-1 )
							buffer[0].SetPoint(x+j+l,y+i);
				}
			}
		}
              
		l+=7;/* Next word */
	}
}

#endif
