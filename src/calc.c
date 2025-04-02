#include "calc.h"
#include <ncurses.h>

void noise()
{
    for (int y = 0; y < LINES; ++y)
        for (int x = 0; x < COLS; ++x)
            mvaddch(y, x, NOISE_CHAR);
}