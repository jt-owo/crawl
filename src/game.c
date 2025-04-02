#include "game.h"
#include "cam.h"
#include <stdlib.h>
#include <ncurses.h>

Game* game_new(void)
{
    Game* g = malloc(sizeof(Game));
    g->levels = g->current = level_new(NULL);
    g->state = MAP_WALK;
    return g;
}

void game_run(Game* g)
{
    int key;
    enum GameState state = g->state;

    while (1)
    {
        key = getch();

        if (key == 'z')
            state = MAP_WALK;
        else if (key == 'm')
            state = MAP_INFO;
        else
            handle_input(g, key);
        
        if (state != g->state)
            game_state(g, state);
    }
}

void game_render(Game* g)
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
                c = ' ';
            else
                c = t.c;

            mvaddch(y, x, c);
        }
    }
    
    refresh();
}

void game_render_info(Game* g)
{
    noise();
    mvprintw(0, 0, "Depth: %d", g->current->depth);
}

void game_state(Game* g, enum GameState state)
{
    g->state = state;

    switch (state)
    {
        case MAP_WALK:
            game_render(g);
            break;
        case MAP_INFO:
            game_render_info(g);
            break;
    }
}

void game_ascend(Game* g)
{
    if (g->current->prev == NULL)
        return;

    g->current = g->current->prev;
    game_render(g);
}


void game_descend(Game* g)
{
    if (g->current->next == NULL)
        g->current->next = level_new(g->current);

    g->current = g->current->next;
    game_render(g);
}

void handle_input(Game* g, char key)
{
    switch (g->state)
    {
        case MAP_WALK:
            if (key == '<')
                game_ascend(g);
            else if (key == '>')
                game_descend(g);
            else if (key == 'w')
                cam_move(g, NORTH);
            else if (key == 's')
                cam_move(g, SOUTH);
            else if (key == 'd')
                cam_move(g, EAST);
            else if (key == 'a')
                cam_move(g, WEST);
            break;
        default: break;
    }
}