#include "player.h"
#include "stdlib.h"

Player* player_new()
{
    Player* p = malloc(sizeof(Player));
    p->base = entity_new();
    return p;
}

void player_free(Player* p)
{
    if (p == NULL) return;
    if (p->base != NULL)
        entity_free(p->base);
    free(p);
}

void player_move(Player* p, Point pos)
{
    p->base->pos = pos;
}