#ifndef _LEVEL_H
#define _LEVEL_H

#include "tile.h"
#include "calc.h"

/* Width of a level. */
#define MAP_W   200
/* Height of a level. */
#define MAP_H   50

#define EMPTY_CHAR          ' '
#define FLOOR_CHAR          '.'
#define WALL_CHAR           '#'
#define OPEN_DOOR_CHAR      '-'
#define CLOSED_DOOR_CHAR    '+'

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
/* Checks if a room area fits in a level. */
bool room_fits(Level* l, Rect* r);
/* Checks if a corridor area fits in a level. */
bool corridor_fits(Corridor* c);
/* Attempts to find a corridor of a connected room. */
Corridor find_room_corridor_child(Rect* r);
/* Attempts to find a room via a corridor. */
Rect room_from_corridor(Corridor* c);

#endif