#include "common.h"


#if USE_SNAKE

/* prototypes */
static void add_segment(void);
static void setup_level(void);

/* global variables */
static int score, snake_length, speed, obstacles, level, high_score=0;

static char direction = 0 ;
static unsigned char counter =0;

// 200 bytes...
static struct snake_segment {
	unsigned char row,col;
} snake[100];

void banner( const  char *text)
{
	buffer[0].Clear();
	Text8x6(0,8,(const unsigned char *)text);
	buffer[0].RefreshAll( 1500 );
	buffer[0].Clear();
}

void snake_loop(void)
{	
	if(buffer[0].keyLeft ){ direction = 1;}
	if(buffer[0].keyRight ){ direction = 2;}
	if(buffer[0].keyUp ){ direction = 3;}
	if(buffer[0].keyDown ) {direction = 4;}

	// quit
	if(buffer[0].keyA ){ 
		gCount = 0;
		banner("BYE");
	}

	/* Add a segment to the end of the snake */
	add_segment();

	/* Blank last segment of snake */
	buffer[0].ClearPoint(snake[0].col,snake[0].row);

	/* ... and remove it from the array */
	for (int i=1;i<=snake_length;i++){
		snake[i-1]=snake[i];
	}

	for (int i=0;i<=snake_length;i++) {
		buffer[0].SetPoint(snake[i].col,snake[i].row);
	}


	/* Collision detection - walls (bad!) */
	if ((snake[snake_length-1].row>PEGGY2_HEIGHT+1)||(snake[snake_length-1].row<1)||
		(snake[snake_length-1].col>PEGGY2_WIDTH+1)||(snake[snake_length-1].col<1)) {

			gCount=0; /* i.e. exit loop - game over */
				
			banner("FIN");
			
		}

	/* Collision detection - snake (bad!) */
	for (int i=0;i<snake_length-1;i++)
		if ( (snake[snake_length-1].row)==(snake[i].row) &&
			(snake[snake_length-1].col)==(snake[i].col))
		{
			gCount=0; /* i.e. exit loop - game over */
			break; /* no need to check any more segments */
		}

		counter ++;

		// every few frames, add a segment
		if (counter == 15 )
		{

			counter = 0;

			/* increase score and length of snake */
			score+=snake_length*obstacles;

			snake_length++; add_segment();
			/* if length of snake reaches certain size, onto next level */
			if (snake_length==(level+3)*2)
			{
				score+=level*1000; obstacles+=2; level++;  /* add to obstacles */
				if ((level%5==0)&&(speed>1)) speed--; /* increase speed every 5 levels */
				
				buffer[0].Clear();
				Text8x6(0,8,(const unsigned char*)"UP!");
				buffer[0].RefreshAll( 1500 );
				buffer[0].Clear();
			}
		}

		buffer[0].RefreshAll( 100 );
}

void init_snake(void)
{
	ClearFrames();
	obstacles=4; level=1; score=0; speed=14;
}


void setup_level(void)
{
	ClearFrames();

	/* Set up global variables for new level */
	snake_length=level+4; 

	// go right
	direction = 2;
	/* Create snake array of length snake_length */
	for(int i=0;i<snake_length;i++)
	{
		snake[i].row=18/2;
		snake[i].col=1+i;
	}

}


static void add_segment(void)
{
	switch( direction) { 

		case 2:
			snake[snake_length].row=snake[snake_length-1].row;
			snake[snake_length].col=snake[snake_length-1].col+1;
			break;
		case 1:
			snake[snake_length].row=snake[snake_length-1].row;
			snake[snake_length].col=snake[snake_length-1].col-1;
			break;
		case 3:	
			snake[snake_length].row=snake[snake_length-1].row-1;
			snake[snake_length].col=snake[snake_length-1].col;
			break;
	
		case 4:
			snake[snake_length].row=snake[snake_length-1].row+1;
			snake[snake_length].col=snake[snake_length-1].col;
			break;
	}
}


#endif
