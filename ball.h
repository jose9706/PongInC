#pragma once
#include "SDL2/SDL.h"
#include "players.h"
#include "utils.h"
#define GREEN 0, 255, 0, 255

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

void HandleBallMovement(pBall ball, const pPlayers players, const int screenWidth, const int screenHeight);

void ProcessBallBounceOffEdges(pBall ball, const int screenHeight);

// Returns player that scored. Null if no one scored.
pPlayer CheckBoundsForGoal(pBall ball, const int screenWidth, pPlayers players);

void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius);