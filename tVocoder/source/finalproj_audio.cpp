using namespace std;

#include <iostream>
#include <math.h>

#include "chuck_fft.h"
#include "RtAudio.h"
#include "RtMidi.h"

unsigned int def_buffer_size = 1024;
int timez = 0;

bool snapMode = true;	//this should be actually controlled, at some point

class AudioData{
public:
	AudioData(){}
	AudioData(PitchSource *source, PitchDetector *detector, PitchShifter *shifter){
		pitchsource = source;
		pitchdetector = detector;
		pitchshifter = shifter; // this was the segfault problem? I'm a fucking idiot!
	}
	PitchSource *pitchsource;
	PitchDetector *pitchdetector;
	PitchShifter *pitchshifter;
};

int audio_callback(void *outputBuffer, void *inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void *userData){
	
	double *input_buff = (double*)inputBuffer;
	double *output_buff = (double*)outputBuffer;
	memset(output_buff, 0.0, nFrames * sizeof(double));
	
	AudioData *data = (AudioData*)userData;	
	data->pitchsource->getPitches();
	float pitch = data->pitchdetector->detectPitch(input_buff,nFrames);
	
	data->pitchshifter->shiftPitch(input_buff, pitch, data->pitchsource->pitches, (double*)outputBuffer, nFrames);

	return 0;
}

void audio_init(AudioData *sharedData){
	sharedData->pitchsource = new MidiPitchSource();
	sharedData->pitchdetector = new AutoCorrelator();
	sharedData->pitchshifter = new SmbPitchShifter(true, sharedData->pitchsource->isActive());
	
	RtAudio *audio = new RtAudio();
	RtAudio::StreamParameters *output_params = new RtAudio::StreamParameters();
	output_params->deviceId = audio->getDefaultOutputDevice();
	output_params->nChannels = 1;
	
	RtAudio::StreamParameters *input_params;
	input_params = new RtAudio::StreamParameters();
	input_params->deviceId = audio->getDefaultInputDevice();
	input_params->nChannels = 1;
	RtAudio::StreamOptions *stream_options = new RtAudio::StreamOptions();
	stream_options->numberOfBuffers = 32;
	audio->openStream(output_params, input_params, RTAUDIO_FLOAT64, 44100, &def_buffer_size, &audio_callback, sharedData, stream_options);
	audio->startStream();
}
