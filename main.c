#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

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

SDL_Rect GetARect(int x, int y, int h, int w)
{
    SDL_Rect rect;
    rect.h = h;
    rect.w = w;
    rect.x = x;
    rect.y = y;
    return rect;
}

enum EVENT_TYPE HandleEvent(SDL_Event *ev)
{
    switch (ev->type)
    {
    case SDL_QUIT:
        return EXIT;
    case SDL_KEYDOWN:
        return KEYBOARD_DOWN;

    default:
        printf("UNKNOWN EVENT HAPPENED... %d\n", ev->type);
        return WTF;
    }
}

bool IsMovementKeyPress(SDL_Event *ev)
{
    return ev->key.keysym.scancode == SDL_SCANCODE_RIGHT || ev->key.keysym.scancode == SDL_SCANCODE_LEFT || ev->key.keysym.scancode == SDL_SCANCODE_DOWN || ev->key.keysym.scancode == SDL_SCANCODE_UP;
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

int DrawRect(SDL_Renderer *renderer, SDL_Rect *rectToDraw)
{
    if (SDL_SetRenderDrawColor(renderer, WHITE) != 0)
    {
        SDL_ERROR_PRINTF("Failing setting renderer color for some reason!!!")
        return TERRIBLE_FAILURE;
    }
    if (SDL_RenderFillRect(renderer, rectToDraw) != 0)
    {
        SDL_ERROR_PRINTF("Failing setting renderer color for some reason!!!")
        return TERRIBLE_FAILURE;
    }

    return NO_FAIL;
}

void CheckOrUpdateBounds(SDL_Rect *rect)
{
    if (rect->y < 0)
    {
        rect->y = 0;
    }
    // Rectangles get drawn from a point at y axis. Hence to not leave the screen from the bottom half we have to take y + height = bottom edge pixels and make sure those dont leave the screen.
    // Then since we force it to not leave we use SCREEN_HEIGHT - h to get the max location to start the y axis rectangle draw.
    if (rect->y + rect->h > SCREEN_HEIGHT)
    {
        rect->y = SCREEN_HEIGHT - rect->h;
    }
}

void UpdateRectMovementDown(SDL_Rect *rect)
{
    rect->y += PAD_MOVING_SPEED;
    CheckOrUpdateBounds(rect);
}

void UpdateRectMovementUp(SDL_Rect *rect)
{
    rect->y -= PAD_MOVING_SPEED;
    CheckOrUpdateBounds(rect);
}

void DrawPlayers(SDL_Renderer *renderer, SDL_Window *window, SDL_Rect **players, int count)
{
    for (size_t i = 0; i < count; i++)
    {
        if (DrawRect(renderer, players[i]) == TERRIBLE_FAILURE)
        {
            CloseWindowAndExitFromThisPain(window, renderer);
        }
    }
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
    SDL_Rect *p1_rect = (SDL_Rect *)malloc(sizeof(SDL_Rect));
    if (p1_rect == NULL)
    {
        printf("FAILING ALLOCATING P1 MEMORY AHHHH...\n");
        CloseWindowAndExitFromThisPain(window, renderer);
        return 0;
    }
    p1_rect->h = PAD_HEIGHT;
    p1_rect->w = PAD_WIDTH;
    p1_rect->x = 50;
    p1_rect->y = 50;
    SDL_Rect *p2_rect = (SDL_Rect *)malloc(sizeof(SDL_Rect));
    if (p2_rect == NULL)
    {
        printf("FAILING ALLOCATING P2 MEMORY AHHHH...\n");
        CloseWindowAndExitFromThisPain(window, renderer);
        return 0;
    }
    p2_rect->h = PAD_HEIGHT;
    p2_rect->w = PAD_WIDTH;
    p2_rect->x = 600;
    p2_rect->y = 100;

    SDL_Rect *players[] = {p1_rect, p2_rect};
    int loc = 50;
    int running = true;

    while (running)
    {
        while (SDL_PollEvent(&event)) // This will process every event before any frames!!
        {
            if (HandleEvent(&event) == EXIT)
            {
                running = false;
            }
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        const Uint8 *keyState = SDL_GetKeyboardState(NULL);
        if (keyState[SDL_SCANCODE_DOWN])
        {
            UpdateRectMovementDown(p1_rect);
        }
        if (keyState[SDL_SCANCODE_UP])
        {
            UpdateRectMovementUp(p1_rect);
        }
        if (keyState[SDL_SCANCODE_W])
        {
            UpdateRectMovementUp(p2_rect);
        }
        if (keyState[SDL_SCANCODE_S])
        {
            UpdateRectMovementDown(p2_rect);
        }
        if (ClearScreen(renderer) == TERRIBLE_FAILURE)
        {
            CloseWindowAndExitFromThisPain(window, renderer);
        }
        DrawPlayers(renderer, window, players, PLAYER_COUNT);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // 60 fps ?
        loc++;
    }

    free(p1_rect);
    free(p2_rect);
    CloseWindowAndExitFromThisPain(window, renderer);
    return 0;
}