#include "game.h"
#include "gui.h"
#include "cam.h"
#include <stdlib.h>
#include <ncurses.h>

Game* game_new(void)
{
    Game* g = malloc(sizeof(Game));
    g->levels = g->current = level_new(NULL);
    g->player = player_new(KNIGHT);
    g->player->base->sight = 5;
    player_move(g->player, g->current->stairsUp);
    level_fov(g->current, g->player->base, g->player->base->sight);
    cam_center(g->player->base->pos);
    player_status(g->player);
    
    g->state = MAP_WALK;
    g->running = TRUE;
    return g;
}

void game_free(Game* g)
{
    if (g == NULL) return;
    if (g->current != NULL)
    {
        if (g->current->goldPiles != NULL)
        {
            LIST_FREE(g->current->goldPiles);
        }
    }
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
        else if (key == 'l')
            state = CAM_MOVE;
        else if (key == 'i')
            state = INFO_SCREEN;
        else
            handle_input(g, key);
        
        if (state != g->state)
            game_state(g, state);
    }
}

void game_state(Game* g, enum GameState state)
{
    if (g->state == state)
        return;

    g->state = state;

    switch (state)
    {
        case MAP_WALK:
            cam_center(g->player->base->pos);
            gui_redraw(g);
            player_status(g->player);
            break;
        case CAM_MOVE:
            g_selector = g->player->base->pos;
            gui_redraw(g);
            break;
        case INFO_SCREEN:
            gui_draw_info(g);
            break;
    }
}

void game_ascend(Game* g)
{
    if (g->current->prev == NULL)
        return;

    g->current = g->current->prev;
    
    player_move(g->player, g->current->stairsDown);
    level_fov(g->current, g->player->base, g->player->base->sight);
    cam_center(g->player->base->pos);
    gui_redraw(g);
}


void game_descend(Game* g)
{
    if (g->current->next == NULL)
        g->current->next = level_new(g->current);

    g->current = g->current->next;
    
    player_move(g->player, g->current->stairsUp);
    level_fov(g->current, g->player->base, g->player->base->sight);
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
        level_fov(g->current, g->player->base, g->player->base->sight);
        cam_center(g->player->base->pos);
    }
    else if (t->c == CLOSED_DOOR_CHAR)
    {
        t->c = OPEN_DOOR_CHAR;
        level_fov(g->current, g->player->base, g->player->base->sight);
    }

    player_status(g->player);
    gui_redraw(g);
}

void game_interact(Game* g)
{
    Gold* found = NULL;
    int foundIndex = 0;
    Gold* gold = NULL;
    for (int i = 0; i < g->current->goldPiles->size; ++i)
    {
        LIST_GET(g->current->goldPiles, i, gold);
        if (gold == NULL) continue;
        if (pteq(gold->pos, g->player->base->pos))
        {
            found = gold;
            foundIndex = i;
            break;
        }
    }

    if (found == NULL)
    {
        gui_status("Nothing to interact");
    }
    else
    {
        g->player->gold += found->amount;
        int size = digits(found->amount) + 16;
        char* str = calloc(0, sizeof(char));
        snprintf(str, size, "Picked up %d gold", found->amount);
        LIST_REMOVE(Gold, g->current->goldPiles, foundIndex);
        //Gold **tmp = calloc(g->current->goldPiles->size - 1, sizeof(Gold));
//
        //if (foundIndex != 0)
        //    memcpy(tmp, g->current->goldPiles->data, foundIndex * sizeof(Gold));
//
        //if (foundIndex != (g->current->goldPiles->size - 1))
        //    memcpy(tmp + foundIndex, g->current->goldPiles->data + foundIndex + 1, (g->current->goldPiles->size - foundIndex - 1) * sizeof(Gold *));
//
        //g->current->goldPiles->data = tmp;

        gui_status(str);
        free(str);
    }
}

const char* game_tile_description(Game* g, Point p)
{
    static const char* UNKNOWN = "Unexplored";

    if (p.x > MAP_W)
        return UNKNOWN;
    if (p.y > MAP_H)
        return UNKNOWN;
    
    Tile* t = &g->current->tiles[p.x][p.y];
    if (!t->isVisible)
        return UNKNOWN;
    
    if (pteq(p, g->player->base->pos))
        return class_name(g->player->class);
    
    Gold* gold = NULL;
    for (size_t i = 0; i < g->current->goldPiles->size; i++)
    {
        LIST_GET(g->current->goldPiles, i, gold);
        if (gold == NULL || !pteq(p, gold->pos))
            continue;
        
        int size = digits(gold->amount) + 6;
        char* goldStr = calloc(size, sizeof(char));
        snprintf(goldStr, size, "%d gold", gold->amount);
        return goldStr;
    }
    
    if (pteq(p, g->current->stairsUp))
        return "Stairs up";
    else if (pteq(p, g->current->stairsDown))
        return "Stairs down";
    else if (t->c == FLOOR_CHAR)
        return "A floor";
    else if (t->c == WALL_CHAR)
        return "A wall";
    else if (t->c == OPEN_DOOR_CHAR)
        return "Open door";
    else if (t->c == CLOSED_DOOR_CHAR)
        return "Closed door";
    return "UNKNOWN";
}

void handle_input(Game* g, int key)
{
    switch (g->state)
    {
        case MAP_WALK:
            if (key == '<')
            {
                if (pteq(g->player->base->pos, g->current->stairsUp))
                    game_ascend(g);
            }
            else if (key == '>')
            {
                if (pteq(g->player->base->pos, g->current->stairsDown))
                    game_descend(g);
            }
            else if (key == 'w')
                game_move(g, NORTH);
            else if (key == 's')
                game_move(g, SOUTH);
            else if (key == 'd')
                game_move(g, EAST);
            else if (key == 'a')
                game_move(g, WEST);
            else if (key == 'f')
                game_interact(g);
            break;
        case CAM_MOVE:
            if (key == KEY_UP)
                cam_move(g, NORTH);
            else if (key == KEY_DOWN)
                cam_move(g, SOUTH);
            else if (key == KEY_RIGHT)
                cam_move(g, EAST);
            else if (key == KEY_LEFT)
                cam_move(g, WEST);
            else if (key == 'w')
                cam_move_selector(g, NORTH);
            else if (key == 's')
                cam_move_selector(g, SOUTH);
            else if (key == 'd')
                cam_move_selector(g, EAST);
            else if (key == 'a')
                cam_move_selector(g, WEST);
            break;
        case INFO_SCREEN:
            break;
    }
}