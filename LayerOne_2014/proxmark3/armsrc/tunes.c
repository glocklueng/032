
#include <proxmark3.h>
#include <stdlib.h>
#include "apps.h"
#include "legicrf.h"

#define OCTAVE_OFFSET ( 0 )
#define TONE_PIN      ( 13 )

#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951

#define isdigit(n) (n >= '0' && n <= '9')

const unsigned short notes[] = { 0,
	NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
	NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
	NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
	NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
};


const char song[][512] = {
	"TheSimpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6\0\0",
	"Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6\0\0",
	"Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6\0\0",
	"Looney:d=4,o=5,b=140:32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f\0\0",
	"Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6\0\0",
	//5
	"MASH:d=8,o=5,b=140:4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b\0\0",
	"StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6\0\0",
	"TopGun:d=4,o=4,b=31:32p,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,16f,d#,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,g#\0\0",
	"A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#\0\0",
	"Flinstones:d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6\0\0",
	//10
	"Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#\0\0",
	"MahnaMahna:d=16,o=6,b=125:c#,c.,b5,8a#.5,8f.,4g#,a#,g.,4d#,8p,c#,c.,b5,8a#.5,8f.,g#.,8a#.,4g,8p,c#,c.,b5,8a#.5,8f.,4g#,f,g.,8d#.,f,g.,8d#.,f,8g,8d#.,f,8g,d#,8c,a#5,8d#.,8d#.,4d#,8d#.\0\0",
	"MissionImp:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d\0\0",
	"GoodBad:d=4,o=5,b=56:32p,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,d#,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,c#6,32a#,32d#6,32a#,32d#6,8a#.,16f#.,32f.,32d#.,c#,32a#,32d#6,32a#,32d#6,8a#.,16g#.,d#\0\0",
	"KnightRider:d=4,o=5,b=125:16e,16p,16f,16e,16e,16p,16e,16e,16f,16e,16e,16e,16d#,16e,16e,16e,16e,16p,16f,16e,16e,16p,16f,16e,16f,16e,16e,16e,16d#,16e,16e,16e,16d,16p,16e,16d,16d,16p,16e,16d,16e,16d,16d,16d,16c,16d,16d,16d,16d,16p,16e,16d,16d,16p,16e,16d,16e,16d,16d,16d,16c,16d,16d,16d\0\0",
	//15
	"DukesOfHazard:d=4,o=5,b=160:8a6,8c7,8d7,c7,16a6,16g6,1f6,p,8a6,8c7,8d7,c7,16a6,16g6,1f.6,p,8g#6,g#6,8f6,8g#6,8g6,f6,8f6,8f6,8d#6,8d6,8c6,8a,c6,8c6,8c6,8d6,8f6,8d6,1f6\0\0",
	"DaftPunk:d=8,o=5,b=125:4d6,4c#.6,a,b,a,b,a,4c#6,b,4a,c#.6,e.6,4f#.6,a,b,a,b,a,4c#6,b,4a,c#6,e6,4f#6,4p,a,b,a,b,a,b,a,4c#.6,a,a,a,a,4a,f#,a,a,4a,4f#,4e\0\0",
	"Aerodynami:d=4,o=5,b=125:16g.6,32c.6,16d#6,32c.6,32g6,32p,16c6,16d#6,32c.6,32g6,32p,32c.6,16d#6,32c6,32p,16g6,16c6,32d#6,16c6,16d#.6,16a,32c.6,16a,32d#6,32p,32a6,32p,16c6,16a,32d#6,32p,32a,32p,16c6,16a,16p,32a,32p,16c6,16a,16p,16c6,32d#6,32p,16c6,32g#6\0\0",
	"AXELF:d=4,o=5,b=160,f#,8a.,8f#,16f#,8a#,8f#,8e,f#,8c.6,8f#,16f#,8d6,8c#6,8a,8f#,8c#6,8f#6,16f#,8e,16e,8c#,8g#,f#.\0\0",
	"Ghostbusters:d=8,o=5,b=180,b,b,d#6,b,p,c#6,a,0p,b,b,b,b,a\0\0",
	//20
	"Inspector:d=4,o=5,b=285,e,8f#,g,8a,b.,g.,a#.,f#.,a.,g.,e,8f#,g,8a,b.,e.6,d#.6,g.6,2g6\0\0",
	"smb:d=4,o=5,b=100:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6\0\0",
	"smb_under:d=4,o=6,b=100:32c,32p,32c7,32p,32a5,32p,32a,32p,32a#5,32p,32a#,2p,32c,32p,32c7,32p,32a5,32p,32a,32p,32a#5,32p,32a#,2p,32f5,32p,32f,32p,32d5,32p,32d,32p,32d#5,32p,32d#,2p,32f5,32p,32f,32p,32d5,32p,32d,32p,32d#5,32p,32d#\0\0",
	"smbdeath:d=4,o=5,b=90:32c6,32c6,32c6,8p,16b,16f6,16p,16f6,16f.6,16e.6,16d6,16c6,16p,16e,16p,16c\0\0",
	"korobyeyniki:d=4,o=5,b=160:e6,8b,8c6,8d6,16e6,16d6,8c6,8b,a,8a,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,2a,8p,d6,8f6,a6,8g6,8f6,e6,8e6,8c6,e6,8d6,8c6,b,8b,8c6,d6,e6,c6,a,a\0\0",
	//25
	"Tetris:d=4,o=5,b=160:d6,32p,c.6,32p,8a,8c6,8a#,16a,16g,f,c,8a,8c6,8g,8a,f,c,d,8d,8e,8g,8f,8e,8d,c,c,c\0\0",
	"tetris3:d=4,o=5,b=63:d#6,b,c#6,a#,16b,16g#,16a#,16b,16b,16g#,16a#,16b,c#6,g,d#6,16p,16g#,16a#,16b,c#6,16p,16b,16a#,g#,g,g#,16f,16g,16g#,16a#,8d#.6,32d#6,32p,32d#6,32p,32d#6,32p,16d6,16d#6,8f.6,16d6,8a#,8p,8f#6,8d#6,8f#,8g#,a#.,16p,16a#,8d#.6,16f6,16f#6,16f6,16d#6,16a#,8g#.,16b,8d#6,16f6,16d#6,8a#.,16b,16a#,16g#,16f,16f#,d#\0\0",
	"BubbleBobble:d=4,o=5,b=125:8a#6,8a6,8g.6,16f6,8a6,8g6,8f6,8d#6,8g6,8f6,16d#6,8d.6,f.6,16d6,16c6,8a#,8c6,8d6,8d#6,8c6,16d6,8d#.6,8f6,8f6,8g6,16a6,8g.6,8f6,8f6,8g6,8a6,8a#6,8a6,8g.6,16f6,8a6,8g6,8f6,8d#6,8g6,8f6,16d#6,8d.6,f.6,16d6,16c6,8a#,8c6,8d6,8d#6,8c6,16d6,8d#.6,8f6,8f6,8g6,16a6,8f.6,8a#.6\0\0",
	"bubbonus:d=4,o=5,b=70:16e6,16c6,16e6,16c6,16e6,16c6,16d6,16e6,16f6,16d6,16f6,16d6,16f6,16d6,16e6,16f6,16g6,16e6,16g6,16e6,16g6,16e6,16f6,16g6,16f6,16d6,16f6,16d6,16f6,16d6,16e6,16f6,16e6,16c6,16e6,16c6,16e6,16c6,16d6,16e6,16f6,16d6,16f6,16d6,16f6,16d6,16e6,16f6,16g6,16e6,16g6,16e6,16g6,16e6,16f6,16g6,16f6,16d6,16f6,16d6,16f6,16d6,16e6,16f6\0\0",
	"bubover:d=4,o=5,b=140:8b,16d6,8e6,16b,8g6,16g.6,8p,32p,8b,16d6,8e6,16b,8g6,16g.6,8p,32p,8b,16d6,8e6,16b,8g6,16g.6,8p,32p,8b,16d6,8e6,16b,8g6,16g.6,8p,32p,8f#6,16g#6,8b6,16f#6,8g#6,16b6,8f#6,16g#6,8b6,16f#6,8g#6,16b6,8f#6,16g#6,8b6,16p,8f#6,16g#6,8b6,16f#6,8g#6,16b6,8f#6,16g#6,8b6,16f#6,8g#6,16b6,16f#6,16g#6,16b6,16f#6,16g#6,16b6\0\0",
	//30
	"gng:d=4,o=5,b=112:16b,16b,b,8f#6,8g6,8f#6,8f6,8e6,8f#.6,16f#,f#.\0\0",
	"gng1:d=4,o=5,b=100:g#6,8g6,f#6,8f6,8e6,16d6,16d#6,16p,16d6,16d#6,16p,16d6,16d#6,16p,16c#,16d#,16c#,8d#,8g\0\0",
	"gradius:d=4,o=5,b=70:8c#,16c#.,32f#,16b,8a#,16f#,16c#.6,32e,8e.,32c#,32d,32e,32a,32b,32c#6,16f#.,16d#.,8f#,16b,32d#,32f,32f#,32g#,16a,32f#,16d,16a,32f#,16b,32g#,16e,32g#,16b,8c#,16c#.,32f#,16b,8a#,16f#,16c#.6,32e,8e.,32c#,32d,32e,32a,32b,32c#6,16d#.6,32f#,8f#.,32b,32c#6,16d#.6,32b,16g#,c#6\0\0",
	"grdintro:d=4,o=5,b=112:2e6,8p,16e6,16p,2d6,8p,16d6,16p,2c6,8p,16c6,16p,2d6,p,16g#,16f#,16g#,16a#,16p,16b,16p,16a#,16p,16g#,16p,16f#,16g#,16f#,16g#,16e,16g#,16f#,16g#,16a#,16p,16b,16p,16a#,16p,16g#,16p,16f#,16g#,16f#,16g#,16e,16f#,16e,16f#,16g#,16p,16a,16p,16g#,16p,16f#,16p,16g#,e\0\0",
	"rtype:d=4,o=4,b=125:d.5,f.5,16g5,16a5,2g5,8p,g5,f5,32c6,d.6,f.6,1d7,8p,32f.6,32p,32f.6,16p,32e.6,32p,32e.6,16p,32d.6,32p,32d.6,16p,32c.6,32p,32c.6,16p,16b.5,32p,16c.6,32p,32g.6,32p,32g.6,16p,32f.6,32p,32f.6,16p,32e.6,32p,32e.6,16p,32d.6,32p,32d.6,16p,16c.6,32p,16d6\0\0",
	//35
	"Robocop:d=4,o=5,b=140:8f,8g#,8f,g,a#.,32p,8f,8g#,8f,2c#,8p,8f,8g#,8f,g,a#,2d#.,p,8f,8g#,8f,g,a#.,32p,8a#,8c#6,8a#,2f6,8p,8f6,8g#6,8f6,c7,g#6,2a#.6,8p,16a#6,16a#6,2c.7\0\0",
	"yiear:d=4,o=5,b=125:f6,8p,16g6,16f6,8d6,8c6,8d6,8f6,16c6,16d6,16c6,16a#,8g,32p,8a#.,32p,a#\0\0",
	"gods:d=4,o=5,b=63:8f.7,16d#7,8d7,8c7,8d#.7,16d7,8a#6,8c7,16c7,16a#.6,p,8f.7,16d#7,8d7,8c7,8d#.7,16d7,8a#6,8c7,16c7,16a#.6,p,8c#.7,p,8c.7,p,16c7,8a#.6,p,8c#.7,8p,16c#7,16f7,8d#7,8p,8c#7,8c7,16c7,8a#.6\0\0",
	"chaosengine:d=4,o=5,b=125:16d#6,16a#6,16a#6,16d#6,16a#6,16a#6,16d#6,16a#6,16a#6,16d#6,16a#6,16a#6,16d#6,16a#6,16a#6,16a#6,16c#6,16g#6,16g#6,16c#6,16g#6,16g#6,16c#6,16g#6,16g#6,16c#6,16g#6,16g#6,16c#6,16g#6,16g#6,16g#,16d#6,16a#6,16a#6,16d#6,16a#6,16a#6,16d#6,16a#6,16a#6,16d#6,16a#6,16a#6,16d#6,16a#6,16a#6,16a#6,16c#6,16g#6,16g#6,16c#6,16g#6,16g#6,16c#6,16g#6,16g#6,16c#6,16g#6,16g#6,16c#6,16g#6,16g#6,16g#\0\0",
	"lemmings:d=4,o=5,b=140:f#6,8f#.6,16f#6,8f#.6,16e6,d6,f#6,8f#.6,16f#6,16f#.6,16g6,16f#.6,e6,f#6,8f#.6,16f#6,8f#.6,16g6,a6,1c#6,b,8b.,16b,8b.,16c#6,d6,c#6,8c#.6,16c#6,8c#.6,16d6,e6,a6,f#6,d6,a,1e6\0\0",
	//40
	"turrican:d=4,o=5,b=180:p,8c.6,16p,8c6,8g6,8c.7,8c6,8g6,8c.7,8c.7,8c7,8a#.6,8p,8a#.6,8a#6,8a#.6,8a#.6,8a#6,8a.6,8a#.6,g6,p,8c.6,16p,8c6,8g6,8c.7,8c6,8g6,8c.7,8c.7,8c7,8a#.6,8p,8d#.7,8d#7,8d#.7,8d#.7,8d#7,8d.7,8a#.6,c.7\0\0",
	"wormsong:d=4,o=5,b=112:g,a,b.,8d6,d6,8c6,8b,8a.,16g,8a,8b,2a,32p,8a.,16g,8a,8b,2e,32p,8a.,16g,8a,8b,a,8g,8a,b.,8d6,d6,8c6,8b,8a.,16g,8a,8b,2a,32p,8a.,16g,8a,8b,2e,32p,8a.,16g,8a,8b,a\0\0",
	"xenon2:d=4,o=5,b=112:16a,16p,16a,16a,16c6,16a,16a,16a,16c6,a,8p,16c6,16p,16c6,16c6,16e6,16c6,16c6,16c6,16e6,c6,8p,16g,16p,16g,16g,16b,16g,16g,16g,16b,g,8p,16a,16p,16a,16a,16c6,16a,16a,16a,16c6,a,8p\0\0",
	"starfox:d=4,o=5,b=70:f#6,16f6,16f#6,16g#6,16c#6,16d#6,16f6,8g#6,32f#6,32f6,f#6,8f#6,8g#6,8a#6,8b6,32e6,32d#6,8e6,8b6,8b6,32e6,32d#6,8e6,8b6,8b6,32a#6,32g#6,f#6\0\0",
	"smk_title:d=4,o=5,b=100:16c6,32p,16c6,16d6,32p,16c6,32p,16b,16p,16g6,8p,16d6,32p,16f6,32p,16e6,32p,16d6,16e6,32p,16f6,p,16c6,32p,16c6,16d6,32p,16c6,32p,16b,16p,16g6,8p,16d6,32p,16f6,32p,16e6,32p,16d6,16e6,32p,16f6\0\0",
	//45
	"addams:d=4,o=5,b=160:8c,8d,8e,8f,1p,8d,8e,8f#,8g,1p,8d,8e,8f#,8g,p,8d,8e,8f#,8g,p,8c,8d,8e,8f,1p,8c,f,8a,f,8c,b4,g.,8f,e,8g,e,8c,a4,f.,8c,f,8a,f,8c,b4,g.,8f,e,8c,d,8e,2f\0\0",
	"smwwd1:d=4,o=5,b=125:a,8f.,16c,16d,16f,16p,f,16d,16c,16p,16f,16p,16f,16p,8c6,8a.,g,16c,a,8f.,16c,16d,16f,16p,f,16d,16c,16p,16f,16p,16a#,16a,16g,2f,16p,8a.,8f.,8c,8a.,f,16g#,16f,16c,16p,8g#.,2g,8a.,8f.,8c,8a.,f,16g#,16f,8c,2c6\0\0",
	"alteredb:d=2,o=6,b=63:16d#,16c#,16d#,8g#5,16a#5,16b5,16c#,16c#,16b5,16c#,8f#5,16g#5,16a#5,16b5,16d#,16c#,16d#,8g#5,16a#5,16b5,16c#,16c#,16b5,16c#,8f#5,16g#5,16a#5,16b5,16d#,16c#,16d#,8g#,16f#,16e,16d#,16c#,16b5,16c#,8d#,16a#5,16g#5,16f#5,g#5\0\0",
	"commando:d=4,o=5,b=125:8a.,16a,8a,8a,a,8a,8a,8a.,16e6,8e6,8e6,e6,p,f6,e6,f6,e6,8e6,16b,16b,8b,8b,b,p,8c.6,16c6,8c6,8c6,c6,8c6,8c6,8c.6,16g6,8g6,8g6,g6,p,g#6,g6,g#6,g6,8g6,16d6,16d6,8d6,8d6,d6\0\0",
	"nzstory:d=4,o=5,b=140:8g,8a,8p,8b,8p,16c6,16d6,16c6,16b,8c6,8g,8g,8g,8f#,8g,8p,16c6,16p,16c6,16p,16c6,16p,16c6,16p,16c6,16p,8d6,8e6,16e6,16f6,16e6,16d6,8e6,8c6,8c6,8c6,8b,8c6,8p,16e6,16p,16e6,16p,16e6,16p,16e6,16p,16e6,16p,8f6,8g6,8p,8a6,8g#6,8a6,8f6,8f6,8e6,8f6,8p,8a6,8g#6,8a6,8f#6,8f#6,8e6,8f#6\0\0",
	//50
	"Pacland:d=4,o=5,b=140:16f6,16g6,16f6,16g6,8f6,16a,16a#,16c6,16d6,16c6,16d6,8c6,16f,16g,16a,16p,16a,1p,16f6,16g6,16f6,16g6,8f6,16a,16a#,16c6,16d6,16c6,16d6,8c6,8f,16g,16p,16g,1p,a,8e6,8p,a,8e6,p,16f6,8f6,16d6,8e6,8d.6,p,g,8d6,8p,g,8d6,p,16c6,16d6,16e6,16d6,16c6,16d6,8c.6\0\0",
	"ninjaw:d=4,o=5,b=140:8c6,8d#6,8c6,2f6,8p,8c6,8d#6,8c6,2f6,8p,8c6,8d#6,8c6,f6,8d#6,8d#6,8c6,8c6,8a#,8a#,2c6,8p,8c6,8d#6,8c6,2f6,8p,8c6,8d#6,8c6,2f6,8p,8c6,8d#6,8c6,f6,8g6,8g6,8g#6,8g#6,8g6,8g6,2f6\0\0",
	"spaceharrier:d=4,o=5,b=160:f6,8e6,1e6,g,a,f6,8e6,1e.6,f6,8e6,1e6,d6,e6,1f6,g#6,2g6,f6,f6,8e6,1e6,g,a,f6,8e6,1e.6,f6,8e6,1e6,d6,e6,1f6,g#6,2g6,f6,1e6,e,g,d6,2c#6,a#,2a,8g,8f,e,8f,1d,8d,8e,f,8g,2a,g#,2f6,2d6,1e6,e,g,d6,1c#6,8e6,8c#6,8a#,8a,8g,8f,8e,2d,e,8f,8e,8f,8g,8f,8g,2a,8g,2g6\0\0",
	"outrun_magic:d=4,o=5,b=160:f6,d#6,8g#.6,f6,d#6,8c#.6,d#6,c6,2g#.,c#6,c6,8d#.6,c#6,c6,8f.,a#,16c.6,1a#,f6,d#6,8g#.6,f6,d#6,8c#.6,d#6,c6,2g#.,c#6,c6,8d#.6,c#6,c6,16f.,16g#.,c6,2a#.\0\0",
	"rollthnd:d=4,o=5,b=63:32g4,32a#4,32f#,f,16d#,32d,8p,32g4,32a#4,32f#,16f,32e,16f,32e,16f,32e,16f,32e,16d#,32d,8p,32c,32g,32c6,b,16a#,32a,8p,32c,32g,32c6,32b,32b,32b,32b,32b,32b,32a#,32a#,32a#,32\0\0",
	//55
	"jsw:d=4,o=5,b=80:8b,8b,8b,8b,8a,8g,8g,8f#,8e,8e,8g,8b,8e6,8e6,8e6,8e6,8d6,8c6,8c6,8b,8a,8a,8b,8c6,8b,8c6,8b,8d#6,8c6,8b,8b,8a,8g,8g,8f#,8e,8f#,8f#,8f#,8f#,8g,8f#,8e,8e,8e,8e\0\0",
	"spyvsspy:d=4,o=5,b=125:2f6,8c.6,2d6,p,2a#,16c.6,16a#.,2a,8g.,1f,p,8g.,a#.,8g.,a.\0\0",
	"toas:d=4,o=5,b=225:e,e,8g,8g#,e,e,d,c#,e,e,e,8g,8g#,8d,8e,1p,e,e,8g,8g#,e,e,d,c#,e,e,e,8g,8g#,8d,8e,1p,e,e,8g,8g#,e,e,d,c#,e,e,e,8g,8g#,8d,8e,8f,f,8f,8f#,f#,8f#,8g,g,8g,8g#,g#,8g#,8c6,8c#6,e6,e6,8c#6,8e6,8f#6,8e6,c6,16b,16p,a,8p,8c6,8c#6,e6,e6,8c#6,8e6,8a6,8a6,g6,e6,8p,8e6,8e6,8f#6,8a6,8p,8a6,8p,8a6,8p,f#6,e6,c6,b,a\0\0",
	"monty_title:d=4,o=5,b=140:8g,16d6,16d6,8c#6,8d6,8g6,8d6,8c#6,8d6,8g,16d6,16d6,8c#6,8d6,8g6,8f#6,8g6,8a6,8a#6,16c7,16a#6,8a6,8f6,8d#7,16f#7,16d#7,8d7,8c7,1d7,8g,16d6,16d6,8c#6,8d6,8g6,8d6,8c#6,8d6,8g,16d6,16d6,8c#6,8d6,8g6,8f#6,8g6,8a6,8a#6,16c7,16a#6,8a6,8g6,8d#7,16f7,16d#7,8d7,8f7,8a#6,16c7,16a#6,8a6,8d7,g6"
	"Rambo:d=4,o=5,b=125:16f#4,16f#,16c#,16b4,2c#.,16a4,b4,2f#.4,16f#4,16f#,16c#,16b4,2c#.,8f#,8e.,16f#,2e.,16f#4,16f#,16c#,16b4,2c#.,16a4,b4,2f#.4,16f#4,16f#,16c#,16e,b.4,8c#,8b.4,16a4,2b4\0\0",
	"bttf:d=4,o=5,b=100:c#4,g#.4,8c#,b.4,16a#4,16g#4,8a#.4,8g#.4,8f#4,g#.4,16g#4,16g#4,2g#.4,c#,g#.,8c#6,b.,16a#,16g#,8a#.,8g#.,8f#,1g#,g#.,c#.,2g.,32g#.,32a#.,8g#,8f,8c#,g,32g#.,32a#.,8g#.,8d#.,8g#.,8d#.6,d#.6,d6,32c.6,32d.6,2d#.6,16d#,16g#,16d#,16g#,16c#6,16a#,16d#6,16g#6,c#.6,f#.,2c.6,32c#.6,32d#.6,8c#6,8a#,8f#,c6,32c#.6,32d#.6,8c#.6,8g#.,8c#.6,8g#.6,g#.6,g6,32f.6,32g.6,2g#.6\0\0",
	//60
	"parasols:d=4,o=5,b=160:8b,8c6,8d6,8e6,8b,8c6,8d6,8e6,8b,8c6,d6,b6,g6,8b,8c6,8d6,8e6,8b,8c6,8d6,8e6,8b,8c6,d6,8c6,8b,c6,8a,8b,8c6,8d6,8a,8b,c6,8b,8c6,8d6,8e6,8b,8c6,d6,8c#6,8d6,8e6,8f#6,8c#6,8d6,8e6,8f#6,8g6,a6\0\0",
	"mission:d=4,o=6,b=100:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,16g,8p,16g,8p,16a#,16p,16c7,16p,16g,8p,16g,8p,16f,16p,16f#,16p,16g,8p,16g,8p,16a#,16p,16c7,16p,16g,8p,16g,8p,16f,16p,16f#,16p,16a#,16g,2d,32p,16a#,16g,2c#,32p,16a#,16g,2c,16p,16a#5,16c\0\0",
	"bakkwo:d=4,o=5,b=140:8g,8g,8g,16g6,16g6,8f6,8c6,g,8g,8g,8g,8g6,8f6,8d6,a#6,8f6,8f6,8f6,16g6,16g6,8f6,8d6,g6,a6,f6,8e6,8d6,8e6,8f6,d6,8e6,2f6,g6,e6,8c6,8c6,8d6,8e6,8c6,8c6,8d6,8e6,8f6,8e6,8d6,8c6\0\0",
	"dott:d=4,o=5,b=125:8d#6,16e6,16p,16c#6,16p,16a,16p,16e,16p,d#.,16p,8d#,16e,16p,16a,16p,16c#6,16p,16e6,16p,d#6\0\0",
	"caissons:d=4,o=5,b=125:8g,8e,g,8g,8e,g,8g,8e,8g,16p,16a,8g,8e,g,8e,8f,8g,f,8d,8g,f,8d,2c,8p,8g,8e,g,8g,8e,g,8g,8e,8g,16p,16a,8g,8e,g,8e,8f,8g,f,8d,8g,f,8d,2c,8p,8g,8g,c6,c6,b,8g,16p,16g,8a,8b,8c6,8a,g,p,8c6,c6,8b,8a,8b,8c6,8a,2d6,8p,8g,8g,c6,c6,2b,8a,8b,8c6,8a,g,8e,8f,8g,f,8d,8g,f,8d,2c\0\0",
	"sonicadv:d=4,o=5,b=80:8c#,8d,16e.,8d,8c#,16a.4,8f#4,8a4,8e.,16d.,8e.,8c#,8d,16e.,8d,8c#,16a.4,8b4,8a4,16b.4,g4,8p,8c#,8d,16e.,8d,8c#,16a.4,8f#4,8a4,8e.,16d.,8e.,8c#,8d,16e.,8d,8c#,16a.4,8b4,8a4,16b.4,g4\0\0",
	"Zelda1:d=4,o=5,b=125:a#,f.,8a#,16a#,16c6,16d6,16d#6,2f6,8p,8f6,16f.6,16f#6,16g#.6,2a#.6,16a#.6,16g#6,16f#.6,8g#.6,16f#.6,2f6,f6,8d#6,16d#6,16f6,2f#6,8f6,8d#6,8c#6,16c#6,16d#6,2f6,8d#6,8c#6,8c6,16c6,16d6,2e6,g6,8f6,16f,16f,8f,16f,16f,8f,16f,16f,8f,8f,a#,f.,8a#,16a#,16c6,16d6,16d#6,2f6,8p,8f6,16f.6,16f#6,16g#.6,2a#.6,c#7,c7,2a6,f6,2f#.6,a#6,a6,2f6,f6,2f#.6,a#6,a6,2f6,d6,2d#.6,f#6,f6,2c#6,a#,c6,16d6,2e6,g6,8f6,16f,16f,8f,16f,16f,8f,16f,16f,8f,8f\0\0",
	"ZeldaDung:d=4,o=5,b=80:16g,16a#,16d6,16d#6,16g,16a#,16d6,16d#6,16g,16a#,16d6,16d#6,16g,16a#,16d6,16d#6,16f#,16a,16d6,16d#6,16f#,16a,16d6,16d#6,16f#,16a,16d6,16d#6,16f#,16a,16d6,16d#6,16f,16g#,16d6,16d#6,16f,16g#,16d6,16d#6,16f,16g#,16d6,16d#6,16f,16g#,16d6,16d#6,16e,16g,16d6,16d#6,16e,16g,16d6,16d#6,16e,16g,16d6,16d#6,16e,16g,16d6,16d#6,16d#,16g,16c6,16d6,16d#,16g,16c6,16d6,16d#,16g,16c6,16d6,16d#,16g,16c6,16d6,16d,16g,16c6,16d6,16d,16g,16c6,16d6,16d,16g,16c6,16d6,16d,16g,16c6,16d6,16c,16f#,16a\0\0",
	"ZeldaOvrGB:d=4,o=5,b=140:8c.,16g4,g.4,8c,16c,16d,16d#,16f,g.,16a,16a#,8a.,8g.,8f,8g.,16c,1c6,8p,8g,8d#6,8d6,8d#6,8f6,8g6,16c6,16g6,c.7,8g6,8f6,8d#6,8f6,16a#,16f6,a#.6,8f6,8d#6,8d6,8d#.6,16g,g.,16g,16f,8d#,8f,1g,8c.6,16g,g.,8c6,16c6,16d6,16d#6,16f6,g.6,16g#6,16a#6,8g#6,g6,8f6,8d#.6,16c6,g.6,8d#6,8c7,8g6,d#.7,8d7,8c7,8d7,8d#7,8f7,8g7,16f7,16g7,g#.7,a#7,8g#7,g7,8d7,8d#7,8f7,8d#7,8d7,2c.7\0\0",
	"MonkeyIsland:d=4,o=5,b=100:16d#7,16p,16d#7,16f#7,16e#7,16d#7,16c#7,16p,8d#7, p,16c#7,16p,16c#7,16a#6,16c#7,16a#6,16p,16a#6,16p,8a#6\0\0",
	"ghostdance:d=4,o=5,b=100:8c6,16p,16g,16c6,16d6,8d#.6,8d.6,8c6,16p,16g,16c6,16d6,g6,8p,c6,8p,d#6,16p,16c6,8c#6,16d#6,8f6,b.6,16p,8c6,16p,16g,16c6,16d6,8d#.6,8d.6,8c6,16p,16f#,16c6,16d#6,8f#.6,8f.6,8d#6,8g6,8p,8b6,8d#.7,2c7\0\0",
	"Lechuck:d=4,o=5,b=200:g,8p,8g,8a#,8p,8d6,8p,8c#6,8p,8c#6,8p,a,p,8d#,8p,8d#,8p,d#,p,8d,8p,8d,8p,d,p,g,8p,8g,8a#,8p,8d6,8p,8e6,8p,8e6,8p,c#6,p,8c,8p,8c,8p,8c,8p,8c,8p,2d,2p\0\0",
	"scumm:d=4,o=5,b=180:8g,8p,8g,8a,a#,8a,8g,8c6,8p,8c6,8p,c6,8a#,8a,8g,8p,8g,8a,a#,8a,8g,8d6,8p,8d6,8p,1d6,p,8d6,8p,8d6,8d#6,8f6,8p,8f6,8p,8d6,8p,8d6,8d#6,8f6,8p,8f#6,8p,g6,8d6,8p,d#6,8d6,8c6,d6,8c6,8a#,c6,8a#,8a,g.,8a,a#.,8c6,8d6,8d#6,8d6,8c6,a#,g,g.6,8d6,d#6,8d6,8c6,a#,a,a#,1g\0\0",
	"misland:d=4,o=5,b=180:8e6,8p,8e6,8g6,8f#6,8e6,d6,2e6,8p,8d6,8p,8d6,8c6,8b,8d6,8c6,8p,8c6,8p,b,p,8e6,8p,e.6,8g6,8f#6,8e6,d6,2e6,8p,8f#6,8g6,8p,8g6,8p,a6,p,f#6,8p,8g6,8f#6,8e6,8d6,8f#6,8g6,8p,8g6,8p,f#6\0\0",
	"Frogger:d=4,o=5,b=250:a#,f#,f#,f#,a#,f#,f#,f#,b,b,a#,a#,2g#,2p,b,b,a#,a#,g#,g#,d#6,d#6,c#6,b,a#,g#,2f#\0\0",
	"Frogger1:d=4,o=5,b=70:8c#6,16a#,16f#,8f#6,16f6,16d#6,8c#6,16f#6,16a#,16g#,c#6,16c#6,16c#6,16c#6,16c#6,16a#,16g#,8f#,16f#6,16f#6,16f#6,16g#6,16f#6,16f6,16d#6,16c#6,8a#,16a#,8f#6,8a#,16c#6,2g#,16a#,16a#,16b,16c#6,16d#6,16f6,2d#6,16b,16b,16c#6,8d#6,16f6,16f#6,2f6\0\0",
	"sal1:d=4,o=5,b=112:d6,8a#,8c6,8g#,8c6,a#,16f,16a#,g#,16c6,16d#6,16g#6,d6,8a#,8c6,8g#,8c6,a#,16f,16a#,16d6,c6,16g,16c6,16e6,1f#6,1f6,16d,16e,16f,16g,16f,16e,16f,16e,16d,16e,16d,16c,2a4,2d,16p,16d#,16p,d#,16p,16d#,16p,d#,16p,16d#,16p,8d#,16f,8f#,16f#,16f#,16f,16d#,16d,16e,16f,16g,16f,16e,16f,16e,16d,16e,16d,16c,2a4,2d,16p,16d#,16p,d#,16p,16d#,16p,d#,16p,16d#,16p,8d#,16f,8f#,16f#,16f#,16f,16d#,d\0\0",
	"sal2:d=4,o=5,b=140:16c6,16e6,16g6,16c7,16c6,16e6,16g6,16c7,16c6,16e6,16g6,16c7,16c6,16e6,16g6,16c7,16c6,16d#6,16g#6,16c7,16c6,16d#6,16g#6,16c7,16c6,16d#6,16g#6,16c7,16c6,16d#6,16g#6,16c7,16a#,16d6,16f6,16a#6,16a#,16d6,16f6,16a#6,16a#,16d6,16f6,16a#6,16a#,16d6,16f6,16a#6,16c6,16e6,16g6,16c7,16c6,16e6,16g6,16c7,16b,16e6,16g6,16b6,16b,16e6,16g6,16b6,e,16f,16g,16c6,2g6,d#,16f,16g#,16c6,2d#6,d,16f,16a#,16d6,8f6,8d#6,8f6,d6,b,g,f,e\0\0",
	"Wonderboy:d=4,o=5,b=225:f6,d6,f6,8d6,f6,32p,8f6,d6,f6,d6,e6,c6,e6,8c6,e6,32p,8e6,c6,e6,c6\0\0",
};


