using namespace std;

#ifdef __MACOSX_CORE__
// note: for mac only
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "graphics_helpers.h"

#include <set>
#include <vector>
#include <math.h>

//#include "GUIView.h"

// width and height of the window
//GLsizei g_width = 1024;
//GLsizei g_height = 768;

//shared data between audio and graphics
AudioData *g_data;
GLuint tex_2d;


//-----------------------------------------------------------------------------
// Name: reshapeFunc( )
// Desc: called when window size changes
//-----------------------------------------------------------------------------
void reshapeFunc( GLsizei w, GLsizei h )
{
    //uncomment to fix window size, for simplicity
	//w = 800;
	//h = 600;
    g_width = w; g_height = h;
	g_data->pitchshifter->setWindow(w, h);
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );			
	glLoadIdentity();
	gluOrtho2D(0, g_width, 0, g_height);
	
}



//-----------------------------------------------------------------------------
// Name: keyboardFunc( )
// Desc: key event
//-----------------------------------------------------------------------------
void keyboardFunc( unsigned char key, int x, int y )
{
    switch( key )
    {
        case 'Q':
        case 'q':
            exit(1);
            break;
    }
	
    glutPostRedisplay( );
}


void mouseMoveFunc(int x, int y){
	g_data->pitchshifter->vibRateSlider->handleClick(x,y);
	g_data->pitchshifter->vibDepthSlider->handleClick(x,y);
	g_data->pitchshifter->attackSlider->handleClick(x,y);
}


//-----------------------------------------------------------------------------
// Name: mouseFunc( )
// Desc: handles mouse stuff
//-----------------------------------------------------------------------------
void mouseFunc( int button, int state, int x, int y )
{
	y = g_height - y;
	
    if( button == GLUT_LEFT_BUTTON )
    {	
        // when left mouse button is down
        if( state == GLUT_DOWN )
        {
			g_data->pitchshifter->handlePianoClick(x,y);
			g_data->pitchshifter->vibRateSlider->setClicked(x,y, true);
			g_data->pitchshifter->vibDepthSlider->setClicked(x,y, true);
			g_data->pitchshifter->attackSlider->setClicked(x,y, true);
        }
        else
        {
			g_data->pitchshifter->vibRateSlider->setClicked(x,y, false);
			g_data->pitchshifter->vibDepthSlider->setClicked(x,y, false);
			g_data->pitchshifter->attackSlider->setClicked(x,y, false);
        }
    }
    else if ( button == GLUT_RIGHT_BUTTON )
    {
        // when right mouse button down
        if( state == GLUT_DOWN )
        {
        }
        else
        {
        }
    }
    else
    {
    }
	
    glutPostRedisplay( );
}


//-----------------------------------------------------------------------------
// Name: idleFunc( )
// Desc: callback from GLUT
//-----------------------------------------------------------------------------
void idleFunc( )
{
    // render the scene
    glutPostRedisplay( );
}


