#pragma once
#include "SDL2/SDL.h"
#include "players.h"

typedef struct ball
{
    int locX;
    int locY;
    int vx; // velocity x axis.
    int vy; // velocity y axis.
    SDL_Renderer *renderer;
    int radiusSize;
} ball, *pBall;

void DrawBall(pBall ball);

void HandleBallMovement(pBall ball);

void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius);