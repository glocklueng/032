#include <windows.h>


#include <set>
#include <deque>
#include <iostream>
#include <math.h>
#include "PitchShifter.h"
#include "smbPitchShift.cpp"
#include "GUISlider.h"

#ifdef __MACOSX_CORE__
// note: for mac only
#include <GLUT/glut.h>
#else


#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glut.h>
#endif

PitchShifter::PitchShifter(bool snapmode, bool pluggedIn){
	g_width1 = 1024;
	g_height1 = 768;
	snapMode = snapmode;
	midiPluggedIn = pluggedIn;
	numshifts = 5000; 
	shifts = deque<PitchShift>();
	
	
	float shiftplotheight = g_height1/3.0;
	float pianoplotheight = shiftplotheight + g_height1/2.0;
	float controlplotY = pianoplotheight;
	float vibratoX = g_width1 * (2.0 / 3.0);
	float vibSliderLeft = (g_width1 - vibratoX)*0.1 + vibratoX;
	float vibSliderLen = (g_width1 - vibratoX)*0.8;
	float sliderHeight = 20;
	

	//float x, float y, float width, float height, int minVal, int maxVal, int initialVal
	vibRateSlider = new GUISlider(vibSliderLeft, shiftplotheight + (controlplotY-shiftplotheight) * 0.12,
								  vibSliderLen, sliderHeight, 0.2, 20.0, 4.0);
	vibDepthSlider = new GUISlider(vibSliderLeft, shiftplotheight + (controlplotY-shiftplotheight) * 0.35,
								   vibSliderLen, sliderHeight, 0.0, 0.05, 0);
	attackSlider = new GUISlider(g_width1 * 0.10, controlplotY + (g_height1 - controlplotY)*0.3, g_width1 * 0.25, 20, 0.0, 200, 0.0);

	
	autoPitches.insert(0);
	autoPitches.insert(2);
	autoPitches.insert(4);
	autoPitches.insert(5);
	autoPitches.insert(7);
	autoPitches.insert(9);
	autoPitches.insert(11);
	currPitch = 0;
	attack = 0;
	numElapsed = 0;
	vibRate = 4.0;
	vibDepth = 0.00;
	g_time = 0.0;
}

PitchShifter::~PitchShifter(){
	delete vibRateSlider;
	delete vibDepthSlider;
	delete attackSlider;
}

void PitchShifter::setWindow(float w, float h){
	g_width1 = w;
	g_height1 = h;
	float shiftplotheight = g_height1/3.0;
	float pianoplotheight = shiftplotheight + g_height1/2.0;
	float controlplotY = pianoplotheight;
	float vibratoX = g_width1 * (2.0 / 3.0);
	float vibSliderLeft = (g_width1 - vibratoX)*0.1 + vibratoX;
	float vibSliderLen = (g_width1 - vibratoX)*0.8;
	float sliderHeight = 20;
	vibRateSlider = new GUISlider(vibSliderLeft, shiftplotheight + (controlplotY-shiftplotheight) * 0.12,
								  vibSliderLen, sliderHeight, 0.2, 20.0, vibRate);
	vibDepthSlider = new GUISlider(vibSliderLeft, shiftplotheight + (controlplotY-shiftplotheight) * 0.35,
								   vibSliderLen, sliderHeight, 0.0, 0.05, vibDepth);
	attackSlider = new GUISlider(g_width1 * 0.10, controlplotY + (g_height1 - controlplotY)*0.3, g_width1 * 0.25, 20, 0.0, 200, (float)attack * 1024.0/441.0);
}

/*
 * LOTS of magic numbers, prolly needs to be redone
 * but this hack works for now
 * not even sure if the coordinates mean anything
 */