void drawRects()
{
	float shiftplotheight = g_height/3.0;
	float pianoplotheight = shiftplotheight + g_height/2.0;
	float pianoHeight = (pianoplotheight - shiftplotheight) * 0.8;
	float pianoBottom = (pianoplotheight - shiftplotheight) * 0.1 + shiftplotheight;
	float pianoWidth = g_width * 0.56;
	float pianoLeft = g_width * 0.05;
	float keyWidth = pianoWidth / 12.0;
	float controlplotY = pianoplotheight;
	
	for(int i = 0; i < 12; i++)
	{
		if(g_data->pitchshifter->containsPitch(i)) {
			glColor3f(40.0/256.0, 76.0/256.0, 126.0/256.0);
		}
		else if(g_data->pitchshifter->isDetectedNote(i)) {
			glColor3f(0.0, 0.5, 0.5);
		}
		else {
			glColor3f(0.0, 0.0, 0.5);
		}
		float x1 = pianoLeft + keyWidth * ((float)i);
		float y1 = pianoBottom;
		float x2 = pianoLeft + keyWidth * (((float)i) + 0.8);
		float y2 = pianoBottom+pianoHeight;
		if(i == 1 || i == 3 || i == 6 || i == 8 || i == 10) {
			y1 += pianoHeight*0.3;
		}
		glRectf(x1, y1, x2, y2);
	}
	glColor3f(1.0, 1.0, 1.0);
	
	//write note names on keys
	float x = pianoLeft + keyWidth * 0.1;
	float y = pianoBottom+pianoHeight*0.85;
	float size = 0.15;
	float singleOffset = 0.14;
	static char str[1024];
	sprintf(str, "C");
	draw_string(x + keyWidth * singleOffset, y, str, size);
	x += keyWidth;
	sprintf(str, "C#");
	draw_string(x, y, str, size);
	x += keyWidth;
	sprintf(str, "D");
	draw_string(x + keyWidth * singleOffset, y, str, size);
	x += keyWidth;
	sprintf(str, "D#");
	draw_string(x, y, str, size);
	x += keyWidth;
	sprintf(str, "E");
	draw_string(x + keyWidth * singleOffset, y, str, size);
	x += keyWidth;
	sprintf(str, "F");
	draw_string(x + keyWidth * singleOffset, y, str, size);
	x += keyWidth;
	sprintf(str, "F#");
	draw_string(x, y, str, size);
	x += keyWidth;
	sprintf(str, "G");
	draw_string(x + keyWidth * singleOffset, y, str, size);
	x += keyWidth;
	sprintf(str, "G#");
	draw_string(x, y, str, size);
	x += keyWidth;
	sprintf(str, "A");
	draw_string(x + keyWidth * singleOffset, y, str, size);
	x += keyWidth;
	sprintf(str, "A#");
	draw_string(x, y, str, size);
	x += keyWidth;
	sprintf(str, "B");
	draw_string(x + keyWidth * singleOffset, y, str, size);
	x += keyWidth;
	
	if(g_data->pitchshifter->isMidiPluggedIn()) {
		float x1 = g_width * 0.8;
		float y1 = (g_height - controlplotY) * 0.1 + controlplotY;
		float x2 = g_width * 0.9;
		float y2 = (g_height - controlplotY) * 0.9 + controlplotY;
		if(g_data->pitchshifter->isSnapMode()) {
			glColor3f(155.0/256.0, 200.0/256.0, 256.0/256.0);
			sprintf(str, "Snap Mode");
		}
		else {
			glColor3f(105.0/256.0, 150.0/256.0, 211.0/256.0);
			sprintf(str, "MIDI Mode");
		}
		draw_string(g_width * 0.6, (y1+y2)/2, str, 0.2);
		glRectf(x1, y1, x2, y2);
	}
	
}

