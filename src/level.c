#include "level.h"
#include "gold.h"
#include "calc.h"
#include <stdlib.h>
#include <ncurses.h>

// REMOVE
#include "gui.h"

#define CORRIDOR_TRIES      30

Level* level_new(Level* parent)
{
    Level* l = malloc(sizeof(Level));
    l->prev = parent;
    l->next = NULL;

    if (l->prev != NULL)
        l->depth = l->prev->depth + 1;
    else
        l->depth = 1;

    // Blank all tiles.
    for (int y = 0; y < MAP_H; ++y)
        for (int x = 0; x < MAP_W; ++x)
            l->tiles[x][y] = tile_create(WALL_CHAR, FALSE);
    
    Rect r = {0};
    r.w = (rand() % 15) + 5;
    r.h = (rand() % 5) + 5;
    r.x = rand() % (MAP_W - r.w - 2) + 1;
    r.y = rand() % (MAP_H - r.h - 2) + 1;

    l->stairsUp.x = r.x + rand() % (r.w - 2) + 1;
    l->stairsUp.y = r.y + rand() % (r.h - 2) + 1;
    l->stairsDown.x = -1;

    level_add_room(l, &r);

    return l;
}

void level_free(Level* l)
{
    if (l->next != NULL)
        level_free(l->next);
    
    free(l);
}

void level_add_room(Level* l, Rect* r)
{
    // Floor
    for (int y = 0; y < r->h; ++y)
        for (int x = 0; x < r->w; ++x)
            l->tiles[x + r->x][y + r->y].c = FLOOR_CHAR;

    // Gold
    if (rand() % 100 >= 10)
        level_add_gold(l, r);

    // Monster
    if (rand() % 100  <= 70)
        level_spawn_monster(l, r);

    // Connectors
    Corridor c = {0};
    Rect rChild = {0};
    int exits = 0;
    for (int i = 0; i < CORRIDOR_TRIES; ++i)
    {
        c = find_room_corridor_child(r);
        if (!corridor_fits(&c))
            continue;
        
        rChild = room_from_corridor(&c);
        if (!room_fits(l, &rChild))
            continue;

        exits++;
        level_add_corridor(l, &c);
        level_add_room(l, &rChild);
    }
    
    if (l->stairsDown.x == -1 && exits == 0)
    {
        l->stairsDown.x = r->x + rand() % (r->w - 2) + 1;
        l->stairsDown.y = r->y + rand() % (r->h - 2) + 1;
    }
}

void level_add_corridor(Level* l, Corridor* c)
{
    l->tiles[c->area.x][c->area.y].c = CLOSED_DOOR_CHAR;
    l->tiles[c->area.x + c->area.w][c->area.y + c->area.h].c = CLOSED_DOOR_CHAR;
    if (c->direction == EAST || c->direction == WEST)
    {
        for (int i = 1; i < c->area.w; ++i)
            l->tiles[i + c->area.x][c->area.y].c = FLOOR_CHAR;
    }
    else
    {
        for (int i = 1; i < c->area.h; ++i)
            l->tiles[c->area.x][i + c->area.y].c = FLOOR_CHAR;        
    }
}

void level_add_gold(Level* l, Rect* r)
{
    if (l->goldPiles == NULL)
    {
        LIST(GoldPiles, l->goldPiles, 1);
    }

    Gold* g = malloc(sizeof(Gold));
    g->pos.x = r->x + rand() % r->w;
    g->pos.y = r->y + rand() % r->h;
    g->amount = rand() % ((l->depth * l->depth) + 5) + 1;
    
    LIST_SET(Gold, l->goldPiles, l->goldPiles->size, g);
}

void level_spawn_monster(Level* l, Rect* r)
{
    // TODO:
}

void level_fov(Level* l, Entity* e, float distance)
{
    level_fov_calc(l, NORTH, e->pos.x, e->pos.y, distance);
    level_fov_calc(l, SOUTH, e->pos.x, e->pos.y, distance);
    level_fov_calc(l, EAST, e->pos.x, e->pos.y, distance);
    level_fov_calc(l, WEST, e->pos.x, e->pos.y, distance);
}

