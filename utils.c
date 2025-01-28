#include "utils.h"

int GetRandomStartSpeed(int max, int min)
{
    srand(time(NULL));
    int base = (rand() % max + 1);
    if (base < min)
    {
        base = min;
    }
    if ((rand() % 2))
    {
        base = -base;
    }
    return base;
}