int PitchShifter::getPitchFromCoords(GLfloat x, GLfloat y)
{
	float shiftplotheight = g_height1/3.0;
	float pianoplotheight = shiftplotheight + g_height1/2.0;
	float pianoHeight = (pianoplotheight - shiftplotheight) * 0.8;
	float pianoBottom = (pianoplotheight - shiftplotheight) * 0.1 + shiftplotheight;
	float pianoWidth = g_width1 * 0.56;
	float pianoLeft = g_width1 * 0.05;
	float keyWidth = pianoWidth / 12.0;
	
	if(x < pianoLeft || x > pianoLeft + pianoWidth
		|| y < pianoBottom || y > pianoBottom + pianoHeight) {
		return -1;
	}
	for(int i = 0; i < 12; i++)
	{
		float x1 = pianoLeft + keyWidth * ((float)i);
		float y1 = pianoBottom;
		float x2 = pianoLeft + keyWidth * (((float)i) + 0.8);
		float y2 = pianoBottom+pianoHeight;
		if(i == 1 || i == 3 || i == 6 || i == 8 || i == 10) {
			y1 += pianoHeight*0.3;
		}
		if(y1 <= y && y <= y2 && x1 <= x && x <= x2) {
			return i;
		}
	}
	return -1;
}

void PitchShifter::handlePianoClick(int xCoord, int yCoord) {
	int pitchVal = getPitchFromCoords(xCoord, yCoord);
	if(pitchVal >= 0) {
		addPitch(pitchVal);
	}
	
	//toggle snap mode
	float controlplotY = 5.0*g_height1/6.0;
	float x1 = g_width1 * 0.8;
	float y1 = (g_height1 - controlplotY) * 0.1 + controlplotY;
	float x2 = g_width1 * 0.9;
	float y2 = (g_height1 - controlplotY) * 0.9 + controlplotY;
	if(midiPluggedIn && xCoord >= x1 && xCoord <= x2 &&
	   yCoord >= y1 && yCoord <= y2) {
		toggleSnapMode();
	}
	
}

void PitchShifter::addPitch(int pitch){
	if(0 > pitch || 12 < pitch) { return; }
	if(autoPitches.end() == autoPitches.find(pitch)) {
		autoPitches.insert(pitch);
	}
	else {
		autoPitches.erase(autoPitches.find(pitch));
	}
}

bool PitchShifter::containsPitch(int pitch) {
	return (autoPitches.end() == autoPitches.find(pitch));
}

float PitchShifter::snapFactor(float freq){
	float p = 0.0;
	float d = 0.0;
	int p_i = 0;
	int d_i = 0;
	for(int i = 1; i < 120; i++) {
		set<int>::iterator autoIter;
		autoIter = autoPitches.begin();
		bool endLoop = false;
		while( autoIter != autoPitches.end() ) {
			int k = *autoIter;
			if(i % 12 == k) 
			{
				p = d;
				p_i = d_i;
				d = 8.175 * pow(1.05946309f, (float) i);
				d_i = i;
				if(d > freq) { endLoop = true; }
				break;
			}
			++autoIter;
		}
		if(endLoop) {
			break;
		}
	}
	if(d == 0.0) { return 1.0; }
	int num_i = 0;
	double num = 0.0;
	if(d - freq > freq - p) {
		num_i = p_i;
		num = p;
	}
	else {
		num_i = d_i;
		num = d;
	}
	if(num_i != currPitch) {
		numElapsed = 0;
		currPitch = num_i;
	}
	if(attack > numElapsed) {
		double n = (num - freq) / attack * numElapsed;
		num = freq + n;
	}
	numElapsed++;
	return num/freq;
}

void PitchShifter::updateShifts(){
	if(shifts.size() >= numshifts) shifts.pop_front();
	PitchShift shift;
	shift.detectedPitch = pitch;
	shift.shiftedPitch = shiftedPitch;
	shifts.push_back(shift);
	
	//these are going here, because this gets called once per frame
	vibRate = vibRateSlider->getValue();
	vibDepth = vibDepthSlider->getValue();
	attack = (int)((attackSlider->getValue() * 441) / 1024.0);
	
}

/*
 * Time is in the positive x direction, pitch in the y direction.
 * Y=0 maps to 80Hz, Y=5 maps to 1100Hz.
 */
