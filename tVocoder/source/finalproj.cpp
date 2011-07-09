using namespace std;

//include these first!
#include "PitchSource.h"
#include "PitchDetector.h"
#include "PitchShifter.h"

#include "finalproj_audio.cpp"
#include "finalproj_graphics.cpp"

/*
 * This file is what is actually compiled into an executable
 * it is set up like this (importing graphics and audio
 * rather than just implementing them here) because it became
 * far too difficult to figure out where what was.
 *
 * All relevant global variables are listed in the relevant
 * files, rather than here (although maybe that should change).
 */

/*
 * There is currently an information-sharing problem between the
 * audio and video portions of this program. This is currently
 * remedied by creating a class that contains a PitchSource,
 * PitchDetector, and PitchShifter and passing it to graphics_init
 * after initializing it in audio_init. This is probably not the best
 * solution, because graphics basically has to rely on globals, because
 * glut (to my knowledge/googling) has no user data that you can pass.
 */

int main(int argc, char **argv){
	AudioData sharedData;
	audio_init(&sharedData);
	graphics_init(argc, argv, &sharedData);
	while(true){
		usleep(10000);
	}
	
	//closestream stuff?
}
