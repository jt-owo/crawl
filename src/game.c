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
    g->player = player_new();
    g->player->base->sight = 5;
    player_move(g->player, g->current->stairsUp);
    level_fov(g->current, g->player->base);
    cam_center(g->player->base->pos);
    return g;
}

void game_free(Game* g)
{
    if (g == NULL) return;
    if (g->levels != NULL)
        level_free(g->levels);
    if (g->player != NULL)
        player_free(g->player);
    free(g);
}

void game_run(Game* g)
{
    int key;
    enum GameState state = g->state;

    while (g->running)
    {
        key = getch();

        if (key == KEY_F(12))
        {
            g->running = !gui_confirm("Do you want to quit?", 'y', 'n');
            if (g->running)
                gui_redraw(g);
        }
            
        if (key == '`')
            state = MAP_WALK;
        else if (key == 'i')
            state = INFO_SCREEN;
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
        case INFO_SCREEN:
            game_render_info(g);
            break;
    }
}

void game_ascend(Game* g)
{
    if (g->current->prev == NULL)
        return;

    g->current = g->current->prev;
    
    player_move(g->player, g->current->stairsUp);
    level_fov(g->current, g->player->base);
    cam_center(g->player->base->pos);
    gui_redraw(g);
}


void game_descend(Game* g)
{
    if (g->current->next == NULL)
        g->current->next = level_new(g->current);

    g->current = g->current->next;
    
    player_move(g->player, g->current->stairsDown);
    level_fov(g->current, g->player->base);
    cam_center(g->player->base->pos);
    gui_redraw(g);
}

void game_move(Game* g, enum Direction d)
{
    Point target = g->player->base->pos;
    if (d == NORTH)
        target.y--;
    if (d == SOUTH)
        target.y++;
    if (d == EAST)
        target.x++;
    if (d == WEST)
        target.x--;

    Tile* t = &g->current->tiles[target.x][target.y];

    if (t->c == FLOOR_CHAR || t->c == OPEN_DOOR_CHAR)
    {
        player_move(g->player, target);
        level_fov(g->current, g->player->base);
        cam_center(g->player->base->pos);
        gui_redraw(g);
    }
    else if (t->c == CLOSED_DOOR_CHAR)
    {
        t->c = OPEN_DOOR_CHAR;
        gui_redraw(g);
    }

}

void handle_input(Game* g, int key)
{
    switch (g->state)
    {
        case MAP_WALK:
            if (key == '<')
                game_ascend(g);
            else if (key == '>')
                game_descend(g);
            else if (key == 'w')
                game_move(g, NORTH);
            else if (key == 's')
                game_move(g, SOUTH);
            else if (key == 'd')
                game_move(g, EAST);
            else if (key == 'a')
                game_move(g, WEST);
            else if (key == KEY_UP)
                cam_move(g, NORTH);
            else if (key == KEY_DOWN)
                cam_move(g, SOUTH);
            else if (key == KEY_RIGHT)
                cam_move(g, EAST);
            else if (key == KEY_LEFT)
                cam_move(g, WEST);
            break;
        case INFO_SCREEN:
            break;
    }
}