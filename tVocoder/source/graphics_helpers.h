#ifndef _GRAPHICS_HELPERS
#define _GRAPHICS_HELPERS

#ifdef __MACOSX_CORE__
// note: for mac only
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <string>

// width and height of the window
GLsizei g_width = 1024;
GLsizei g_height = 768;

void draw_string( GLfloat x, GLfloat y, const char * str, GLfloat scale = 1.0f );

#endif