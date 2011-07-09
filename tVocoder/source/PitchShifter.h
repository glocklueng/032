#ifndef _PITCH_SHIFTER
#define _PITCH_SHIFTER

using namespace std;

#include <set>
#include <deque>
#include "GUISlider.h"

#ifdef __MACOSX_CORE__
// note: for mac only
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glut.h>
#endif

typedef struct{
	float detectedPitch;
	float shiftedPitch; //eventually this needs to handle multiple pitches . . . (?)
}PitchShift;

class PitchShifter{
	public:
		PitchShifter(bool snapmode, bool pluggedIn);
		virtual ~PitchShifter();
		virtual void shiftPitch(double *inputBuff, float inputPitch, set<float> &pitches, double *outputBuff, int nFrames) {}
		virtual void drawShifts(float x, float y, float width, float height);
		virtual void drawCorrections(float x, float y, float width, float height);
		virtual void drawVibrato(float x, float y, float width, float height);
		virtual void updateShifts();
		virtual void addPitch(int pitch);
		virtual void handlePianoClick(int xCoord, int yCoord);
		virtual void setDetectedNote(float pitch);
		virtual void setWindow(float w, float h);
		virtual bool containsPitch(int pitch);
		virtual bool isDetectedNote(int note);
		virtual bool isSnapMode();
		virtual bool isMidiPluggedIn();
		virtual void toggleSnapMode();
		virtual float addVibrato(int nFrames);
		virtual int getPitchFromCoords(GLfloat x, GLfloat y);
		GUISlider *vibRateSlider;
		GUISlider *vibDepthSlider;
		GUISlider *attackSlider;
		
	protected:
		virtual float snapFactor(float freq);
		int detectedNote;
		float pitch;	//set == inputPitch when shiftPitch is called
		float shiftedPitch;	//set == the target pitch when shiftPitch is called
		bool snapMode;
		int numshifts;	//number of stored detectedPitch, shiftedPitch pairs stored for the viewer (more = slower, less = faster)
		deque<PitchShift> shifts;
		set<int> autoPitches;
		int currPitch;
		int attack;
		int numElapsed;
		double vibRate;
		double vibDepth;
		double g_time;
		bool midiPluggedIn;
		float g_width1;
		float g_height1;
};

class SmbPitchShifter : public PitchShifter{
	public:
		SmbPitchShifter(bool snapmode, bool pluggedIn);
		virtual ~SmbPitchShifter() {}
		virtual void shiftPitch(double *inputBuff, float inputPitch, set<float> &pitches, double *outputBuff, int nFrames);
		//virtual void drawShifts(float x, float y, float width, float height);
	protected:
		virtual float snapFactor(float freq){ return PitchShifter::snapFactor(freq); }
		virtual void updateShifts(){ PitchShifter::updateShifts(); }
	private:
		void pitchShift(double factor, int nFrames, float *buff);
};

#endif