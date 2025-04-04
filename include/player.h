#ifndef _PLAYER_H
#define _PLAYER_H

#include "entity.h"

typedef struct Player
{
    Entity* base;
    int xp;
} Player;

/* Creates a new player and base entity and allocates the memory. */
Player* player_new();
/* Frees the memory of an entity. */
void player_free(Player* p);
/* Moves a player around. (NO COLLISION CHECK INCLUDED) */
void player_move(Player* p, Point pos);
#endif