//-----------------------------------------------------------------------------
// Name: displayFunc( )
// Desc: callback function invoked to draw the client area
//-----------------------------------------------------------------------------
void displayFunc( )
{
	// local state
    static GLfloat zrot = 0.0f, c = 0.0f;
	
    // clear the color and depth buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//not working in 3D, so no depth test
	glDisable(GL_DEPTH_TEST);
	
    // line width
    glLineWidth( 2.0 );
    // step through and plot the waveform
    // increment

    // push the matrix
    glPushMatrix();

	//shiftplot
	glColor3f(15.0/256.0, 79.0/256.0, 168.0/256.0);
	float shiftplotheight = g_height/3.0;
	glRectf(0,0,g_width,shiftplotheight);
	
	//pianoplot
	glColor3f(155.0/256.0, 200.0/256.0, 256.0/256.0);
	float pianoplotheight = shiftplotheight + g_height/2.0;
	glRectf(0, shiftplotheight, g_width, pianoplotheight);
	
	glColor3f(5.0/256.0, 49.0/256.0, 109.0/256.0);
	float controlplotY = pianoplotheight;
	glRectf(0, controlplotY, g_width, g_height);
	
	glColor3f(67.0/256.0, 128.0/256.0, 211.0/256.0);
	float vibratoX = g_width * (2.0 / 3.0);
	float vibSliderLeft = (g_width - vibratoX)*0.1 + vibratoX;
	float vibSliderLen = (g_width - vibratoX)*0.8;
	glRectf(vibratoX, shiftplotheight, g_width, controlplotY);
	
	g_data->pitchshifter->updateShifts();
	
	//g_data->pitchshifter->drawShifts(0.0, 0.0, g_width, shifplotheight);
	g_data->pitchshifter->drawCorrections(0.0, 0.0, g_width, shiftplotheight);
	g_data->pitchshifter->drawVibrato(vibSliderLeft, shiftplotheight + (controlplotY - shiftplotheight) * 0.67, vibSliderLen, shiftplotheight);
	
	drawRects();
	
	float sliderHeight = 20;
	//draw label text
	glColor3f(0.0, 0.0, 0.0);
	static char str[1024];
	
	//vibrato labels
	sprintf(str, "Vibrato");
	draw_string(vibSliderLeft, shiftplotheight + (controlplotY-shiftplotheight) * 0.9, str, 0.2);
	sprintf(str, "Depth");
	draw_string(vibSliderLeft, shiftplotheight + (controlplotY-shiftplotheight) * 0.4, str, 0.15);
	sprintf(str, "Rate");
	draw_string(vibSliderLeft, shiftplotheight + (controlplotY-shiftplotheight) * 0.17, str, 0.15);
	
	//vibrato scales
	sprintf(str, "0.2 Hz");
	draw_string(vibSliderLeft, shiftplotheight + (controlplotY-shiftplotheight) * 0.05, str, 0.1);
	sprintf(str, "20 Hz");
	draw_string(vibSliderLeft + vibSliderLen*0.85, shiftplotheight + (controlplotY-shiftplotheight) * 0.05, str, 0.1);
	sprintf(str, "0.0");
	draw_string(vibSliderLeft, shiftplotheight + (controlplotY-shiftplotheight) * 0.28, str, 0.1);
	sprintf(str, "5.0");
	draw_string(vibSliderLeft + vibSliderLen*0.91, shiftplotheight + (controlplotY-shiftplotheight) * 0.28, str, 0.1);
	
	//vibrato values
	float rate = ((int)(g_data->pitchshifter->vibRateSlider->getValue() * 100.0 ))/ 100.0;
	float depth = ((int)(g_data->pitchshifter->vibDepthSlider->getValue() * 10000.0 ))/ 100.0;
	sprintf(str, "%g Hz", rate);
	draw_string(vibSliderLeft + vibSliderLen*0.425, shiftplotheight + (controlplotY-shiftplotheight) * 0.05, str, 0.1);
	sprintf(str, "%g", depth);
	draw_string(vibSliderLeft + vibSliderLen*0.425, shiftplotheight + (controlplotY-shiftplotheight) * 0.28, str, 0.1);
	
	//attack labels
	glColor3f(1.0, 1.0, 1.0);
	sprintf(str, "Attack");
	draw_string(g_width * 0.10, controlplotY + (g_height - controlplotY)*0.6, str, 0.2);
	sprintf(str, "0 ms");
	draw_string(g_width * 0.04, controlplotY + (g_height - controlplotY)*0.3, str, 0.1);
	sprintf(str, "200 ms");
	draw_string(g_width * 0.38, controlplotY + (g_height - controlplotY)*0.3, str, 0.1);
	
	//attack value
	float att = ((int)(g_data->pitchshifter->attackSlider->getValue() * 100.0 ))/ 100.0;
	sprintf(str, "%g ms", att);
	draw_string(g_width * 0.21, controlplotY + (g_height - controlplotY)*0.12, str, 0.1);
    
	// pop
    glPopMatrix();
	
    // flush!
    glFlush( );
    // swap the double buffer
    glutSwapBuffers( );
}

/*
 * this is slightly hackish, as AudioData depends on audio
 * existing before graphics in terms of includes. yikes.
 */
void graphics_init(int argc, char **argv, AudioData *sharedData){
	g_data = sharedData;
	
	// initialize GLUT
    glutInit( &argc, argv );
    // double buffer, use rgb color, enable depth buffer
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    // initialize the window size
    glutInitWindowSize( g_width, g_height );
    // set the window postion
    glutInitWindowPosition( 0, 0 );
    // create the window
    glutCreateWindow( "AwesomeBox" );
	
    // set the idle function - called when idle
    glutIdleFunc( idleFunc );
    // set the display function - called when redrawing
    glutDisplayFunc( displayFunc );
    // set the reshape function - called when client area changes
    glutReshapeFunc( reshapeFunc );
    // set the keyboard function - called on keyboard events
    glutKeyboardFunc( keyboardFunc );
    // set the mouse function - called on mouse stuff
    glutMouseFunc( mouseFunc );
	// set mouse move callback
	glutMotionFunc( mouseMoveFunc );
	glutMainLoop();
	
	
}
