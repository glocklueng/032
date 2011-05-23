#include "common.h"


#if USE_SNAKE

/* prototypes */
void draw_line(int col, int row);
void add_segment();
void setup_level();

/* constants */
const int maxrow=18, maxcol=18;
const int snake_start_col=18/2,snake_start_row=18/2;


/* global variables */
int score, snake_length, speed, obstacles, level, high_score=0;
char screen_grid[maxrow][maxcol];

char direction = 0 ;

struct snake_segment {
  int row,col;
} snake[100];

void snake_loop(void)
{	
	unsigned char  row,col;
	char keypress;

	if(buffer[0].keyLeft ) direction = 1;
	if(buffer[0].keyRight ) direction = 2;
	if(buffer[0].keyUp ) direction = 3;
	if(buffer[0].keyDown ) direction = 4;

  /* Variable declarations within main() only */


	// keeps game running
	gCount = 2;

	for(row = 0 ; row < maxrow ; row ++ ) {	
		for(col = 0 ; col < maxcol ; col ++ ) {
			if(screen_grid[row][col]) 
				buffer[0].SetPoint(row,col);
		}
	}
   	
  
      /* If key has been hit, then check it is a direction key - if so,
         change direction */

      /* Add a segment to the end of the snake */
      add_segment();

      /* Blank last segment of snake */
	  buffer[0].ClearPoint(snake[0].col,snake[0].row);

      /* ... and remove it from the array */
        for (int i=1;i<=snake_length;i++)
          snake[i-1]=snake[i];

      for (int i=0;i<=snake_length;i++)
     {
	 	buffer[0].SetPoint(snake[i].col,snake[i].row);
      }
      

      /* Collision detection - walls (bad!) */
      if ((snake[snake_length-1].row>maxrow+1)||(snake[snake_length-1].row<=1)||
          (snake[snake_length-1].col>maxcol+1)||(snake[snake_length-1].col<=1)||
   		/* Collision detection - obstacles (bad!) */
        	  (screen_grid[snake[snake_length-1].row-2][snake[snake_length-1].col-2]=='x'))

       	 gCount=0; /* i.e. exit loop - game over */

      /* Collision detection - snake (bad!) */
      for (int i=0;i<snake_length-1;i++)
        if ( (snake[snake_length-1].row)==(snake[i].row) &&
             (snake[snake_length-1].col)==(snake[i].col))
        {
          gCount=0; /* i.e. exit loop - game over */
          break; /* no need to check any more segments */
        }
      /* Collision detection - food (good!) */
      if (screen_grid[snake[snake_length-1].row-2][snake[snake_length-1].col-2]=='.')
     {
        /* increase score and length of snake */
        score+=snake_length*obstacles;
		 
		 snake_length++; add_segment();
        /* if length of snake reaches certain size, onto next level */
        if (snake_length==(level+3)*2)
       {
          score+=level*1000; obstacles+=2; level++;  /* add to obstacles */
          if ((level%5==0)&&(speed>1)) speed--; /* increase speed every 5 levels */
          setup_level(); /* display next level */
        }
      }

	buffer[0].RefreshAll( 100 );
}



void setup_level()
{
	ClearFrames();
	obstacles=4; level=1; score=0; speed=14;


	/* variables local to setup_level() */
	int row,col;

	/* Set up global variables for new level */
	snake_length=level+4; 

	direction = 2;


	/* Fill grid with blanks */

	memset(screen_grid,0,maxrow*maxcol);

	/* Fill grid with Xs and food */
	for(int i=0;i<obstacles*2;i++)
	{
		row= qrand()%maxrow;
		col= qrand()%maxcol;

		if(i<obstacles)
			screen_grid[row][col]='x';
		else
			screen_grid[row][col]='.';
		}

	/* Create snake array of length snake_length */
	for(int i=0;i<snake_length;i++)
	{
		snake[i].row=snake_start_row;
		snake[i].col=snake_start_col+i;
	}

}


void add_segment()
{

	
    if( direction == 2 ) { 
					snake[snake_length].row=snake[snake_length-1].row;
                     snake[snake_length].col=snake[snake_length-1].col+1;
                     }

    if( direction == 1) {
	                 snake[snake_length].row=snake[snake_length-1].row;
                     snake[snake_length].col=snake[snake_length-1].col-1;
                     }

    if( direction == 3)   {
			 snake[snake_length].row=snake[snake_length-1].row-1;
             snake[snake_length].col=snake[snake_length-1].col;
	}	

    if( direction == 4) {
					snake[snake_length].row=snake[snake_length-1].row+1;
                     snake[snake_length].col=snake[snake_length-1].col;

	}

 
}


#endif
