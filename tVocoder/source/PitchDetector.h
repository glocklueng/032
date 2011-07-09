#ifndef _PITCH_DETECTOR
#define _PITCH_DETECTOR

using namespace std;

#include <stdlib.h>

class PitchDetector{	
	public:
		virtual float detectPitch(double *buffer, int nFrames){}
};

class AutoCorrelator : public PitchDetector{
	public:
		AutoCorrelator(){ prevBuffer = NULL; }
		virtual float detectPitch(double *buffer, int nFrames);
	
	private:
		double *prevBuffer;
};

#endif