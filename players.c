#include "players.h"

pPlayer InitPlayer(const int padWidth, const int padHeight, const char *name, const int x, const int y)
{
    pPlayer player = (pPlayer)malloc(sizeof(struct player));
    if (player == NULL)
    {
        printf("Failed allocating memory for player.");
        return NULL;
    }
    player->locX = x;
    player->locY = y;
    player->name = (char *)malloc(10 * sizeof(char));
    strcpy(player->name, name);
    player->pad = NULL;
    player->score = 0;
    if (CreatePlayerPad(player, padWidth, padHeight) == -1)
    {
        printf("Failing creating player pad.\n");
        return NULL;
    }
    printf("Created player %s", player->name);
    return player;
}

void CheckOrUpdatePlayerBounds(SDL_Rect *rect, const int screenHeight)
{
    if (rect->y < 0)
    {
        rect->y = 0;
    }
    // Rectangles get drawn from a point at y axis. Hence to not leave the screen from the bottom half we have to take y + height = bottom edge pixels and make sure those dont leave the screen.
    // Then since we force it to not leave we use SCREEN_HEIGHT - h to get the max location to start the y axis rectangle draw.
    if (rect->y + rect->h > screenHeight)
    {
        rect->y = screenHeight - rect->h;
    }
}

int HandlePlayerMovement(const Uint8 *keyboardState, pPlayers players, const int screenHeight)
{
    if (keyboardState[SDL_SCANCODE_DOWN])
    {
        UpdateRectMovementDown(players->p1->pad, screenHeight);
    }
    if (keyboardState[SDL_SCANCODE_UP])
    {
        UpdateRectMovementUp(players->p1->pad, screenHeight);
    }
    if (keyboardState[SDL_SCANCODE_W])
    {
        UpdateRectMovementUp(players->p2->pad, screenHeight);
    }
    if (keyboardState[SDL_SCANCODE_S])
    {
        UpdateRectMovementDown(players->p2->pad, screenHeight);
    }
    return 0;
}

void UpdateRectMovementDown(SDL_Rect *rect, const int screenHeight)
{
    rect->y += PAD_MOVING_SPEED;
    CheckOrUpdatePlayerBounds(rect, screenHeight);
}

void UpdateRectMovementUp(SDL_Rect *rect, const int screenHeight)
{
    rect->y -= PAD_MOVING_SPEED;
    CheckOrUpdatePlayerBounds(rect, screenHeight);
}

int DrawPlayers(const pPlayers players)
{
    if (SDL_SetRenderDrawColor(players->renderer, WHITE) != 0)
    {
        SDL_ERROR_PRINTF("Failing setting renderer color for some reason!!!")
        return -1;
    }
    if (SDL_RenderFillRect(players->renderer, players->p1->pad) != 0)
    {
        SDL_ERROR_PRINTF("Failed drawing P1 pad!!!")
        return -1;
    }
    if (SDL_RenderFillRect(players->renderer, players->p2->pad) != 0)
    {
        SDL_ERROR_PRINTF("Failed drawing P2 pad!!!")
        return -1;
    }

    return 1;
}

int CreatePlayerPad(pPlayer player, const int width, const int height)
{
    SDL_Rect *pad = (SDL_Rect *)malloc(sizeof(SDL_Rect));
    if (pad == NULL && player->name != NULL)
    {
        printf("FAILING ALLOCATING %s MEMORY AHHHH...\n", player->name);
        return -1;
    }
    pad->h = height;
    pad->w = width;
    pad->x = player->locX;
    pad->y = player->locY;
    player->pad = pad;
    return 1;
}