#pragma once
#include "SDL2/SDL.h"
#include "utils.h"
#include "ball.h"
#include "players.h"

typedef struct gameInfo
{
    int screenWidth;
    int screenHeight;
    int startBallLocX;
    int startBallLocY;

    pPlayers players;
    pBall ball;

} gameInfo, *pGameInfo;

pGameInfo InitGameInfo(int screenWidth, int screenHeight, pPlayers players, pBall ball);

bool CheckAndUpdateScore(pGameInfo gameInfo);

void ResetGame();