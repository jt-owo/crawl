#include "level.h"
#include <stdlib.h>
#include <ncurses.h>
#include "calc.h"

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
            l->tiles[x][y] = tile_create(WALL_CHAR, TRUE);
    
    Rect r = {0};
    r.w = (rand() % 15) + 5;
    r.h = (rand() % 5) + 5;
    r.x = rand() % (MAP_W - r.w - 2) + 1;
    r.y = rand() % (MAP_H - r.h - 2) + 1;
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

    // Connectors
    Corridor c = {0};
    Rect rChild = {0};
    for (int i = 0; i < CORRIDOR_TRIES; ++i)
    {
        c = find_room_corridor_child(r);
        if (!corridor_fits(&c))
            continue;
        
        rChild = room_from_corridor(&c);
        if (!room_fits(l, &rChild))
            continue;

        level_add_corridor(l, &c);
        level_add_room(l, &rChild);
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