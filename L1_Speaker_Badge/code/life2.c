#include "common.h"


// Caution: uses framebuffer 0,1, and 2 ( 0 for display, 1 and 2 for intermediate)
// http://www.cs.utexas.edu/users/djimenez/utsa/cs1713-3/c/life.txt

#if USE_LIFE2

/* set everthing to zero */

static unsigned char *board= NULL;

static void initialize_board (void) 
{
	int	i, j;

	//might not be able to get the memory needed
	if( board == NULL) {
		board = (unsigned char *)calloc(PEGGY2_WIDTH*PEGGY2_HEIGHT,sizeof( unsigned char ));
	}

	if( board == NULL )  {
		gCount = 1;
		return;
	}

	for (i=0; i<PEGGY2_WIDTH; i++) {
		for (j=0; j<PEGGY2_HEIGHT; j++) {
			board[i+(PEGGY2_WIDTH*j)] = (qrand()%2)?1:0;
		}
	}
}

/* add to a width index, wrapping around like a cylinder */

static int xadd (int i, int a) 
{
	i += a;
	while (i < 0) i += PEGGY2_WIDTH;
	while (i >= PEGGY2_WIDTH) i -= PEGGY2_WIDTH;
	return i;
}

/* add to a height index, wrapping around */

static int yadd (int i, int a) 
{
	i += a;
	while (i < 0) i += PEGGY2_HEIGHT;
	while (i >= PEGGY2_HEIGHT) i -= PEGGY2_HEIGHT;
	return i;
}

/* return the number of on cells adjacent to the i,j cell */

static unsigned char adjacent_to (int i, int j) 
{
	int	k, l, count;

	count = 0;

	/* go around the cell */

	for (k=-1; k<=1; k++) for (l=-1; l<=1; l++)

		/* only count if at least one of k,l isn't zero */

		if (k || l) {
			if ( board[ xadd(i,k)+(yadd(j,l)*PEGGY2_WIDTH)] ) {
				count++;
			}
		}
	return count;
}

static void play (void)
{
/*
	(copied this from some web page, hence the English spellings...)

	1.STASIS : If, for a given cell, the number of on neighbours is 
	exactly two, the cell maintains its status quo into the next 
	generation. If the cell is on, it stays on, if it is off, it stays off.

	2.GROWTH : If the number of on neighbours is exactly three, the cell 
	will be on in the next generation. This is regardless of the cell's 		current state.

	3.DEATH : If the number of on neighbours is 0, 1, 4-8, the cell will 
	be off in the next generation.
*/
	int	i, j;
	unsigned char a, *newboard;

	// re-use framebuffer memory
	newboard = (unsigned char *)buffer[2].pbuffer;

	/* for each cell, apply the rules of Life */

	for (i=0; i<PEGGY2_WIDTH; i++) 
		for (j=0; j<PEGGY2_HEIGHT; j++) {
			a = adjacent_to ( i, j);
			if (a == 2) newboard[i+(PEGGY2_WIDTH*j)] = board[i+(PEGGY2_WIDTH*j)];
			if (a == 3) newboard[i+(PEGGY2_WIDTH*j)] = 1;
			if (a < 2) newboard[i+(PEGGY2_WIDTH*j)] = 0;
			if (a > 3) newboard[i+(PEGGY2_WIDTH*j)] = 0;
		}

	/* copy the new board back into the old board */

	memcpy(board,newboard,(PEGGY2_WIDTH * PEGGY2_HEIGHT) * sizeof(unsigned char) );

}

/* print the life board */

static void print (void)
{
	int	i, j;

	/* for each row */

	for (j=0; j<PEGGY2_HEIGHT; j++) {

		/* print each column position... */

		for (i=0; i<PEGGY2_WIDTH; i++) {

			if( board[i+(PEGGY2_WIDTH*j)] ){

				buffer[0].SetPoint(i,j);

			}
		}

	}
}


void setup_life2(void )
{
	ClearFrames();

	initialize_board();
	gCount = 100;
}

void loop_life2(void )
{
	if( board == NULL ) {
		return;
	}
	
	buffer[0].Clear();

	play();
	print();

	buffer[0].RefreshAll( 40 );

}

#endif
