#pragma once
#include "SDL2/SDL.h"
#include <stdbool.h>

#define SDL_ERROR_PRINTF(s)           \
    do                                \
    {                                 \
        printf(s);                    \
        printf("%s", SDL_GetError()); \
    } while (0);