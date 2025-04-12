#include "gui.h"
#include "cam.h"
#include "calc.h"
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

static char* status = "";

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

    for (int i = 0; i < 8; ++i)
        init_pair(i, i, COLOR_BLACK);
}

void gui_draw(Game* g)
{
    int y2, x2;
    char c;

    move(0, 0);

    for (int y = 0; y < LINES - 1; ++y)
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
            {
                attron(COLOR_PAIR(RED));
                addch(c);
                attroff(COLOR_PAIR(RED));
            }
            else
            {
                addch(c);
            }
        }
    }

    // Gold
    Gold* gold = NULL;
    for (size_t i = 0; i < g->current->goldPiles->size; i++)
    {
        LIST_GET(g->current->goldPiles, i, gold);
        if (gold == NULL)
            continue;
       gui_draw_obj_conditionally(g->current, gold->pos, '$', YELLOW);
    }

    // Special objects
    gui_draw_obj_conditionally(g->current, g->current->stairsUp, '<', WHITE);
    gui_draw_obj_conditionally(g->current, g->current->stairsDown, '>', WHITE);
    gui_draw_obj_conditionally(g->current, g->player->base->pos, g->player->base->symbol, g->player->base->color);

    refresh();
}

void gui_draw_info(Game* g)
{
    WINDOW* win = NULL;
    Rect area = {0};
    area.w = 80;
    area.h = 25;
    area.x = (COLS - area.w) / 2;
    area.y = (LINES - area.h) / 2;

    win = newwin(area.h, area.w, area.y, area.x);
    if (win == NULL)
    {
        fprintf(stderr, "Error making window for level info");
        exit(EXIT_FAILURE);
    }

    box(win, 0, 0);

    // Horizontal lines
    wmove(win, 0, 1);
    for (int i = 0; i < area.w - 2; ++i)
    {
        if ((i - 1) % 3 == 0)
            waddch(win, '|');
        else
            waddch(win, '=');
    }
    wmove(win, area.h - 1, 1);
    for (int i = 0; i < area.w - 2; ++i)
    {
        if ((i - 1) % 3 == 0)
            waddch(win, '|');
        else
            waddch(win, '=');
    }

    mvwprintw(win, 1, 2, "INFO");

    // Info
    static const char* const DEPTH      = "Depth  :: ";
    int size = strlen(DEPTH) + digits(g->current->depth) + 1;
    char* info = calloc(size, sizeof(char));
    snprintf(info, size, "%s%d", DEPTH, g->current->depth);
    mvwprintw(win, 3, 3, info);

    free(info);
    wrefresh(win);
    delwin(win);
}

void gui_draw_status()
{
    mvprintw(LINES - 1, 1, status);
    for (int i = strlen(status); i <= COLS; ++i)
        addch(' ');
}

void gui_redraw(Game* g)
{
    if (g->state == MAP_WALK)
    {
        gui_draw(g);
        gui_draw_status();
    }
    else if (g->state == CAM_MOVE)
    {
        gui_status(game_tile_description(g, g_selector));
        gui_draw(g);
        gui_draw_status();
        gui_draw_cam_selector();
    }
    else if (g->state == CHARACTER_SCREEN)
    {
        gui_draw_player_screen(g->player);
    }
    else if (g->state == INFO_SCREEN)
    {
        gui_draw_info(g);
    }
}

void gui_draw_obj_relative(Point p, char c)
{
    mvaddch(p.y - g_cam.y, p.x - g_cam.x, c);
}

void gui_draw_obj_conditionally(Level* l, Point p, char c, enum Color color)
{
    if (!gui_is_onscr(p))
        return;
    if (!l->tiles[p.x][p.y].isVisible)
        return;

    attron(COLOR_PAIR(color));
    gui_draw_obj_relative(p, c);
    attroff(COLOR_PAIR(color));
}

void gui_draw_as_overlay(Point p, char c, enum Color color)
{
    if (!gui_is_onscr(p))
        return;
    attron(COLOR_PAIR(color));
    mvaddch(p.y - g_cam.y, p.x - g_cam.x, c);
    attroff(COLOR_PAIR(color));
}

void gui_draw_cam_selector()
{
    static enum Color color = PURPLE;
    Point p = {
        .x = g_selector.x - 1,
        .y = g_selector.y
    };

    gui_draw_as_overlay(p, '-', color);
    p.x = g_selector.x + 1;
    gui_draw_as_overlay(p, '-', color);
    p.x = g_selector.x;
    p.y = g_selector.y - 1;
    gui_draw_as_overlay(p, '|', color);
    p.y = g_selector.y + 1;
    gui_draw_as_overlay(p, '|', color);
}