void level_fov_calc(Level* l, enum Direction d, int x, int y, float distance)
{
    if (distance < 0)
        return;

    Tile* t = &l->tiles[x][y];
    t->isVisible = TRUE;
    if (!tile_trans(t))
        return;

    if (d == NORTH)
    {
        level_fov_calc(l, NW, x - 1, y - 1, distance - 1.41);
        level_fov_calc(l, NORTH, x, y - 1, distance - 1);
        level_fov_calc(l, NE, x + 1, y - 1, distance - 1.41);
    }
    else if (d == SOUTH)
    {
        level_fov_calc(l, SW, x - 1, y + 1, distance - 1.41);
        level_fov_calc(l, SOUTH, x, y + 1, distance - 1);
        level_fov_calc(l, SE, x + 1, y + 1, distance - 1.41);
    }
    else if (d == EAST)
    {
        level_fov_calc(l, NE, x + 1, y - 1, distance - 1.41);
        level_fov_calc(l, EAST, x + 1, y, distance - 1);
        level_fov_calc(l, SE, x + 1, y + 1, distance - 1.41);
    }
    else if (d == WEST)
    {
        level_fov_calc(l, NW, x - 1, y - 1, distance - 1.41);
        level_fov_calc(l, WEST, x - 1, y, distance - 1);
        level_fov_calc(l, SW, x - 1, y + 1, distance - 1.41);
    }
    else if (d == NW)
        level_fov_calc(l, NW, x - 1, y - 1, distance - 1.41);
    else if (d == NE)
        level_fov_calc(l, NE, x + 1, y - 1, distance - 1.41);
    else if (d == SW)
        level_fov_calc(l, SW, x - 1, y + 1, distance - 1.41);
    else if (d == SE)
        level_fov_calc(l, SE, x + 1, y + 1, distance - 1.41);
}

bool room_fits(Level* l, Rect* r)
{
    if (r->x <= 0 || r->y <= 0)
        return FALSE;
    if (r->x + r->w >= MAP_W)
        return FALSE;
    if (r->y + r->h >= MAP_H)
        return FALSE;

    for (int y = MAX(r->y - 1, 0); y < r->y + r->h; ++y)
        for (int x = MAX(r->x - 1, 0); x < r->x + r->w; ++x)
            if (l->tiles[x][y].c == FLOOR_CHAR)
                return FALSE;

    return TRUE;
}

bool corridor_fits(Corridor* c)
{
    if (c->area.x <= 1 || c->area.y <= 1)
        return FALSE;
    if (c->area.x + c->area.w >= MAP_W)
        return FALSE;
    if (c->area.y + c->area.h >= MAP_H)
        return FALSE;
    return TRUE;
}

Corridor find_room_corridor_child(Rect* r)
{
    Corridor c;

    if (rand() % 2) // NORTH or SOUTH
    { 
        c.area.w = 0;
        c.area.h = ((rand() % 2) + 1) * 3;
        c.area.x = r->x + rand() % r->w;
        if (rand() % 2) // NORTH
        { 
            c.direction = NORTH;
            c.area.y = r->y - 1 - c.area.h;
        }
        else // SOUTH
        { 
            c.direction = SOUTH;
            c.area.y = r->y + r->h;
        }
    }
    else // EAST or WEST
    { 
        c.area.h = 0;
        c.area.w = ((rand() % 2) + 1) * 3;
        c.area.y = r->y + rand() % r->h;
        if (rand() % 2) // WEST
        { 
            c.direction = WEST;
            c.area.x = r->x - 1 - c.area.w;
        }
        else // EAST
        { 
            c.direction = EAST;
            c.area.x = r->x + r->w;
        }
    }

    return c;
}

Rect room_from_corridor(Corridor* c)
{
    Rect r = {
        .w = (rand() % 15) + 5,
        .h = (rand() % 5) + 5,
    };

    if (r.w % 2 == 0)
        r.w++;
    if (r.h % 2 == 0)
        r.h++;
    
    if (c->direction == NORTH)
    {
        r.x = c->area.x - rand() % r.w;
        r.y = c->area.y - r.h;
    }
    else if (c->direction == SOUTH)
    {
        r.x = c->area.x - rand() % r.w;
        r.y = c->area.y + c->area.h + 1;
    }
    else if (c->direction == EAST)
    {
        r.x = c->area.x + c->area.w + 1;
        r.y = c->area.y - rand() % r.h;
    }
    else if (c->direction == WEST)
    {
        r.x = c->area.x - r.w;
        r.y = c->area.y - rand() % r.h;
    }

    return r;
}