#include "common.h"

// This one is pretty memory hungry

#if USE_STARFIELD1

// Member constants
// Set this to the number of stars you would like in the starfield
#define NUM_STARS		( 12 )

// Speed of the stars ranges from 0.1 to this.
#define VELOCITY_MAX		(1.7f)

// NOTE: BRIGHT_MAX is related to FRAME_MAX.
// BRIGHT_MAX is the number of greyscale levels and FRAME_MAX is the number
// of frames (or planes) required to represent it.
// Safe numbers are:
/*
   Greyscale levels       Frames
         2                  1
         4                  2
         8                  3
         16                 4
         32                 5
       higher            too slow!
*/
// Set as per chart above.
// moved to common.h

// Set as per chart.  This could be automatically calculated with some macros
// here (it is pow(2,FRAMES_MAX)) but it... isn't.  C'mon now. Really? Use the chart.
#define BRIGHT_MAX 64



// The star class represents a star (drawn as one LED on PEGGY2).
// Each star originates at the middle of the board.  Each star
// is randomly assigned an angle, defining their direction from the
// center; a velocity, and a brightness.  Since the standard PEGGY2 has
// width=height I have omitted a scaling factor for the velocity
// of the height versus the width.  If you have stacked PEGGY2s,
// then you will need to add this consideration.

// How brightness is handled:
// Each star is randomly assigned a brightness of 1 to BRIGHT_MAX, representing
// the available greyscale levels on Peggy2.  A brightness of 0 would
// be valid, but isn't drawn (black/off).


// The Star class represents the "star" in the starfield.
class Star 
{
  private:
  
  float angle;       // Angle in radians
  float velocity;    // Speed of star
  float step_x;      // Per frame increment to x pos
  float step_y;      // Per frame increment to y pos

  // Quick utility method to generate random floats  
  float CreateRandomFloat (float maxValue)
  {
    return ((float)qrand() / ((float)RAND_MAX +1.) * maxValue);
  }
  
  // Overloaded for ranges
  float CreateRandomFloat (float minValue, float maxValue)
  {
    float swap;
    
    // Validate range
    if (minValue > maxValue)
    {
      swap = maxValue;
      maxValue = minValue;
      minValue = swap;
    }
    
    return ((maxValue- minValue)*((float)qrand()/RAND_MAX)) + minValue;
  }
  
  
  public:
  
  float pos_x;      // Raw x position
  float pos_y;      // Raw y position
  int   brightness; // Greyscale level
  
  //Constructor
  Star()
  {
    GenerateStar();
  }

