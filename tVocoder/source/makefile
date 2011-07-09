CXX=g++
INCLUDES=
FLAGS=-D__MACOSX_CORE__ -c -O3 -funroll-loops
LIBS=-framework CoreAudio -framework CoreMIDI -framework CoreFoundation \
	-framework IOKit -framework OpenGL -framework Carbon \
	-framework GLUT -framework Foundation -framework AppKit -lstdc++ -lm

OBJS=   RtMidi.o RtAudio.o finalproj.o chuck_fft.o PitchDetector.o PitchShifter.o PitchSource.o GUISlider.o graphics_helpers.o

finalproj: $(OBJS)
	$(CXX) -o finalproj $(OBJS) $(LIBS)

finalproj.o: finalproj.cpp finalproj_graphics.cpp finalproj_audio.cpp PitchDetector.h PitchShifter.h PitchSource.h graphics_helpers.h
	$(CXX) $(FLAGS) finalproj.cpp
	
PitchSource.o: PitchSource.h PitchSource.cpp
	$(CXX) $(FLAGS) PitchSource.cpp
	
PitchDetector.o: PitchDetector.h PitchDetector.cpp
	$(CXX) $(FLAGS) PitchDetector.cpp
	
PitchShifter.o: PitchShifter.h PitchShifter.cpp graphics_helpers.h
	$(CXX) $(FLAGS) PitchShifter.cpp
	
GUISlider.o: GUISlider.h GUISlider.cpp
	$(CXX) $(FLAGS) GUISlider.cpp
	
graphics_helpers.o: graphics_helpers.h graphics_helpers.cpp
	$(CXX) $(FLAGS) graphics_helpers.cpp

RtMidi.o: RtMidi.h RtMidi.cpp RtError.h
	$(CXX) $(FLAGS) RtMidi.cpp

RtAudio.o: RtAudio.h RtAudio.cpp RtError.h
	$(CXX) $(FLAGS) RtAudio.cpp

chuck_fft.o: chuck_fft.h chuck_fft.c
	$(CXX) $(FLAGS) chuck_fft.c

clean:
	rm -f *~ *# *.o finalproj