void gui_status(const char* msg)
{
    int size = strlen(msg) + 1;
    status = calloc(size, sizeof(char));
    snprintf(status, size, " %s", msg);
    strcpy(status, msg);
    gui_draw_status();
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

void gui_quit(Game* g)
{
    g->running = !gui_confirm("Do you really want to quit?", 'y', 'n');
    if (g->running)
        gui_redraw(g);
}

void gui_draw_player_screen(Player* p)
{
    WINDOW* win = NULL;
    Rect area = {0};
    area.w = 80;
    area.h = 25;
    area.x = (COLS - area.w) / 2;
    area.y = (LINES - area.h) / 2;

    win = newwin(area.h, area.w, area.y, area.x);
    if (win == NULL)
    {
        fprintf(stderr, "Error making window for player.");
        exit(EXIT_FAILURE);
    }
    box(win, 0, 0);

    wmove(win, 3, 1);
    for (int i = 0; i < area.w - 2; ++i)
    {
        if ((i - 1) % 3 == 0)
            waddch(win, '|');
        else
            waddch(win, '=');
    }
    wmove(win, 0, 1);
    for (int i = 0; i < area.w - 2; ++i)
    {
        if ((i - 1) % 3 == 0)
            waddch(win, '|');
        else
            waddch(win, '=');
    }
    wmove(win, area.h - 1, 1);
    for (int i = 0; i < area.w - 2; ++i)
    {
        if ((i - 1) % 3 == 0)
            waddch(win, '|');
        else
            waddch(win, '=');
    }

    // Name, Class, Race
    mvwprintw(win, 1, (area.w - strlen(p->base->name)) / 2, p->base->name);

    int size = strlen(class_name(p->class)) + strlen(race_name(p->race)) + 2;
    char* info = calloc(size, sizeof(char));
    snprintf(info, size, "%s %s", class_name(p->class), race_name(p->race));
    mvwprintw(win, 2, (area.w - strlen(info)) / 2, info);

    // Stats (kinda messy, clean up later with sub function)
    static const char* const SLASH   = "/";
    area.x = 3;
    area.y = 5;

    static const char* const HP      = "HP   :: ";
    size = strlen(HP) + digits(p->base->hp) + strlen(SLASH) + digits(p->base->baseHP) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s%d%s%d", HP, p->base->hp, SLASH, p->base->baseHP);
    mvwprintw(win, area.y, area.x, info);

    if (p->base->baseMP > 0)
    {
        static const char* const MP      = "MP   :: ";
        size = strlen(MP) + digits(p->base->mp) + strlen(SLASH) + digits(p->base->baseMP) + 1;
        info = realloc(info, size * sizeof(char));
        snprintf(info, size, "%s%d%s%d", MP, p->base->mp, SLASH, p->base->baseMP);
        mvwprintw(win, ++area.y, area.x, info);
    }

    static const char* const STR      = "STR  :: ";
    size = strlen(STR) + digits(p->base->str) + strlen(SLASH) + digits(p->base->baseSTR) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s%d%s%d", STR, p->base->str, SLASH, p->base->baseSTR);
    mvwprintw(win, area.y += 2, area.x, info);

    static const char* const TOU      = "TOU  :: ";
    size = strlen(TOU) + digits(p->base->tou) + strlen(SLASH) + digits(p->base->baseTOU) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s%d%s%d", TOU, p->base->tou, SLASH, p->base->baseTOU);
    mvwprintw(win, ++area.y, area.x, info);

    static const char* const ATT      = "ATT  :: ";
    size = strlen(ATT) + digits(p->base->att) + strlen(SLASH) + digits(p->base->baseATT) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s%d%s%d", ATT, p->base->att, SLASH, p->base->baseATT);
    mvwprintw(win, ++area.y, area.x, info);

    static const char* const DEF      = "DEF  :: ";
    size = strlen(DEF) + digits(p->base->def) + strlen(SLASH) + digits(p->base->baseDEF) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s%d%s%d", DEF, p->base->def, SLASH, p->base->baseDEF);
    mvwprintw(win, ++area.y, area.x, info);

    static const char* const MAG      = "MAG  :: ";
    size = strlen(MAG) + digits(p->base->mag) + strlen(SLASH) + digits(p->base->baseMAG) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s%d%s%d", MAG, p->base->mag, SLASH, p->base->baseMAG);
    mvwprintw(win, ++area.y, area.x, info);

    static const char* const WIL      = "WIL  :: ";
    size = strlen(WIL) + digits(p->base->wil) + strlen(SLASH) + digits(p->base->baseWIL) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s%d%s%d", WIL, p->base->wil, SLASH, p->base->baseWIL);
    mvwprintw(win, ++area.y, area.x, info);

    mvwprintw(win, area.y += 2, area.x - 1, "=|==|==|==|==|==|==|==|==|==|==|==|=");
    area.y++;

    static const char* const FOV      = "FOV  :: ";
    size = strlen(FOV) + digits(p->base->sight) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s%d", FOV, p->base->sight);
    mvwprintw(win, ++area.y, area.x, info);

    static const char* const MV       = "MV   :: ";
    size = strlen(MV) + digits(p->base->mvcost) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s%d", MV, p->base->mvcost);
    mvwprintw(win, ++area.y, area.x, info);

    area.x = 40;
    area.y = 5;
    static const char* const LVL       = "LVL  :: ";
    size = strlen(LVL) + digits(p->level) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s%d", LVL, p->level);
    mvwprintw(win, ++area.y, area.x, info);

    static const char* const XP        = "XP   :: ";
    size = strlen(XP) + digits(p->xp) + strlen(SLASH) + digits(p->nextLevel) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s%d%s%d", XP, p->xp, SLASH, p->nextLevel);
    mvwprintw(win, ++area.y, area.x, info);

    mvwprintw(win, area.y + 2, area.x - 1, "=|==|==|==|==|==|==|==|==|==|==|==|==|=");

    // Equipment
    area.y = 10;

    static const char* const WEAPON     = "WEAPON :: NONE";
    size = strlen(WEAPON) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s", WEAPON);
    mvwprintw(win, ++area.y, area.x, info);

    static const char* const HEAD       = "HEAD   :: NONE";
    size = strlen(HEAD) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s", HEAD);
    mvwprintw(win, ++area.y, area.x, info);

    static const char* const BODY       = "BODY   :: NONE";
    size = strlen(BODY) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s", BODY);
    mvwprintw(win, ++area.y, area.x, info);

    static const char* const HANDS      = "HANDS  :: NONE";
    size = strlen(HANDS) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s", HANDS);
    mvwprintw(win, ++area.y, area.x, info);

    static const char* const FEET       = "FEET   :: NONE";
    size = strlen(FEET) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s", FEET);
    mvwprintw(win, ++area.y, area.x, info);

    static const char* const RING1       = "RING1  :: NONE";
    size = strlen(RING1) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s", RING1);
    mvwprintw(win, ++area.y, area.x, info);

    static const char* const RING2       = "RING2  :: NONE";
    size = strlen(RING2) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s", RING2);
    mvwprintw(win, ++area.y, area.x, info);

    static const char* const NECK        = "NECK   :: NONE";
    size = strlen(NECK) + 1;
    info = realloc(info, size * sizeof(char));
    snprintf(info, size, "%s", NECK);
    mvwprintw(win, ++area.y, area.x, info);

    free(info);
    wrefresh(win);
    delwin(win);
}

void gui_draw_inv(Game* g)
{
    WINDOW* win = NULL;
    Rect area = {0};
    area.w = 80;
    area.h = 25;
    area.x = (COLS - area.w) / 2;
    area.y = (LINES - area.h) / 2;

    win = newwin(area.h, area.w, area.y, area.x);
    if (win == NULL)
    {
        fprintf(stderr, "Error making window for inventory.");
        exit(EXIT_FAILURE);
    }

    box(win, 0, 0);

    // Horizontal lines
    wmove(win, 0, 1);
    for (int i = 0; i < area.w - 2; ++i)
    {
        if ((i - 1) % 3 == 0)
            waddch(win, '|');
        else
            waddch(win, '=');
    }
    wmove(win, 2, 1);
    for (int i = 0; i < area.w - 2; ++i)
    {
        if ((i - 1) % 3 == 0)
            waddch(win, '|');
        else
            waddch(win, '=');
    }
    wmove(win, area.h - 1, 1);
    for (int i = 0; i < area.w - 2; ++i)
    {
        if ((i - 1) % 3 == 0)
            waddch(win, '|');
        else
            waddch(win, '=');
    }

    mvwprintw(win, 1, 2, "INVENTORY");

    static const char *const GOLD = "Gold   :: ";
    int size = strlen(GOLD) + digits(g->player->gold) + 1;
    char* info = calloc(size, sizeof(char));
    snprintf(info, size, "%s%d", GOLD, g->player->gold);
    mvwprintw(win, 1, area.w - 2 - strlen(info), info);

    free(info);
    wrefresh(win);
    delwin(win);
}

void gui_resize(Game* g)
{
    refresh();
    gui_redraw(g);
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
    if (status != NULL)
        free(status);
    endwin();
}