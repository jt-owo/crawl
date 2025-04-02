#ifndef _TILE_H
#define _TILE_H

#define bool _Bool

typedef struct Tile
{
    char c;
    bool isVisible;
} Tile;

/* Creates a new tile. */
Tile tile_create(char c, bool isVisible);

#endif