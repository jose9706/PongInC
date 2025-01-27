#include "ball.h"

void DrawBall(pBall ball)
{
    DrawCircle(ball->renderer, ball->locX, ball->locY, ball->radiusSize);
}

void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
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