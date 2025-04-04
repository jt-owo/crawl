#ifndef _TILE_H
#define _TILE_H

#define bool _Bool

#define EMPTY_CHAR          ' '
#define FLOOR_CHAR          '.'
#define WALL_CHAR           '#'
#define OPEN_DOOR_CHAR      '-'
#define CLOSED_DOOR_CHAR    '+'


typedef struct Tile
{
    char c;
    bool isVisible;
} Tile;

/* Creates a new tile. */
Tile tile_create(char c, bool isVisible);

#endif