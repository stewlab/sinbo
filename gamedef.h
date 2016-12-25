#ifndef GAMEDEF_H
#define GAMEDEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "physics.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define PADDLE_WIDTH 75
#define NUM_PADDLES 1
#define NUM_BALLS 1

/* ball diameter */
#define BALL_DM 15

#define MAX_BALL_SPEED 4

SDL_Surface *screen;
SDL_Surface *background;
SDL_Surface *paddle;
SDL_Surface *ball;

typedef struct paddle_s {
	int x, y;
	int dx;
	int balldocked;
	int direction;
} paddle_t, *paddle_p;

typedef struct ball_s {
	int x, y;
	int dx, dy;
} ball_t, *ball_p;

paddle_t p[NUM_PADDLES];
ball_t b[NUM_BALLS];

extern double time_scale;

/*####################
	PROTOTYPES
#####################*/
void DrawGraphics();
void InitBall();
int LoadGameData();
void UnloadGameData();
void GameLoop();

#endif
