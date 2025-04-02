#include "gui.h"
#include <stdlib.h>
#include <ncurses.h>

void gui_init(void)
{
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();

    if (COLS < 80 || LINES < 24)
    {
        endwin();
        fprintf(stderr, "ERROR: This game requires a terminal size of at least 80x24\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 16; ++i)
        init_pair(i, i, COLOR_BLACK);

}

void gui_end(void)
{
    endwin();
}