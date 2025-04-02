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