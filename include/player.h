#ifndef _PLAYER_H
#define _PLAYER_H

#include "entity.h"
#include "equipment.h"

typedef struct Affinity
{
    int             hp;
    int             mp;
    int             str;
    int             tou;
    int             att;
    int             def;
    int             mag;
    int             wil;
    int             mv;
    int             sight;
} Affinity;

enum Class
{
    KNIGHT = 1,
    ROGUE,
    WIZARD,
    LAST_CLASS
};

enum Race
{
    HUMAN = 1,
    ELF,
    DWARF,
    ORC,
    NAGA,
    LAST_RACE
};

extern Affinity ClassAffinities[LAST_CLASS];
extern Affinity RaceAffinities[LAST_RACE];

typedef struct Player
{
    Entity*         base;
    enum Class      class;
    enum Race       race;
    Affinity        affinity;
    int             gold;
    int             xp;
    int             level;
    int             nextLevel;

    Equipment*      weapon;
    Equipment*      helm;
    Equipment*      body;
    Equipment*      hands;
    Equipment*      feet;
    Equipment*      ring1;
    Equipment*      ring2;
    Equipment*      neck;
} Player;

/* Creates a new player and base entity and allocates the memory. */
Player* player_new(const char* name, enum Class c, enum Race r);
/* Frees the memory of an entity. */
void player_free(Player* p);
/* Moves a player around. (NO COLLISION CHECK INCLUDED) */
void player_move(Player* p, Point pos);
/* Fully restores a player based on the base stats. */
void player_restore(Player* p);
/* Levels up a player and increases the affinity. */
void player_levelup(Player* p);
/* Fills the status line with player info. */
void player_status(Player* p);

/* Returns the name of a class. */
const char* class_name(enum Class c);

/* Returns the name of the race. */
const char* race_name(enum Race r);

/* Sums up all class/race stats. */
Affinity sumaff(enum Class c, enum Race r);

#endif