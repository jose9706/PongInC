#include <SDL2/SDL.h>
#include <stdio.h>

#define SUBSYSTEMS SDL_INIT_VIDEO | SDL_INIT_EVENTS

void FinishSDL()
{
    SDL_QuitSubSystem(SUBSYSTEMS);
    SDL_Quit();
}

SDL_Rect GetARect(int x, int y, int h, int w)
{
    SDL_Rect rect;
    rect.h = 100;
    rect.w = 100;
    rect.x = 100;
    rect.y = 100;
    return rect;
}

int main()
{
    if (SDL_Init(SUBSYSTEMS))
    {
        printf("Failed misserably for unknown reasons");
        return 0;
    }
    SDL_Window *window = SDL_CreateWindow("The window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 400, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (!window)
    {
        printf("Window is NULL, printing error and failing");
        const char *error = SDL_GetError();
        printf("%s", error);
        return 0;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    SDL_Rect rect = GetARect(100, 100, 100, 100);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
    SDL_Delay(5000);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    FinishSDL();
    return 0;
}
