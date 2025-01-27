#pragma once
#include "SDL2/SDL.h"
#include "utils.h"
#define WHITE 255, 255, 255, 255
#define PAD_MOVING_SPEED 10

typedef struct player
{
    char *name;
    int locX;
    int locY;
    SDL_Rect *pad;
    int score;

} player, *pPlayer;
typedef struct players
{
    pPlayer p1;
    pPlayer p2;
    SDL_Renderer *renderer;
} players, *pPlayers;

pPlayer InitPlayer(const int padWidth, const int padHeight, const char *name, const int x, const int y);

int HandlePlayerMovement(const Uint8 *keyboardState, pPlayers players, const int screenHeight);

void UpdateRectMovementUp(SDL_Rect *rect, const int screenHeight);

void UpdateRectMovementDown(SDL_Rect *rect, const int screenHeight);

int DrawPlayers(const pPlayers players);

int CreatePlayerPad(pPlayer player, const int width, const int height);