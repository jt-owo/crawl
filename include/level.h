#ifndef _LEVEL_H
#define _LEVEL_H

#include "tile.h"

/* Width of a level. */
#define MAP_W   320
/* Height of a level. */
#define MAP_H   320

typedef struct Level
{
    int                 depth;
    Tile                tiles[MAP_W][MAP_H];
    struct Level*       prev;
    struct Level*       next;
} Level;

/* Generates a new level. */
Level* level_new(Level* parent);

#endif