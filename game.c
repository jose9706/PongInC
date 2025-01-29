#include "game.h"

pGameInfo InitGameInfo(int screenWidth, int screenHeight, pPlayers players, pBall ball)
{
    pGameInfo gameInfo = (pGameInfo)malloc(sizeof(struct gameInfo));
    gameInfo->ball = ball;
    gameInfo->players = players;
    gameInfo->screenHeight = screenHeight;
    gameInfo->screenWidth = screenWidth;
    gameInfo->startBallLocX = screenWidth / 2;
    gameInfo->startBallLocY = screenHeight / 2;
    return gameInfo;
}

bool CheckAndUpdateScore(pGameInfo gameInfo)
{
    pBall ball = gameInfo->ball;
    pPlayers players = gameInfo->players;
    pPlayer scoringPlayer = CheckBoundsForGoal(ball, gameInfo->screenWidth, players);
    if (scoringPlayer != NULL)
    {
        // SOME ONE SCORED.
        scoringPlayer->score++;
        printf("%s scored!! Score is now %d - %d", scoringPlayer->name, gameInfo->players->p1->score, gameInfo->players->p2->score);
        return true;
    }
    HandleBallMovement(ball, players, gameInfo->screenWidth, gameInfo->screenHeight);
    return false;
}

void ResetGame(pGameInfo gameInfo)
{
    // Reset ball
    pBall ball = gameInfo->ball;
    ball->locX = gameInfo->startBallLocX;
    ball->locY = gameInfo->startBallLocY;
    DrawBall(ball);
}

void CheckTicksAndIncreaseSpeed(int *ticks, pGameInfo gameInfo)
{
    if (*ticks == TICKS_TO_INCREASE_SPEED)
    {
        gameInfo->ball->vx += 3;
        *ticks = 0;
    }
}
