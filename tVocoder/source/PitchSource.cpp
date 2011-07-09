//using namespace std;

#include "PitchSource.h"

#include "RtMidi.h"

#include <set>
#include <vector>
#include <math.h>
#include <iostream>

MidiPitchSource::MidiPitchSource(){
	pitches = set<float>();
	
	midi = new RtMidiIn();
	unsigned int nPorts = midi->getPortCount();
	if ( nPorts == 0 ) {
		std::cout << "No ports available!\n";
		active = false;
		return;
	}
	active = true;
	
	midi->openPort( 0 );
	midi->ignoreTypes( false, false, false );
	fprintf(stdout, "reading MIDI input on track N\n");
}

bool MidiPitchSource::isActive(){
	return active;
}

void MidiPitchSource::getPitches(){
	vector<unsigned char> message;
	int nBytes;
	double stamp;
	
	do {
		stamp = midi->getMessage( &message );
		nBytes = message.size();
		if(nBytes == 0) {
			break;
		}
		
		//cout << (int)message[0] << " " << (int)message[1] << " " << (int)message[2] << endl;
		
		//if(message[0] != 144) { continue; } -- this line doesn't actually make sense
		
		/*
		 * This code implicitly leverages the fact that you cant get two identical
		 * key messages from the same midi keyboard (as in two down, as opposed
		 * to one up one down). This may or may not be an issue later (probably not).
		 */
		
		//128 implies note off event (spec),
		//velocity of 0 has been working empirically
		float freq = (float)(8.175 * pow(1.05946309, message[1]));
		if(message[2] == 0 || message[0] == 128) {
			pitches.erase(freq);
		}
		else {
			//rough translation to standard piano tuning
			pitches.insert(freq);
		}					
	} while (nBytes != 0);
}