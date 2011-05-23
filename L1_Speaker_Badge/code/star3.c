#include "common.h"

#if USE_STARFIELD3


#define NUMSTARS			( 32 )


int        starx[NUMSTARS], stary[NUMSTARS], starz[NUMSTARS];
const int    c=8;


void init_star3( void )
{
	int i;

	// Init the starfield

    for ( i =0 ; i < NUMSTARS; i++)
    {
      starx[i]=c*(int)(qrand()*PEGGY2_WIDTH-PEGGY2_WIDTH/2);
      stary[i]=c*(int)(qrand()*PEGGY2_HEIGHT*2-PEGGY2_HEIGHT/2);
      starz[i]=i+2;
    }

	gCount = 800;

}

void loop_star3( void )
{
	int i,x1,y1,x2,y2;

	buffer[0].Clear();

	for ( i = 0; i < NUMSTARS; i++)
	{
		x1=((starx[i]/starz[i])+(PEGGY2_WIDTH/2));
		y1=((stary[i]/starz[i])+(PEGGY2_HEIGHT/2));

		starz[i]--;

		x2=((starx[i]/starz[i])+(PEGGY2_WIDTH/2));
		y2=((stary[i]/starz[i])+(PEGGY2_HEIGHT/2));

		if (x2<0 || x2>PEGGY2_WIDTH || y2<0 || y2>PEGGY2_HEIGHT || starz[i]<2)
		{
			starx[i]=c*(int)(qrand()*PEGGY2_WIDTH-PEGGY2_WIDTH/2);
			stary[i]=c*(int)(qrand()*PEGGY2_HEIGHT-PEGGY2_HEIGHT/2);
			starz[i]=350;
		}
		else
		{
			 buffer[0].Line(x1,y1,x2,y2);
		}
	}

	buffer[0].RefreshAll( 1 ); 

}

#endif
