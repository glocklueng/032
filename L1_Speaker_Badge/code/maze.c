/*
* Title:   Simple Maze
* Author:  Clay Cowgill
* Version: 1.0
* Date: 9-23-2008
*
* The first(?) 3rd-party Uzebox software? ;-)
*
* License: Uhhh... Do whatever you want with it.  If you're injured as a result-- not my fault.
*
* Follow Uze's tutorial for how to build a "Hello, world" demo.
* http://www.belogic.com/uzebox/tutorials.htm
*
* Use the contents of this file for the "HelloWorld.c" and place
* 'mazetiles.c' in the same directory as your project.
*
*  includes an adaptation of the Maze generator 
*  written by Michael Kircher  2008-09-11
*/

/* adapted for layerOne badge, may not have had time to finish it :) */

#include "common.h"

#if USE_MAZE

/* X_SIZE and Y_SIZE should be odd, and greater than 5 */ 
#define X_SIZE (18-1)
#define Y_SIZE (18-1)

#define BLANK 0 // these defines were used in testing a 'text' version of the game
#define WALL 'a'  // prior to the 'real' graphics.  No real need to change them.
#define PLAYER '*'
#define EXIT 'O'

signed char dx[4]={1,0,-1,0};  // used by maze generator to choose directions
signed char dy[4]={0,-1,0,1}; 

char maze[Y_SIZE][X_SIZE+1]; 

int drill(int *ay, int *ax, int j)  // punches a path through the Maze to make is solveable
{ 
	int by,bx,c=j; 
	do 
	{ 
		bx=*ax+2*dx[j]; 
		by=*ay+2*dy[j]; 
		if(WALL == maze[by][bx]) 
		{ 
			maze[by][bx]=j; 
			maze[*ay+dy[j]][*ax+dx[j]]=BLANK; 
			*ax=bx; 
			*ay=by; 
			/* drill successful, ask for a new random direction */ 
			return(1); 
		} 
		j=(j+1)*(j<3); 
	} while(j!=c); 
	/* ended up in a blind lane */ 
	return(0); 
} 

// Redraw refreshes the screen display to the user
// in should be called about 30 times a second.

static void SetTile( char x, char y, char v )
{
	if( v ) {
		buffer[0].SetPoint(x,y);
	}
}

void redraw( void )
{
	int ay,ax; 


	for (ay=0;ay<Y_SIZE-1;ay++) {  // go over the maze array depth
		for (ax=0;ax<X_SIZE;ax++) { // ...and across the width 
			
			switch (maze[ay][ax])
			{
				case WALL:
					SetTile(ax+3,ay,0); break;  // for each case, check to see what the symbol in the maze is,
				case BLANK:                     // then translate it to a tile from 'mazetiles[]'.  The numbers 
					SetTile(ax+3,ay,5); break;  // are direct tile numbers as seen in Tile Studio
				case EXIT:
					SetTile(ax+3,ay,10); break;
				case PLAYER:
					SetTile(ax+3,ay,7); break;
				default: 
					SetTile(ax+3,ay,2); break;
			};
		}
	}


	// redraw display
	buffer[0].RefreshAll(1500);

}

static	unsigned char player_x, player_y;
static	unsigned char exit_x=0, exit_y=0;


void setup_maze( void )
{
	int ay,ax,j; 
	unsigned char corner;

	ClearFrames();

	for(ay=0; ay<Y_SIZE; ay++) 
	{ 
		for(ax=0; ax<X_SIZE; ax++) 
			if(ax>0 && ay>0 && ax<X_SIZE-1 && ay<Y_SIZE-1) // fills the maze with walls, that are then 'drilled' out
				maze[ay][ax]=WALL; 
			else 
				maze[ay][ax]=BLANK; 
	} 

	ay=2; // 2,2 is the top left corner of the maze
	ax=2; 
	maze[ay][ax]=4; // starting state for the maze generator

	do 
	{ 
		while(drill(&ay,&ax,(int)(qrand()&0x03))) {}  // start drilling things out (using random)
		j=maze[ay][ax];                              // note that the first time the game runs, the maze will always be 
		maze[ay][ax]=BLANK;                          // the same because of the random number generator always starting
		if(j<4)                                      // in the same state.  (Normally I will 'sum' the contents of RAM
		{                                            // on power-up to make a random seed, but I don't know how to mess
			/* back-track */                           // with WinAVR's C-startup yet.)
			ax=ax-2*dx[j]; 
			ay=ay-2*dy[j]; 
		} 
	} while(j<4); 

	do{                                             // This generates a random position inside the maze to start the
		do player_x=qrand();                          // player.  Seems more interesting than a corner.
		while ((player_x>=X_SIZE-1)||(player_x<=1));
		do player_y=qrand();
		while ((player_y>=Y_SIZE-1)||(player_y<=1));
	}       
	while (maze[player_y][player_x]==WALL);         // (repeats until it picks a random spot that's not a wall)

	maze[player_y][player_x]=PLAYER;                // place the player in the maze

	do                                              // here we choose a random number from 0-3, representing one of the
	{                                            // four corners of the maze.
		corner=qrand()&0x03;
		switch (corner)                              
		{
			case 0:
				exit_x=2; exit_y=2; break;
			case 1:
				exit_x=X_SIZE-3; exit_y=2; break;
			case 2:
				exit_x=X_SIZE-3; exit_y=Y_SIZE-3;break;
			case 3:
				exit_x=2; exit_y=Y_SIZE-3; break;
		}
	}

	while (ABS(player_x-exit_x)<16);                // we compare the distance from the player to the exit (horizontally)
	
	
	// and only accept the corner if it's relatively far away from the player
	// (could use both x and y distance for better effect)
	
	maze[exit_y][exit_x]=EXIT;                   // place the exit in the maze

	redraw();                                    // draw the screen for the player

}


void loop_maze(void)
{
	// This is what happens when you turn a static demo into a 'game'
	// For a real game, you probably don't want to do this, but it works here and is simple...


	if (buffer[0].keyDown||buffer[0].keyUp||buffer[0].keyLeft||buffer[0].keyRight)       // if the D-pad has a bit or more pressed
	{
		maze[player_y][player_x]=BLANK;            // erase the old player position
		if (buffer[0].keyDown)                     // down?
			if (maze[player_y+1][player_x]!=WALL)  // don't move if a wall is blocking your path
				player_y++;                        // you could 'switch/case' these with breaks to easily disable diagonal moves...
		if (buffer[0].keyUp)                       // up?
			if (maze[player_y-1][player_x]!=WALL)
				player_y--;
		if (buffer[0].keyLeft)                     // left?
			if (maze[player_y][player_x-1]!=WALL)
				player_x--;
		if (buffer[0].keyRight )                   // right
			if (maze[player_y][player_x+1]!=WALL)
				player_x++;
		
		if (maze[player_y][player_x]==EXIT)        // if the new position is the EXIT, you just won
		{
			maze[exit_y][exit_x]=PLAYER;           // place the player on top of the exit
			redraw();                              // draw the screen for the user

			gCount = 0;
			return;
		}
	
		maze[player_y][player_x]=PLAYER;           // after the movement calculations, put the player in the new spot
	}

	redraw();    // redraw the screen to update new player position
} 

#endif