void PitchShifter::drawShifts(float x, float y, float width, float height){
	float curX = x;
	glPushMatrix();
	
	//draw line for detected color
	glBegin( GL_LINE_STRIP );
	glColor3f(1.0,0.0,0.0);
	for(deque<PitchShift>::iterator iter = shifts.begin(); iter != shifts.end(); iter++){
		float detectedY = iter->detectedPitch - 80;
		if(detectedY < 0.0) { detectedY = 0.0; }
		detectedY *= (height) / (1100.0 - 80.0);
		glVertex2f(curX, detectedY);
		curX += width / numshifts;
	}
	glEnd();
	
	curX = x;
	
	//draw line for shifted color
	glBegin( GL_LINE_STRIP );
	glColor3f(0.0,1.0,0.0);
	for(deque<PitchShift>::iterator iter = shifts.begin(); iter != shifts.end(); iter++){
		float shiftedY = iter->shiftedPitch - 80;
		if(shiftedY < 0.0) { shiftedY = 0.0; }
		shiftedY *= (height) / (1100.0 - 80.0);
		glVertex2f(curX, shiftedY);
		curX += width / numshifts;
	}
	glEnd();
	
	glPopMatrix();
}


/*
 * Time is in the positive x direction, pitch in the y direction.
 * Y=0 maps to 80Hz, Y=5 maps to 1100Hz.
 */
void PitchShifter::drawVibrato(float x, float y, float width, float height){
	
	glPushMatrix();
	
	//draw line for detected color
	glBegin( GL_LINE_STRIP );
	glColor3f(1.0,1.0,1.0);
	float yScale = g_height1 * 1.4;
	for(float i = 0.0; i < width; i++){
		float shift = sin( 2 * 3.14159265358979 * vibRate * i / 100 ) * vibDepth;
		glVertex2f(x+i, y + shift * yScale);
	}
	glEnd();
	glPopMatrix();
}


/*
 * Time is in the positive x direction, pitch in the y direction.
 * Y=0 maps to 80Hz, Y=5 maps to 1100Hz.
 */
void PitchShifter::drawCorrections(float x, float y, float width, float height){
	/*float xLow = 0.0;
	float xHigh = 0.75;
	float xLim = xHigh - xLow;
	float yLevel = 0.4;
	float yScale = 2.5;
	float x = xLow;*/
	
	float yCenter = (y + height) / 2;
	
	glPushMatrix();
	
	//draw line for shifted color
	glBegin( GL_LINES );
	glColor3f(0.0,0.0,1.0);
	/*glVertex3f( xLow, yLevel, 1.0 );
	glVertex3f( xHigh, yLevel, 1.0 );*/
	glVertex2f(x, yCenter);
	glVertex2f(x + width, yCenter);	
	glEnd();
	
	//draw line for detected color
	glBegin( GL_LINE_STRIP );
	glColor3f(1.0,1.0,1.0);
	float detectedPitch, shiftedPitch;
	float curX = x;
	for(deque<PitchShift>::iterator iter = shifts.begin(); iter != shifts.end(); iter++){
		/*
		float detectedY = iter->detectedPitch - 80;
		detectedY *= 3.0 / (1100.0 - 80.0);
		detectedY -= 0.35;
		
		float shiftedY = iter->shiftedPitch - 80;
		shiftedY *= 3.0 / (1100.0 - 80.0);
		shiftedY -= 0.35;
		 */
		/*
		detectedY = iter->detectedPitch;
		shiftedY = iter->shiftedPitch;
		float corr = 0.0;
		if(detectedY > 0.0) {
			corr = 0.5 * (detectedY / shiftedY - 1.0);
		}
		if (corr > 0.2) { corr = 0.0; }
		else if (corr < -0.2) { corr = -0.0; }
		
		glVertex3f( x, corr*yScale + yLevel, 1.0 );
		x += xLim / numshifts;
		*/
		detectedPitch = iter->detectedPitch;
		shiftedPitch = iter->shiftedPitch;
		float detectedY = detectedPitch - 80;
		if(detectedY < 0.0) { detectedY = 0.0; }
		detectedY *= (height) / (1100.0 - 80.0);
		
		float shiftedY = shiftedPitch - 80;
		if(shiftedY < 0.0) { shiftedY = 0.0; }
		shiftedY *= (height) / (1100.0 - 80.0);
		
		float yScale = (yCenter - y) / 10.0;
		float shift = detectedY - shiftedY;
		if(shift > 10.0) shift = 10.0;
		if(shift < -10.0) shift = -10.0;
		
		glVertex2f(curX, yCenter + shift * yScale);
		
		curX += width / numshifts;
	}
	glEnd();
	setDetectedNote(shiftedPitch);
	/*
	static char str[1024];
	sprintf( str, "Correction amount (Hz): %f", (shiftedY - detectedY));
	draw_string( -1.0f, 0.9f, 0.0f, str, 0.5f );
	*/
	glPopMatrix();
	
	vibRateSlider->drawSlider();
	vibDepthSlider->drawSlider();
	attackSlider->drawSlider();
}

