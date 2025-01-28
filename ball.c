#include "ball.h"

void DrawBall(pBall ball)
{
    DrawCircle(ball->renderer, ball->locX, ball->locY, ball->radiusSize);
}

void ProcessBallHittingPad(pBall ball, const pPlayers players)
{
    SDL_Rect *p1Rect = players->p1->pad;
    SDL_Rect *p2Rect = players->p2->pad;
    // Check if ball intersects with player 1's rectangle
    if (ball->locX + ball->radiusSize >= p1Rect->x &&
        ball->locX - ball->radiusSize <= p1Rect->x + p1Rect->w)
    {
        if (ball->locY + ball->radiusSize >= p1Rect->y &&
            ball->locY - ball->radiusSize <= p1Rect->y + p1Rect->h)
        {
            // Move the ball to the closest edge and flip the speed
            if (ball->locX < p1Rect->x + p1Rect->w / 2)
            {
                ball->locX = p1Rect->x - ball->radiusSize; // Move to the left side
            }
            else
            {
                ball->locX = p1Rect->x + p1Rect->w + ball->radiusSize; // Move to the right side
            }
            ball->vx = -ball->vx;

            // Check if hitting the top or bottom half of the pad
            if (ball->locY < p1Rect->y + p1Rect->h / 2)
            {
                ball->vy = -abs(ball->vy); // Hitting top half, move up
            }
            else
            {
                ball->vy = abs(ball->vy); // Hitting bottom half, move down
            }
        }
    }

    // Check if ball intersects with player 2's rectangle
    if (ball->locX + ball->radiusSize >= p2Rect->x &&
        ball->locX - ball->radiusSize <= p2Rect->x + p2Rect->w)
    {
        if (ball->locY + ball->radiusSize >= p2Rect->y &&
            ball->locY - ball->radiusSize <= p2Rect->y + p2Rect->h)
        {
            // Move the ball to the closest edge and flip the speed
            if (ball->locX < p2Rect->x + p2Rect->w / 2)
            {
                ball->locX = p2Rect->x - ball->radiusSize; // Move to the left side
            }
            else
            {
                ball->locX = p2Rect->x + p2Rect->w + ball->radiusSize; // Move to the right side
            }
            ball->vx = -ball->vx;

            // Check if hitting the top or bottom half of the pad
            if (ball->locY < p2Rect->y + p2Rect->h / 2)
            {
                ball->vy = -abs(ball->vy); // Hitting top half, move up
            }
            else
            {
                ball->vy = abs(ball->vy); // Hitting bottom half, move down
            }
        }
    }
}

void HandleBallMovement(pBall ball, const pPlayers players, const int screenWidth, const int screenHeight)
{
    ball->locX += ball->vx;
    ball->locY += ball->vy;
    ProcessBallHittingPad(ball, players);
    ProcessBallBounceOffEdges(ball, screenHeight);
}

void ProcessBallBounceOffEdges(pBall ball, const int screenHeight)
{
    if (ball->locY - ball->radiusSize < 0)
    {
        ball->locY = 0 + ball->radiusSize;
        ball->vy = -ball->vy;
    }

    if (ball->locY + ball->radiusSize > screenHeight)
    {
        ball->locY = screenHeight - ball->radiusSize;
        ball->vy = -ball->vy;
    }
}

pPlayer CheckBoundsForGoal(pBall ball, const int screenWidth, const pPlayers players)
{
    if (ball->locX - ball->radiusSize < 0)
    {
        ball->locX = 0 + ball->radiusSize;
        ball->vx = -ball->vx;
        return players->p2;
    }

    if (ball->locX + ball->radiusSize > screenWidth)
    {
        ball->locX = screenWidth - ball->radiusSize;
        ball->vx = -ball->vx;
        return players->p1;
    }

    return NULL;
}

// TODO add error managment.
void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    if (SDL_SetRenderDrawColor(renderer, GREEN) != 0)
    {
    }
    SDL_Point points[1300]; // Maybe use vectors here ?? but we allocate to stack anyways ig.
    int count = 0;
    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            if (x * x + y * y <= radius * radius)
            { // Check if point is within the circle
                SDL_Point point;
                point.x = centreX + x;
                point.y = centreY + y;
                points[count] = point;
                count++;
            }
        }
    }
    SDL_RenderDrawPoints(renderer, points, count); // Supposed to be faster to just call this once.. ?
}