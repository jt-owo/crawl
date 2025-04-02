#include "level.h"
#include <stdlib.h>
#include <ncurses.h>

Level* level_new(Level* parent)
{
    Level* l = malloc(sizeof(Level));
    l->prev = parent;
    l->next = NULL;

    if (l->prev != NULL)
        l->depth = l->prev->depth + 1;
    else
        l->depth = 1;

    for (int y = 0; y < MAP_H; ++y)
    {
        for (int x = 0; x < MAP_W; ++x)
        {
            if (rand() % 2)
                l->tiles[x][y] = tile_create('#', TRUE);
            else
                l->tiles[x][y] = tile_create(' ', TRUE);
        }
    }
    
    return l;
}