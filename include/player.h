#ifndef _PLAYER_H
#define _PLAYER_H

#include "entity.h"

enum Class
{
    KNIGHT,
    WIZARD
};

typedef struct Player
{
    Entity*         base;
    enum Class      class;
    int             gold;
    int             xp;
    int             level;
} Player;

/* Creates a new player and base entity and allocates the memory. */
Player* player_new(enum Class c);
/* Frees the memory of an entity. */
void player_free(Player* p);
/* Moves a player around. (NO COLLISION CHECK INCLUDED) */
void player_move(Player* p, Point pos);
/* Fully restores a player based on the base stats. */
void player_restore(Player* p);
/* Fills the status line with player info. */
void player_status(Player* p);

/* Returns the name of a class. */
const char* class_name(enum Class c);

#endif