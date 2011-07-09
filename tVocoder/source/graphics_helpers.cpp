#ifdef __MACOSX_CORE__
// note: for mac only
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <string>

void draw_string( GLfloat x, GLfloat y, const char * str, GLfloat scale = 1.0f ){
	
    GLint len = strlen( str ), i;
	
    glPushMatrix();
    glTranslatef( x, y, 0 );
    glScalef( scale, scale, scale );
	glLineWidth(2.3);
    for( i = 0; i < len; i++ )
        glutStrokeCharacter( GLUT_STROKE_MONO_ROMAN, str[i] );
    
    glPopMatrix();

}