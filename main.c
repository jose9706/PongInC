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

#define SDL_ERROR_PRINTF(s)           \
    do                                \
    {                                 \
        printf(s);                    \
        printf("%s", SDL_GetError()); \
    } while (0);

enum EVENT_TYPE
{
    KEYBOARD,
    EXIT
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
    if (ev->type == SDL_QUIT)
    {
        return EXIT;
    }
}

int ClearScreen(SDL_Renderer *renderer)
{
    if (SDL_SetRenderDrawColor(renderer, BLACK) || SDL_RenderClear(renderer))
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
    }

    SDL_Event event;

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
        }
        if (ClearScreen(renderer) == TERRIBLE_FAILURE)
        {
            CloseWindowAndExitFromThisPain(window, renderer);
        }
        SDL_Rect rect;
        rect.h = 50;
        rect.w = 100;
        rect.x = loc;
        rect.y = loc;
        SDL_SetRenderDrawColor(renderer, WHITE); // Red
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // 60 fps ?
        loc++;
    }

    CloseWindowAndExitFromThisPain(window, renderer);
    return 0;
}
