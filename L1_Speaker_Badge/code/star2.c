#include "common.h"

// start star2 


#define NUMBER_OF_STARS 	( 10 )


/*star struct*/
typedef struct 
{
  float xpos, ypos;
  short zpos, speed;
  unsigned char  color;
} STAR;

static STAR stars[NUMBER_OF_STARS];


void init_star2(STAR* star, int i)
{
  /* randomly init stars, generate them around the center of the screen */
  
  star->xpos =  -10.0 + (20.0 * (rand()/(RAND_MAX+1.0)));
  star->ypos =  -10.0 + (20.0 * (rand()/(RAND_MAX+1.0)));
  
  star->xpos *= 3072.0; /*change viewpoint */
  star->ypos *= 3072.0;

  star->zpos =  i;
  star->speed =  2 + (int)(2.0 * (rand()/(RAND_MAX+1.0)));

  star->color = i >> 2; /*the closer to the viewer the brighter*/

}

void loop_star2(void)
{
	static int i, tempx, tempy;  
  /* time based demo loop */
    {
   

      /* clear screen */
      
		buffer[0].Clear();
      /* move and draw stars */
      
      for (i = 0; i < NUMBER_OF_STARS; i++)
	{
	  stars[i].zpos -= stars[i].speed;
	  
	  if (stars[i].zpos <= 0)
	    {
	      init_star2(stars + i, i + 1);
	    }

	  /*compute 3D position*/
	  tempx = (stars[i].xpos / stars[i].zpos) + CENTERX;
	  tempy = (stars[i].ypos / stars[i].zpos) + CENTERY;

	  if (tempx < 0 || tempx > PEGGY2_WIDTH - 1 || tempy < 0 || tempy > PEGGY2_HEIGHT - 1) /*check if a star leaves the screen*/
	    {
	      init_star2(stars + i, i + 1);
	      continue;
	    }
	  
		// Write the point to the buffer
		buffer[0].SetPoint(tempx, tempy);
	}



	// Display the frame buffer
	buffer[0].RefreshAll(4); //Draw frame buffer some number of times
   
   }
  
}

// end