  // Destructor  
  ~Star()
  {
    //Do nothing
  }  
  
  
  // Creates the "star" by defining initial values.
  void GenerateStar()
  {
    // Set the position to be near the centre
    pos_x = PEGGY2_WIDTH / 2;
    pos_y = PEGGY2_HEIGHT / 2;

    // What's going on?  Imagine a circle on a plane bound by a width of 2 and a height
    // of 2 units.  If it is at origin (0,0) it extends up and down from (0,1) to (0,-1)
    // and left to right from (-1,0) to (0,-1).  The circumference of this circle is
    // diameter * PI.  Since d=2, this gives 2PI.  2PI is the number of radians in a 
    // circle; 360o is equivalent to 2PI.
    
    // By using an origin near the centre of the Peggy2 board, we can give each "star"
    // a direction, by giving it an angle from the origin.  This will cause all the
    // stars to look like they are spreading outwards in 3D.  We move the star along the
    // x and y axis depending on its angle and velocity.  The increment amount is calculated
    // via x = cosine(a) and y = sin(a). Next, we multiply the increment by the
    // velocity (v<1 means slower stars, while, you guessed it, v>1 means faster stars).
    
    // All these numbers are floats though, and to display them we have to convert them
    // to real world coordinates that represent LEDs on the Peggy2 board.  To do this,
    // when we plot the pixel as a point on the board, we convert the floats to integer.
    // We use a roundoff function instead of truncating in order to create smoothness.
    
    // By using multiple frames, and drawing the stars into one or more of these frames,
    // and then displaying each frame for a different amount of time, we create greyscale
    // (technically, this is called pulse width modulation).  The program defaults to
    // 5 frames, meaning 2^5 or 32 levels of greyscale are available for the starfield.
    // You can reduce this as per the chart above, but I have found it too slow going
    // any higher.  The brightness variable of the start represents its greyscale level.

    
    // Velocity must be v > 0, and v < VELOCITY_MAX.
    do
    {
      velocity = CreateRandomFloat(0.1f, VELOCITY_MAX);
    }
    while (velocity == 0);
    
    // Create in degrees
    angle = CreateRandomFloat(360.f);
    
    // Convert to radians
    angle = (float)((angle * PI) / 180);
    
    // Pre calculate the incremental steps; this 
    // reduces calc time during execution
    // x increment = cosine(angle) * velocity; y increment = sine(angle) * velocity)
    step_x = (float)(cos(angle) * velocity);
    step_y = (float)(sin(angle) * velocity);
    
    // Integer brightness from 1 to BRIGHT_MAX-1
    brightness = (int)qrand()%BRIGHT_MAX; 
  }
  
  
  // Move star; if star has moved off screen, regenerate it.
  void MoveStar()
  {
    // Move x,y by step_x, step_y
    pos_x += step_x;
    pos_y += step_y;
    
    if ((pos_x < 0) || (pos_x > PEGGY2_WIDTH) ||
        (pos_y < 0) || (pos_y > PEGGY2_HEIGHT))
      GenerateStar();
  }

};


// Variables

static Star starfield[NUM_STARS];   // Starfield of n stars

static char intensity[FRAMES_MAX];   // Stores LED intensity for greyscale


// Another quick utility method to round - this makes the stars
// display smoother than just truncating... if anything can be "smooth"
// on a 25x25 display. ;)
int Round(float x)
{
  return ((int)(x > 0.0 ? x + 0.5f : x- 0.5f));
}


// This procedure implements the levels of greyscale by
// drawing the stars onto each frame accordingly, as per this
// chart
/*
        Greyscale
Frame   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
    0   -  x  -  x  -  x  -  x  -  x  -  x  -  x  -  x
    1   -  -  x  x  -  -  x  x  -  -  x  x  -  -  x  x
    2   -  -  -  -  x  x  x  x  -  -  -  -  x  x  x  x
    3   -  -  -  -  -  -  -  -  x  x  x  x  x  x  x  x
    
    and so on.  32 levels of greyscale uses 5 frames (0-4).
*/
void SetPointGrey (int x, int y, int grey)
{
  for (int i=0; i<FRAMES_MAX; i++)
  {
    if ((grey & intensity[i]) == intensity[i])
     buffer[i].SetPoint(x,y); 
  }
}
  

// Redraws frame buffers to create greyscale 
void Redraw()
{
   for (int i=0; i<FRAMES_MAX; i++)
    buffer[i].RefreshAll(intensity[i]);
}
  

  
void setup_star()
{ 
  // Init frame buffer.  Only needs to be done once, no matter how many frames.
  buffer[0].HardwareInit();

 
  // Pre-calc tbe LED brightness levels to save clocks
  // The madness of the casting and rounding is necessary with pow() otherwise
  // it produces unpredictable integer values.
  for (int i=0; i<FRAMES_MAX; i++)
    intensity[i] = Round(pow (2.f, (float)i));
    
  // Create each star in the starfield.
  for (int i=0; i<NUM_STARS; i++)
    starfield[i].GenerateStar();
} 

 
void loop_star()
{ 
  // Reset all frames.
  for (int i=0; i<FRAMES_MAX; i++)
    buffer[i].Clear();
  
  // Translate each star coords to integer (x,y) locations and draw
  for (int i=0; i<NUM_STARS; i++)
  {
    starfield[i].MoveStar();
    SetPointGrey(Round(starfield[i].pos_x), Round(starfield[i].pos_y), starfield[i].brightness); 
  }
  
  // Draw each frame for its intensity level
  Redraw();
    
} 

#endif
