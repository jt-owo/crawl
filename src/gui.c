#include "gui.h"
#include <cam.h>
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

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
}

void gui_draw(Game* g)
{
    int y2, x2;
    char c;

    for (int y = 0; y < LINES; ++y)
    {
        for (int x = 0; x < COLS; ++x)
        {
            x2 = x + g_cam.x;
            y2 = y + g_cam.y;

            Tile t = g->current->tiles[x2][y2];
            if (x2 >= MAP_W || x2 < 0 || y2 >= MAP_H || y2 < 0)
                c = EMPTY_CHAR;
            else
                c = t.c;

            if (c == CLOSED_DOOR_CHAR || c == OPEN_DOOR_CHAR)
                attron(COLOR_PAIR(2));
            else
                attron(COLOR_PAIR(1));

            mvaddch(y, x, c);
        }
    }

    // Special objects
    attron(COLOR_PAIR(1));

    if (gui_is_onscr(g->current->stairsUp))
        gui_draw_obj_relative(g->current->stairsUp, '<');
    
    if (gui_is_onscr(g->current->stairsDown))
        gui_draw_obj_relative(g->current->stairsDown, '>');
    
    refresh();
}

void gui_draw_obj_relative(Point p, char c)
{
    mvaddch(p.y - g_cam.y, p.x - g_cam.x, c);
}

bool gui_is_onscr(Point p)
{
    if (p.x - g_cam.x < 0)
        return FALSE;
    if (p.y - g_cam.y < 0)
        return FALSE;
    if (p.x - g_cam.x >= COLS)
        return FALSE;
    if (p.y - g_cam.y >= LINES - 1)
        return FALSE;

    return TRUE;
}

void gui_end(void)
{
    endwin();
}