// pulled from arduino forums
void play(const char p[],int printName)
{
  unsigned char default_dur = 4;
  unsigned char default_oct = 6;
  unsigned short bpm = 63;
  unsigned short num;
  long wholenote;
  long duration;
  unsigned char note;
  unsigned char scale;
  char *np,name[40];

  memset(name,0,sizeof(name));

  np = &name[0];

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while( *p != ':' ) {
	*np++ = *p;
	p++;					// ignore name
  }

  Dbprintf("%s",name);


  OLEDPrintStr(name);
	p++;                     // skip ':'

	if( printName )
		  return;

  // get default duration
  if(*p == 'd') {
    p++; p++;              // skip "d="
    num = 0;
    while(isdigit(*p)) {
      num = (num * 10) + (*p++ - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }


  // get default octave
  if(*p == 'o') {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }


  // get BPM
  if(*p == 'b') {
    p++; p++;              // skip "b="
    num = 0;
    while(isdigit(*p)) {
      num = (num * 10) + (*p++ - '0');
    }

    bpm = num;
    p++;                   // skip colon
  }

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

  // begin note loop
  while( *p ) {

	UsbPoll(FALSE);
	WDT_HIT();

	if(BUTTON_PRESS())  {
 	 	PWMC_DisableChannel(1);
 	return;
	}

	// first, get note duration, if available
    num = 0;

	while( isdigit(*p) ) {
      num = (num * 10) + (*p++ - '0');
    }

    if( num ) {
	  duration = wholenote / num;
    } else {
	  duration = wholenote / default_dur;  // we will need to check if we are a dotted note after
	}

    // get the note
    switch( *p ) {

      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 5;
        break;
      case 'f':
        note = 6;
        break;
      case 'g':
        note = 8;
        break;
      case 'a':
        note = 10;
        break;
      case 'b':
        note = 12;
        break;
      case 'p':
      default:
        note = 0;
    }

	//lcd_gotoxy(POS(0),LINE(11));
 //	lcd_puts("                          ");
// 	lcd_puts(p);

    p++;

    // get optional '#' sharp
    if(*p == '#') {
      note++;
      p++;
    }

    // get optional '.' dotted note
    if(*p == '.') {
      duration += duration/2;
      p++;
    }

    // get scale
    if(isdigit(*p)) {
      scale = *p - '0';
      p++;
    } else {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    if(*p == ',')
      p++;       // skip comma for next note (or we may be at the end)


    if( note ) {

	  // play the note
	  PWMC_BeepN(1,(notes[(scale - 4) * 12 + note])*10);
	  //SpinDelay( duration/20 );

	  for(volatile unsigned int j=0 ; j < 1300*duration ; j++) j = j;
	  PWMC_DisableChannel(1);

	} else {

	  //delay only
	  for(volatile unsigned int j=0 ; j < 1300*duration ; j++) j = j;
	 // SpinDelay( duration/20 );

    }
  }

	lcd_gotoxy(POS(0),LINE(10));
        lcd_puts("                             ");
	lcd_gotoxy(POS(0),LINE(11));

	lcd_puts("                             ");

        PWMC_DisableChannel(1);
}

int GetSong(void)
{
  return (sizeof(song) / sizeof(song[0]));
}
