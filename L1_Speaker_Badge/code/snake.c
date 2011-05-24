#include "common.h"

// requires frame buffer 3 !

/*                                                          
											 ad88888ba                            88                    
											d8"     "8b                           88                    
											Y8,                                   88                    
											`Y8aaaaa,    8b,dPPYba,   ,adPPYYba,  88   ,d8   ,adPPYba,  
											  `"""""8b,  88P'   `"8a  ""     `Y8  88 ,a8"   a8P_____88  
											        `8b  88       88  ,adPPPPP88  8888[     8PP"""""""  
											Y8a     a8P  88       88  88,    ,88  88`"Yba,  "8b,   ,aa  
										 	 "Y88888P"   88       88  `"8bbdP"Y8  88   `Y8a  `"Ybbd8"'  
*/

#if USE_SNAKE

/* prototypes */
static void add_segment(void);
static void setup_level(void);


// only allows for 15x15 but thats ok

// quick access macro to extract row/col
#define	ROW(i)				((snake[i].row_col&0xf0)>>4)
#define COL(i)			  	( snake[i].row_col&0x0f)
#define SETRC(i,r,c)  		( snake[i].row_col = ( (r)<<4) + (c) )

/* global variables */
static unsigned short score, snake_length, speed, obstacles, level;

static char direction = 0 ;
static unsigned char counter =0, timer =0 ;

static const unsigned char PROGMEM tFIN[] = "FIN";
static const unsigned char PROGMEM tBYE[] = "BYE";
static const unsigned char PROGMEM tSNAKE[] = "SNAKE";
static const PROGMEM unsigned char text1[] = "NSL";

// needs 200 bytes... (combine into nibbles = frame buffer size, so reuse buffer 3)
static struct snake_segment {
	unsigned char row_col;
} *snake;


static void frame(void);

/*
 * Clears screen 0, Draws the text and keeps it for a short time, then Clears it again
 *
 */
void banner( const unsigned char *text )
{
	buffer[0].Clear();
	Text6x6(2,7,(const unsigned char *)text);
	buffer[0].RefreshAll( 1400 );
	buffer[0].Clear();
}

void snake_loop(void)
{	
	// check keys ( only in Buffer[0] ) 
	if( buffer[0].keyLeft ) { direction = 1;gCount+=10;}
	if( buffer[0].keyRight ){ direction = 2;gCount+=10;}
	if( buffer[0].keyUp )   { direction = 3;gCount+=10;}
	if( buffer[0].keyDown ) {direction = 4;gCount+=10;}
 
	// quit
	if( buffer[0].keyA ) { 
		gCount = 0;
		banner(tBYE);
	}

	/* Add a segment to the end of the snake */
	add_segment();

	/* Blank last segment of snake */
	buffer[0].ClearPoint( COL(0), ROW(0) );

	/* ... and remove it from the array */
	for (unsigned short i=1;i<=snake_length;i++){
		snake[i-1] = snake[i];
	}

	for (unsigned short i=0;i<=snake_length;i++) {
		buffer[0].SetPoint(COL(i),ROW(i));
	}


	/* Collision detection - walls (bad!) */
	if ((ROW(snake_length-1) > ( PEGGY2_HEIGHT+1) )|| (ROW(snake_length-1) < 1 ) || 
		(COL(snake_length-1) > ( PEGGY2_WIDTH+1) ) || (COL(snake_length-1) < 1 )) {

			gCount=0; /* i.e. exit loop - game over */
				
			banner(tFIN);
			
	}

	/* Collision detection - snake (bad!) */
	for (unsigned int i=0;i<snake_length-1;i++)
		if ( (ROW(snake_length-1) ) == (ROW(i)) && ( COL(snake_length-1) ) == (COL(i) ) )
		{
			gCount=0; /* i.e. exit loop - game over */
			break; /* no need to check any more segments */
		}

	counter ++;

	// every few frames, add a segment
	if (counter == 15 ) {

		counter = 0;

		timer = 8;

		/* increase score and length of snake */
		score+=snake_length*obstacles;

		snake_length++; add_segment();

		/* if length of snake reaches certain size, onto next level */
		if (snake_length==(level+3)*2)
		{
			score+=level*1000; obstacles+=2; level++;  /* add to obstacles */

			if ( (level%5 == 0 ) && (speed>1) ) {

				speed--; 								/* increase speed every 5 levels */
			}	
		}
	}

	if( timer > 0 ) {

			timer --;

		if( buffer[0].GetPoint(level-1,PEGGY2_HEIGHT-1 ) )
			buffer[0].ClearPoint(level-1,PEGGY2_HEIGHT-1);
		else
			buffer[0].SetPoint(level-1,PEGGY2_HEIGHT-1);

		if( timer == 0 ) {
			buffer[0].SetPoint(level-1,PEGGY2_HEIGHT-1);
		}

	}
	
	//Draw frame buffer, helps control game speed too
	buffer[0].RefreshAll( 100 );
}

void init_snake(void)
{	
	int x = 60,x1=-30;

	ClearFrames();

	while(x--) {

		buffer[0].Clear();

		Text6x6(x-(60-18),3,(const unsigned char *)tSNAKE);
		Text6x6(x1,8,(const unsigned char *)text1);

		x1++;

		buffer[0].RefreshAll( 40 );
	}

	buffer[0].Clear();

	// reuse framebuffer 3 as snake data
	snake = (snake_segment*) buffer[3].pbuffer;

	obstacles=4; level=1; score=0; speed=14;
	setup_level();
}


static void setup_level(void)
{
	// only clear buffer 1
	buffer[0].Clear();

	
	/* Set up global variables for new level */
	snake_length=level+4; 

	// start off going right
	direction = 2;

	
	/* Create snake array of length snake_length */
	for(unsigned short i=0;i<snake_length;i++)
	{
		snake[i].row_col =  2<<4;
		snake[i].row_col += (1+i);
	}

	frame();

}

static void frame(void)
{
	for(unsigned char x = 0; x < PEGGY2_WIDTH-1 ; x++ ) {

		buffer[0].SetPoint(x,0);
		buffer[0].SetPoint(x,PEGGY2_HEIGHT-2);
	}

	for(unsigned char y = 0; y < PEGGY2_HEIGHT-1 ; y++ ) {

		buffer[0].SetPoint(0,y);
		buffer[0].SetPoint(PEGGY2_WIDTH-2,y);
	}

	for(unsigned char x = 0; x < level; x++ ) {

		buffer[0].SetPoint(x,PEGGY2_HEIGHT-1);
	}
}


/* adds a segment dependant on the direction of travel */

static void add_segment(void)
{
	switch( direction) { 

		case 2:
			
			SETRC(snake_length, ROW(snake_length-1), COL(snake_length-1)+1 );
			break;

		case 1:

			SETRC(snake_length, ROW(snake_length-1), COL(snake_length-1)-1 );
			break;

		case 3:	

			SETRC(snake_length, ROW(snake_length-1)-1, COL(snake_length-1) );
			break;
	
		case 4:

			SETRC(snake_length, ROW(snake_length-1)+1, COL(snake_length-1) );
			break;
	}
}


#endif