void PitchShifter::setDetectedNote(float pitch)
{
	for(int i = 0; i < 120; i++) {
		float d = 8.175 * pow(1.05946309, (float) i) - pitch;
		if(-1.0 < d && d < 1.0) {
			detectedNote = i;
			return;
		}
	}
	detectedNote = -1;
}

bool PitchShifter::isDetectedNote(int note) {
	return (note % 12) == (detectedNote % 12) && detectedNote >= 0;
}

bool PitchShifter::isSnapMode() {
	return snapMode;
}

bool PitchShifter::isMidiPluggedIn() {
	return midiPluggedIn;
}

void PitchShifter::toggleSnapMode() {
	snapMode = !snapMode;
}


float PitchShifter::addVibrato(int nFrames){
	g_time += nFrames;
	float d = sin(2 * 3.14159265358979 * vibRate * g_time / 44100) * vibDepth;
	return d;
}

/*
 * SmbPitchShifter
 */

SmbPitchShifter::SmbPitchShifter(bool snapmode, bool pluggedIn) : PitchShifter(snapmode, pluggedIn) {}	//hey-- I'm starting to get the hang of this c++ thing!

void SmbPitchShifter::pitchShift(double factor, int nFrames, float *buff){
	//before the second nFrames was def_buffer_size, but def_buffer_size == nFrames (I think)
	smbPitchShift(factor, nFrames, nFrames, 32, 44100, buff, buff);
}

void SmbPitchShifter::shiftPitch(double *inputBuff, float inputPitch, set<float> &pitches, double *outputBuff, int nFrames){
	pitch = inputPitch;
	float shiftFactor = 1.0;
	if(snapMode) {
		if(inputPitch > 0) {
			shiftFactor = snapFactor(inputPitch);
			shiftFactor += addVibrato(nFrames);
		}
		if (shiftFactor > 2.0) shiftFactor = 2.0;
		if (shiftFactor < 0.5) shiftFactor = 0.5;
		
		float tempBuff[nFrames];
		for(int i = 0; i < nFrames; i++) {
			tempBuff[i] = (float)(inputBuff[i]);
		}
		
		pitchShift(shiftFactor, nFrames, tempBuff);
		
		for(int i = 0; i < nFrames; i++) {
			outputBuff[i] += (double)(tempBuff[i]);
		}
	}
	else {
		for(set<float>::iterator iter = pitches.begin(); iter != pitches.end(); ++iter) {
			shiftFactor = 1.0;
			float midiPitch = *iter;
			if(inputPitch > 0 && midiPitch > 0) {
				shiftFactor = midiPitch/inputPitch;
			}
		
			if (shiftFactor > 2.0) shiftFactor = 2.0;
			if (shiftFactor < 0.5) shiftFactor = 0.5;
		
			float tempBuff[nFrames];
			for(int i = 0; i < nFrames; i++) {
				tempBuff[i] = (float)(inputBuff[i]);
			}
		
			pitchShift(shiftFactor, nFrames, tempBuff);
		
			for(int i = 0; i < nFrames; i++) {
				outputBuff[i] += (double)(tempBuff[i]);
			}
			break;	//this line is a hack, because we are not polyphonic right now
		}
	}
	
	//vibrato
	//addVibrato(outputBuff, nFrames);
	
	shiftedPitch = inputPitch * shiftFactor;
	updateShifts();
}
/*
void SmbPitchShifter::drawShifts(float x, float y, float width, float height){
	PitchShifter::drawShifts(x, y, width, height);
}
*/