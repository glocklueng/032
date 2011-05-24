// uzebox pong port

#include "common.h"

//Pong - ported to the Uzebox by Bradley Boccuzzi

#include <stdbool.h>
#include <avr/io.h>
#include <stdlib.h>
#include <math.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <uzebox.h>

#include "tiles.c"
#include "data/8x8font.inc"
#include "tilemap.map.inc"
#include "patches.inc"

#define PADDLE1 1
#define PADDLE2 6

struct Paddlestruct{
	unsigned char x;
	unsigned char y;
};

struct SpriteStruct *pBall = &sprites[0]; 	//Creates pointer 'pBall' to point to the sprite tile 0, the ball
struct Paddlestruct Paddle[2];

void ProcessControls( void );
void ProcessBall( void );
void ProcessPaddles( void );
void ProcessScore( void );

//Variables and stuff
unsigned char Score[2] = {0x00, 0x00};
bool Start = false;
bool BallOut = true;
bool GameOver = false;
double X = 0x00, Y = 0x00;

int main(){
	
	//Set stuff up
	InitMusicPlayer(PongPatch);
	SetTileTable(pongtiles); 
	SetFontTilesIndex(5);	
	SetSpritesTileTable(pongtiles);	
	DrawMap(0, 0, court);

	//Ball- 0, paddle 1 - 1, paddle 2 - 6
	pBall->tileIndex = 0;
	MapSprite(PADDLE1, paddle);
	MapSprite(PADDLE2, paddle);

	//Pre-position objects
	pBall->x = 116;
	pBall->y = 108;
	Paddle[0].x = 10;
	Paddle[1].x = 222;
	Paddle[0].y = 85;
	Paddle[1].y = 85;

	while(1){
		ProcessScore();
		ProcessControls();
		ProcessPaddles();
		ProcessBall();
	};
}


void ProcessControls( void ){
	unsigned int joypad1, joypad2 = 0x00;
	joypad1 = ReadJoypad(0);
	joypad2 = ReadJoypad(1);	
	
	if(joypad1&BTN_UP && Paddle[0].y > 8){

		if(joypad1&BTN_B){
			Paddle[0].y-=3;
		}else{
			Paddle[0].y-=2;
		}

	}else if (joypad1&BTN_DOWN && Paddle[0].y < 184){

		if(joypad1&BTN_B){
			Paddle[0].y+=3;
		}else{
			Paddle[0].y+=2;
		}
	}

	if(joypad2&BTN_UP && Paddle[1].y > 8){

		if(joypad1&BTN_B){
			Paddle[1].y-=3;
		}else{
			Paddle[1].y-=2;
		}

	}else if (joypad2&BTN_DOWN && Paddle[1].y < 184){

		if(joypad2&BTN_B){
			Paddle[1].y+=3;
		}else{
			Paddle[1].y+=2;
		}

	}

	if(joypad1&BTN_START && BallOut == true && GameOver == false){
		Start = true;

	}else if(joypad1&BTN_SELECT && BallOut == true){
		SetSpriteVisibility(true);
		Score[0] = 0x00;
		Score[1] = 0x00;
		X = 0x00;
		Y = 0x00;
		GameOver = false;
	}
	
}

void ProcessScore( void ){
	const int *numbers[10] = {n0, n1, n2, n3, n4, n5, n6, n7, n8, n9};
	if (Score[0] < 10 && Score[1] < 10){
		DrawMap(10, 2, (numbers[Score[0]]));
		DrawMap(16, 2, (numbers[Score[1]]));
	}else{
		SetSpriteVisibility(false);
		X = 0;
		Y = 0;
		BallOut = true;
		GameOver = true;
	}
}

void ProcessPaddles( void ){
	MoveSprite(PADDLE1,10,Paddle[0].y, 1,4);
	MoveSprite(PADDLE2,222,Paddle[1].y, 1,4);

		if(pBall->x > Paddle[0].x+5 && pBall->x < Paddle[0].x+8 && pBall->y+8 > Paddle[0].y && pBall->y < Paddle[0].y+32){
			TriggerFx(0,0xFF,true);
			X*=-1;
			Y=(((pBall->y+4) - (Paddle[0].y+16))*(0.23/2));

		}else if(pBall->x+8 > Paddle[1].x && pBall->x+5 < Paddle[1].x && pBall->y+8 > Paddle[1].y && pBall->y < Paddle[1].y+32){
			TriggerFx(0,0xFF,true);
			X*=-1;
			Y=(((pBall->y+4) - (Paddle[1].y+16))*(0.23/2));
		}

	WaitVsync(1);
}

void ProcessBall( void ){
	static double XX = 116, YY = 108;

	if(Start == true){
		X = -1.5;
		Start = false;
		BallOut = false;
	}

	XX+=X;
	YY+=Y;
	pBall->x=XX;
	pBall->y=YY;

	if(pBall->x < 1 || pBall->x > 229){
		TriggerFx(2,0xFF,true);
		BallOut = true;
		XX = 116;
		YY = 108;
		X = 0;
		Y = 0;

		if(pBall->x < 1){
			Score[1]++;
		}else{
			Score[0]++;
		}

	}else if(pBall->y > 210 || pBall->y < 9){
		TriggerFx(1,0xFF,true);
		Y*=-1;
	}
	
}

