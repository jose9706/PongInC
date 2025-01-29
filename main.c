#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "ball.h"
#include "players.h"
#include "utils.h"
#include "game.h"

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

void CloseWindowAndExitFromThisPain(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font)
{
    TTF_CloseFont(font);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    FinishSDL();
    SDL_Quit();
    TTF_Quit();
}

void DrawTextMiddleScreen(TTF_Font *font, char scoreText[69], SDL_Renderer *renderer, bool clearScreen)
{
    SDL_Color textColor = {255, 255, 255, 255}; // White color
    // Render the text
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, scoreText, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    // Clear the screen and render the new text
    SDL_SetRenderDrawColor(renderer, BLACK);

    SDL_Rect textRect = {SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, textSurface->w, textSurface->h};
    if (clearScreen)
    {
        SDL_SetRenderDrawColor(renderer, BLACK);
        SDL_RenderClear(renderer);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, BLACK);
        SDL_RenderFillRect(renderer, &textRect);
    }

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_RenderPresent(renderer);
}

void DrawGoalScoredScreen(TTF_Font *font, SDL_Renderer *renderer, pGameInfo gameInfo)
{
    char scoreText[69];
    sprintf(scoreText, "Score: %d - %d", gameInfo->players->p1->score, gameInfo->players->p2->score);
    DrawTextMiddleScreen(font, scoreText, renderer, true);
    SDL_Delay(2000);
    SDL_RenderClear(renderer);
}

void DrawCoundown(TTF_Font *font, SDL_Renderer *renderer, pGameInfo gameInfo)
{
    char countDown[69];
    sprintf(countDown, "....3....");
    DrawTextMiddleScreen(font, countDown, renderer, false);
    SDL_Delay(1000);
    sprintf(countDown, "....2....");
    DrawTextMiddleScreen(font, countDown, renderer, false);
    SDL_Delay(1000);
    sprintf(countDown, "....1....");
    DrawTextMiddleScreen(font, countDown, renderer, false);
    SDL_Delay(1000);
}

int main()
{
    if (SDL_Init(SUBSYSTEMS))
    {
        SDL_ERROR_PRINTF("Failed misserably for unknown reasons starting SDL...")
        return 1;
    }
    if (TTF_Init() == -1)
    {
        printf("TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("./arialbi.ttf", 24); // 24 is the font size
    if (!font)
    {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("The window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (!window)
    {
        SDL_ERROR_PRINTF("Window is NULL, printing error and failing")
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        SDL_ERROR_PRINTF("Failing creating renderer...")
        return 1;
    }

    SDL_Event event;
    pPlayer p1 = InitPlayer(PAD_WIDTH, PAD_HEIGHT, "Player 1", 50, 50);
    if (p1 == NULL)
    {
        printf("Failed creating player 1.");
        CloseWindowAndExitFromThisPain(window, renderer, font);
    }
    pPlayer p2 = InitPlayer(PAD_WIDTH, PAD_HEIGHT, "Player 2", SCREEN_WIDTH - 50 - PAD_WIDTH, 100);
    if (p2 == NULL)
    {
        printf("Failed creating player 1.");
        CloseWindowAndExitFromThisPain(window, renderer, font);
    }

    pBall theBall = (pBall)malloc(sizeof(struct ball));
    theBall->locX = SCREEN_WIDTH / 2;
    theBall->locY = SCREEN_HEIGHT / 2;
    theBall->radiusSize = BALL_RADIUS;
    theBall->renderer = renderer;
    theBall->vx = GetRandomStartSpeed(13, 7);
    theBall->vy = GetRandomStartSpeed(5, 3);

    pPlayers players_s = (pPlayers)malloc(sizeof(struct players));
    players_s->p1 = p1;
    players_s->p2 = p2;
    players_s->renderer = renderer;
    int running = true;
    int ticks = 0;

    pGameInfo gameInfo = InitGameInfo(SCREEN_WIDTH, SCREEN_HEIGHT, players_s, theBall);

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
        if (CheckAndUpdateScore(gameInfo))
        {
            DrawGoalScoredScreen(font, renderer, gameInfo);
            ResetGame(gameInfo);
            DrawPlayers(gameInfo->players);
            DrawCoundown(font, renderer, gameInfo);
        }
        if (ClearScreen(renderer) == TERRIBLE_FAILURE)
        {
            CloseWindowAndExitFromThisPain(window, renderer, font);
        }
        DrawPlayers(players_s);
        DrawBall(theBall);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // 60 fps ?
        ticks++;
        CheckTicksAndIncreaseSpeed(&ticks, gameInfo);
    }

    CloseWindowAndExitFromThisPain(window, renderer, font);
    return 0;
}