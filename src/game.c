#include "game.h"
#include "gui.h"
#include "cam.h"
#include <stdlib.h>
#include <ncurses.h>

Game* game_new(void)
{
    Game* g = malloc(sizeof(Game));
    g->levels = g->current = level_new(NULL);
    g->state = MAP_WALK;
    g->running = TRUE;
    return g;
}

void game_free(Game* g)
{
    if (g->levels)
        level_free(g->levels);
    free(g);
}

void game_run(Game* g)
{
    int key;
    enum GameState state = g->state;

    while (g->running)
    {
        key = getch();

        if (key == 'q')
        {
            g->running = !gui_confirm("Do you want to quit?", 'y', 'n');
            if (g->running)
                gui_redraw(g);
        }
            
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

void game_render_info(Game* g)
{
    noise();
    mvprintw(0, 0, "Depth: %d", g->current->depth);
}

void game_state(Game* g, enum GameState state)
{
    if (g->state == state)
        return;

    g->state = state;

    switch (state)
    {
        case MAP_WALK:
            gui_draw(g);
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
    g_cam.x = 0;
    g_cam.y = 0;
    gui_draw(g);
}


void game_descend(Game* g)
{
    if (g->current->next == NULL)
        g->current->next = level_new(g->current);

    g->current = g->current->next;
    g_cam.x = 0;
    g_cam.y = 0;
    gui_draw(g);
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
        case MAP_INFO:
            break;
    }
}