#include <math.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include "tinyptc.h"

#define WIDTH (256)
#define HEIGHT (256)
#define SIZE (WIDTH*HEIGHT)
float dist;

static int pixel[SIZE];

unsigned long pixels = 0;
static int lastpx,lastpy;

float distance(float x1, float y1, float x2, float y2)
{
   return sqrt(pow(x2-x1,2) + pow(y2-y1,2));
}

void setPixel(int x,int y,unsigned long col,int fast)
{
	pixels += abs(lastpx-x)+abs(lastpy-y);
	
	lastpx=x;lastpy=y;

	pixel[(y*WIDTH)+x] = col;
	ptc_update(pixel);
	
	if(fast)
		_sleep(fast);

}

void plotLine(int x0, int y0, int x1, int y1,unsigned long col,int fast)
{
  int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = dx+dy, e2; /* error value e_xy */
 int seg1,seg2;	
  float dist = distance(x0,y0,x1,y1);

  pixels = 0;
  lastpx=0;lastpy=0;

  // calculate 20% of distance
  seg1  = ( dist / 100.0f ) * 20.0f;
  
  // subtract
  dist -= seg1;

  for(;;){  /* loop */


	  if( seg1 > 0 ) {
		  // first segment, slow
		  setPixel(x0,y0,col/2,fast?seg1:0);
	   	  seg1	--;
		  dist--;
	  } else {
		  if (dist >0 ) {
			  // fast 
			setPixel(x0,y0,col,0);
			dist--;
		  } else {
			  //slow last
			  setPixel(x0,y0,col/2,fast?10:0);
		  }
	  }

    if (x0==x1 && y0==y1) 
		break;
    
	e2 = 2*err;

    if (e2 >= dy) { 
		err += dy; 
		x0 += sx; 
	} 
    
	if (e2 <= dx) { 
		err += dx; 
		y0 += sy; 
	} 
  }
}
  

int main()
{
	int w=0,h=0,x=0,y=0,lastx=0,lasty=0;


    if (!ptc_open("test",WIDTH,HEIGHT)) return 1;
	
    while (1)
    {

        plotLine(x,y,w,h,0xaaaaaa/2,0);
		plotLine(x,0,w,0,0,0);
		plotLine(0,y,0,h,0,0);
		w = rand()%WIDTH;;
        h = rand()%HEIGHT;
//		x = rand()%WIDTH;;
  //      y = rand()%HEIGHT;
		x = lastx;
		y = lasty;
		plotLine(x,0,w,0,0xFF0000,0);
		plotLine(0,y,0,h,0xFF00FF,0);
		
		printf("(%d,%d) - (%d,%d) = ",x,y,w,h);

        if ( dist > 10 ) 
			plotLine(x,y,w,h,0xffFFff,1);
		else
			plotLine(x,y,w,h,0xffFFff,0);
		printf("pixels=%d %f\n",pixels,distance(x,y,w,h));
		
		lastx = w ;
		lasty = h;

        ptc_update(pixel);
		_sleep(100);
		if( _kbhit()) {
			memset(pixel,0,sizeof(pixel));
			getch();
		}
    }
}

void keydown(INT)
{
}

void keyup(INT)
{
}

int cleanup(void)
{
	return 0;
}

void mousedown(int xPos,int yPos, int button  )
{
}


void mousemove(int xPos,int yPos, int button  )
{
}


void mouseup(int xPos,int yPos, int button  )
{
}



