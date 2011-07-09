using namespace std;

#include "GUISlider.h"

#ifdef __MACOSX_CORE__
// note: for mac only
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <iostream>
#include <math.h>

GUISlider::GUISlider(){
	/*x = y = minVal = 0;
	width = maxVal = 1;
	sliderWidth = .5;*/
}
GUISlider::GUISlider(float x, float y, float width, float height, float minVal, float maxVal, float initialVal){
	setCoords(x, y, width, height, minVal, maxVal, initialVal);
	sliderWidth = .1;
}

float GUISlider::getValue(){
	return val;
}

void GUISlider::setCoords(float x, float y, float width, float height, float minVal, float maxVal, float initialVal){
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->minVal = minVal;
	this->maxVal = maxVal;	
	val = initialVal;	
}

void GUISlider::setClicked(int xCoord, int yCoord, bool click){
	clicked = false;
	float sliderCenterX = x + width * (val - minVal) / (maxVal - minVal);
	if(fabs(xCoord - sliderCenterX) > height) return;
	float yCenter = y;//(y + height) / 2.0;
	if(fabs(yCoord - yCenter) > height / 2.0) return;
	clicked = click;
}

void GUISlider::handleClick(int xCoord, int yCoord){
	if(clicked){
		val = ((xCoord - x) / (width)) * (maxVal - minVal);
		if(val > maxVal) val = maxVal;
		if(val < minVal) val = minVal;
	}
}

void GUISlider::drawSlider(){
	glPushMatrix();

	float yCenter = y;//(y + height) / 2.0;

	//draw line
	glColor3f(0.0,0.0,0.0);
	glBegin(GL_LINES);
	glVertex2f(x, yCenter);
	glVertex2f(x + width, yCenter);
	glEnd();
	
	//draw button -- right now it's a rectangle for simplicity
	glColor3f(1.0,1.0,1.0);
	float sliderCenterX = x + width * (val - minVal) / (maxVal - minVal);
	glRectf(sliderCenterX - height / 2.0, yCenter - height / 2.0, sliderCenterX + height / 2.0, yCenter + height / 2.0);
	
	glPopMatrix();
}