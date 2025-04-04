#include "gui.h"
#include "cam.h"
#include "calc.h"
#include <stdlib.h>
#include <string.h>
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

    move(0, 0);

    for (int y = 0; y < LINES; ++y)
    {
        for (int x = 0; x < COLS; ++x)
        {
            x2 = x + g_cam.x;
            y2 = y + g_cam.y;

            Tile t = g->current->tiles[x2][y2];
            if (x2 >= MAP_W || x2 < 0 || y2 >= MAP_H || y2 < 0)
                c = EMPTY_CHAR;
            else if (!t.isVisible)
                c = EMPTY_CHAR;
            else
                c = t.c;

            if (c == CLOSED_DOOR_CHAR || c == OPEN_DOOR_CHAR)
                attron(COLOR_PAIR(2));
            else
                attron(COLOR_PAIR(1));

            addch(c);
        }
    }

    // Special objects
    attron(COLOR_PAIR(1));

    gui_draw_obj_conditionally(g->current, g->current->stairsUp, '<');
    gui_draw_obj_conditionally(g->current, g->current->stairsDown, '>');
    gui_draw_obj_conditionally(g->current, g->player->base->pos, '@');

    refresh();
}

void gui_redraw(Game* g)
{
    if (g->state == MAP_WALK)
        gui_draw(g);
}

void gui_draw_obj_relative(Point p, char c)
{
    mvaddch(p.y - g_cam.y, p.x - g_cam.x, c);
}

void gui_draw_obj_conditionally(Level* l, Point p, char c)
{
    if (!gui_is_onscr(p))
        return;
    if (!l->tiles[p.x][p.y].isVisible)
        return;
    gui_draw_obj_relative(p, c);
}

void gui_alert(const char* msg)
{
    static const char* continueMSG = "Press spacebar to continue.";

    WINDOW* win = NULL;
    Rect area = {0};
    int c;

    area.w = MAX(strlen(msg), strlen(continueMSG)) + 4;
    area.w = MIN(area.w, 80);
    area.h = 5;
    area.x = (COLS - area.w) / 2;
    area.y = (LINES - area.h) / 2;

    win = newwin(area.h, area.w, area.y, area.x);
    if (win == NULL)
    {
        fprintf(stderr, "Error making window for an alert!");
        exit(EXIT_FAILURE);
    }

    box(win, 0, 0);
    wrefresh(win);
    refresh();

    area.x = (area.w - strlen(msg)) / 2;
    mvwprintw(win, 2, area.x, msg);
    area.x = (area.w - strlen(continueMSG)) / 2;
    mvwprintw(win, 4, area.x, continueMSG);
    wrefresh(win);
    refresh();

    while ((c = wgetch(win)) != ' ');

    delwin(win);
}

bool gui_confirm(const char* msg, char a, char b)
{
    WINDOW* win = NULL;
    Rect area = {0};
    char c = 0;

    int size = strlen(msg) + 6;
    char* fmt = calloc(size, sizeof(char));
    snprintf(fmt, size, "%s %c/%c", msg, a, b);

    area.w = strlen(fmt) + 4;
    area.h = 3;
    area.x = (COLS - area.w) / 2;
    area.y = (LINES - area.h) / 2;

    win = newwin(area.h, area.w, area.y, area.x);
    if (win == NULL)
    {
        fprintf(stderr, "Error making window for confirm.");
        exit(EXIT_FAILURE);
    }

    box(win, 0, 0);
    area.x = (area.w - strlen(fmt)) / 2;
    mvwprintw(win, 1, area.x, fmt);
    wrefresh(win);
    refresh();

    
    while (1)
    {
        c = wgetch(win);
        if (c == a || c == b)
        break;
    }
    
    free(fmt);
    delwin(win);

    return c == a;
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