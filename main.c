#include <SDL2/SDL.h>
#include <stdio.h>
#include "ball.h"
#include "players.h"
#include "utils.h"

#define SUBSYSTEMS SDL_INIT_VIDEO | SDL_INIT_EVENTS
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define WHITE 255, 255, 255, 255
#define RED 255, 0, 0, 255
#define GREEN 0, 255, 0, 255
#define BLUE 0, 0, 255, 255
#define BLACK 0, 0, 0, 255
#define TERRIBLE_FAILURE 4206969
#define NO_FAIL 6969420
#define PAD_HEIGHT 200
#define PAD_WIDTH 25
#define PLAYER_COUNT 2
#define PAD_MOVING_SPEED 10
#define BALL_RADIUS 20

#define SDL_ERROR_PRINTF(s)           \
    do                                \
    {                                 \
        printf(s);                    \
        printf("%s", SDL_GetError()); \
    } while (0);

enum EVENT_TYPE
{
    KEYBOARD_DOWN,
    EXIT,
    WTF
};

void FinishSDL()
{
    SDL_QuitSubSystem(SUBSYSTEMS);
    SDL_Quit();
}

int ClearScreen(SDL_Renderer *renderer)
{
    if (SDL_SetRenderDrawColor(renderer, BLACK) != 0 || SDL_RenderClear(renderer) != 0)
    {
        SDL_ERROR_PRINTF("Failing clearing the screen for some reason!!!")
        return TERRIBLE_FAILURE;
    }

    return NO_FAIL;
}

void CloseWindowAndExitFromThisPain(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    FinishSDL();
}

int main()
{
    if (SDL_Init(SUBSYSTEMS))
    {
        SDL_ERROR_PRINTF("Failed misserably for unknown reasons starting SDL...")
        return 0;
    }
    SDL_Window *window = SDL_CreateWindow("The window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (!window)
    {
        SDL_ERROR_PRINTF("Window is NULL, printing error and failing")
        return 0;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        SDL_ERROR_PRINTF("Failing creating renderer...")
        return 0;
    }

    SDL_Event event;
    pPlayer p1 = InitPlayer(PAD_WIDTH, PAD_HEIGHT, "Player 1", 50, 50);
    if (p1 == NULL)
    {
        printf("Failed creating player 1.");
        CloseWindowAndExitFromThisPain(window, renderer);
    }
    pPlayer p2 = InitPlayer(PAD_WIDTH, PAD_HEIGHT, "Player 2", SCREEN_WIDTH - 50 - PAD_WIDTH, 100);
    if (p2 == NULL)
    {
        printf("Failed creating player 1.");
        CloseWindowAndExitFromThisPain(window, renderer);
    }

    pBall theBall = (pBall)malloc(sizeof(struct ball));
    theBall->locX = SCREEN_WIDTH / 2;
    theBall->locY = SCREEN_HEIGHT / 2;
    theBall->radiusSize = BALL_RADIUS;
    theBall->renderer = renderer;
    theBall->vx = 10;
    theBall->vy = 3;
    theBall->leavingPad = false;

    pPlayers players_s = (pPlayers)malloc(sizeof(struct players));
    players_s->p1 = p1;
    players_s->p2 = p2;
    players_s->renderer = renderer;
    int running = true;

    while (running)
    {
        while (SDL_PollEvent(&event)) // This will process every event before any frames!!
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        const Uint8 *keyState = SDL_GetKeyboardState(NULL);
        HandlePlayerMovement(keyState, players_s, SCREEN_HEIGHT);
        HandleBallMovement(theBall, players_s, SCREEN_WIDTH, SCREEN_HEIGHT);
        if (ClearScreen(renderer) == TERRIBLE_FAILURE)
        {
            CloseWindowAndExitFromThisPain(window, renderer);
        }
        DrawPlayers(players_s);
        DrawBall(theBall);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // 60 fps ?
    }

    CloseWindowAndExitFromThisPain(window, renderer);
    return 0;
}