#ifndef  _BIGFIX_H_

#define  _BIGFIX_H_ (1)

#define rgb(r,g,b) r,g,b

typedef struct pal_rgb_tag{ 
	unsigned short r;
	unsigned short g;
	unsigned short b;
} pal_rgb;

typedef struct PaletteEntry_tag{ 
	
	char stock_id[8];
	char colourName[32];
	double ratio;

	pal_rgb colours[6];

	char avail[16];

} paletteEntry;

#endif
