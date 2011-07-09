
#include <stdlib.h>
#include <iostream>

using namespace std;

#include "PitchDetector.h"

float AutoCorrelator::detectPitch(double *buffer, int nFrames){
	if(prevBuffer == NULL){
		prevBuffer = (double*) malloc(nFrames*sizeof(double));
		memcpy(prevBuffer, buffer, nFrames * sizeof(double));
		return 0.0;
	}
	float secCor = 0;
	int secLag = 0;
	
	float maxCorr = 0;
	int maxLag = 0;
	for(int lag = 512; lag >= 40; lag--){
		float corr = 0;
		for(int i = 0; i < nFrames; i++){
			float lagVal = (i - lag < 0)? prevBuffer[nFrames - (lag - i)] : buffer[i - lag];
			corr += lagVal * buffer[i];
		}
		if(corr > maxCorr){
			maxCorr = corr;
			maxLag = lag;
		}
		if(corr >= 0.9 * maxCorr){
			secCor = corr;
			secLag = lag;
		}
	}
	memcpy(prevBuffer, buffer, nFrames * sizeof(double));
	if(maxCorr < 0.1) return 0.0;
	return 44100.0 / secLag;	//--works better for singing
	//return 44100.0 / maxLag;
}