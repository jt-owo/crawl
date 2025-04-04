#include "tile.h"
#include <stdlib.h>

Tile tile_create(char c, bool isVisible)
{
    Tile t = {
        .c = c,
        .isVisible = isVisible
    };

    return t;
}

bool tile_trans(const Tile* t)
{
    return !(t->c == WALL_CHAR || t->c == CLOSED_DOOR_CHAR);
}