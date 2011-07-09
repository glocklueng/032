#ifndef _PITCH_SOURCE
#define _PITCH_SOURCE


#include "RtMidi.h"
#include <set>

using namespace std;


class PitchSource {
	public:
		set<float> pitches;
		virtual ~PitchSource() {}
		virtual void getPitches() {}
		virtual bool isActive() {return 0;}
};

class MidiPitchSource : public PitchSource{	
	public:
		MidiPitchSource();
		~MidiPitchSource() {}
		virtual bool isActive();
		virtual void getPitches();

	private:
		RtMidiIn *midi;
		bool active;
};

#endif