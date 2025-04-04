#ifndef _LEVEL_H
#define _LEVEL_H

#include "tile.h"
#include "entity.h"
#include "calc.h"

/* Width of a level. */
#define MAP_W   320
/* Height of a level. */
#define MAP_H   100

typedef struct Corridor
{
    Rect                area;
    enum Direction      direction;
} Corridor;

typedef struct Level
{
    int                 depth;
    Tile                tiles[MAP_W][MAP_H];
    Point               stairsUp;
    Point               stairsDown;
    struct Level*       prev;
    struct Level*       next;
} Level;

/* Generates a new level. */
Level* level_new(Level* parent);
/* Frees the memory of a level */
void level_free(Level* l);
/* Adds a room to a level. */
void level_add_room(Level* l, Rect* r);
/* Adds a corridor to a level. */
void level_add_corridor(Level* l, Corridor* c);
/* Calculates the field of view for an entity. */
void level_fov(Level* l, Entity* e, float distance);
/* Calculates the field of view in a triangular direction. */
void level_fov_calc(Level* l, enum Direction d, int x, int y, float distance);
/* Checks if a room area fits in a level. */
bool room_fits(Level* l, Rect* r);
/* Checks if a corridor area fits in a level. */
bool corridor_fits(Corridor* c);
/* Attempts to find a corridor of a connected room. */
Corridor find_room_corridor_child(Rect* r);
/* Attempts to find a room via a corridor. */
Rect room_from_corridor(Corridor* c);

#endif