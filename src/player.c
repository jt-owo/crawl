#include "player.h"
#include "gui.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Affinity ClassAffinities[LAST_CLASS] = {
    //HP    MP  STR TOU ATT DEF MAG WIL MV  SIGHT
    {10,    0,  5,  5,  5,  5,  0,  5, 0,  0},  //KNIGHT
    {6,     0,  2,  2,  2,  2,  0,  5, 0,  0},  //ROGUE
    {4,     10, 1,  1,  1,  1,  15, 7, 0,  0},  //MAGE
};

Affinity RaceAffinities[LAST_RACE] = {
    //HP    MP  STR TOU ATT DEF MAG WIL MV  SIGHT
    {10,    0,  5,  5,  5,  5,  0,  5,  0,  5},  //HUMAN
    {5,     2,  4,  3,  7,  6,  1,  5,  -5, 7},  //ELF
    {15,    0,  5,  8,  5,  5,  -2, 10,  5,  6},  //DWARF
    {10,    0,  8,  7,  5,  5,  0,  5,  0,  7},  //ORC
    {10,    0,  6,  6,  5,  5,  0,  5,  0,  6},  //NAGA
};

Player* player_new(const char* name, enum Class c, enum Race r)
{
    Player* p = malloc(sizeof(Player));
    p->base = entity_new();
    p->base->name       = (char*)name;
    p->base->symbol     = '@';
    p->base->color      = GREEN;
    p->class            = c;
    p->race             = r;
    p->gold             = 0;
    p->xp               = 0;
    p->level            = 0;
    p->nextLevel        = 1000;
    p->affinity = sumaff(c, r);
    p->base->sight      = p->affinity.sight;
    p->base->baseHP     = p->affinity.hp;
    p->base->baseMP     = p->affinity.mp;
    p->base->baseSTR    = p->affinity.str;
    p->base->baseTOU    = p->affinity.tou;
    p->base->baseATT    = p->affinity.att;
    p->base->baseDEF    = p->affinity.def;
    p->base->baseMAG    = p->affinity.mag;
    p->base->baseWIL    = p->affinity.wil;
    p->base->mvcost     = 1000 + p->affinity.mv;

    p->weapon           = NULL;
    p->helm             = NULL;
    p->body             = NULL;
    p->hands            = NULL;
    p->feet             = NULL;
    p->ring1            = NULL;
    p->ring2            = NULL;
    p->neck             = NULL;

    player_restore(p);
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

void player_restore(Player* p)
{
    p->base->hp = p->base->baseHP;
    p->base->mp = p->base->baseMP;
    p->base->str = p->base->baseSTR;
    p->base->tou = p->base->baseTOU;
    p->base->att = p->base->baseATT;
    p->base->def = p->base->baseDEF;
    p->base->mag = p->base->baseMAG;
    p->base->wil = p->base->baseWIL;
}

void player_levelup(Player* p)
{
    p->level++;
    if (p->affinity.hp > 0)
        p->base->baseHP += rand() % p->affinity.hp;
    if (p->affinity.mp > 0)
        p->base->baseMP += rand() % p->affinity.mp;
    if (p->affinity.str > 0)
        p->base->baseSTR += rand() % p->affinity.str;
    if (p->affinity.tou > 0)
        p->base->baseTOU += rand() % p->affinity.tou;
    if (p->affinity.att > 0)
        p->base->baseATT += rand() % p->affinity.att;
    if (p->affinity.def > 0)
        p->base->baseDEF += rand() % p->affinity.def;
    if (p->affinity.mag > 0)
        p->base->baseMAG += rand() % p->affinity.mag;
    if (p->affinity.wil > 0)
        p->base->baseWIL += rand() % p->affinity.wil;

    p->nextLevel = p->level * p->level * 1000;
    player_restore(p);
}

void player_status(Player* p)
{
    static const char* const HP      = "HP: ";
    static const char* const MP      = " MP: ";
    static const char* const GP      = " GP: ";
    static const char* const SLASH   = "/";

    bool hasMana = p->base->baseMP < 0;

    int size = strlen(HP) + digits(p->base->hp) + strlen(SLASH) + digits(p->base->baseHP) +
               (hasMana ? 0 : strlen(MP) + digits(p->base->mp) + strlen(SLASH) + digits(p->base->baseHP)) +
               strlen(GP) + digits(p->gold) + 1;
    char* str = calloc(size, sizeof(char));
    if (hasMana)
    {
        snprintf(str, size, "%s%d%s%d%s%d%s%d%s%d", HP, p->base->hp, SLASH, p->base->baseHP,
                                                    MP, p->base->mp, SLASH, p->base->baseMP,
                                                    GP, p->gold);
    }
    else
    {
        snprintf(str, size, "%s%d%s%d%s%d", HP, p->base->hp, SLASH, p->base->baseHP,
                                            GP, p->gold);
    }

    gui_status(str);
    free(str);
}

const char* class_name(enum Class c)
{
    switch (c)
    {
        case KNIGHT:
            return "KNIGHT";
        case ROGUE:
            return "ROGUE";
        case WIZARD:
            return "WIZARD";
        default: 
            return "UNKNOWN_CLASS";
    }
}

const char* race_name(enum Race r)
{
    switch (r)
    {
        case HUMAN:
            return "HUMAN";
        case ELF:
            return "ELF";
        case DWARF:
            return "DWARF";
        case ORC:
            return "ORC";
        case NAGA:
            return "NAGA";
        default:
            return "UNKNOWN_RACE";
    }
}

Affinity sumaff(enum Class c, enum Race r)
{
    Affinity* a = &ClassAffinities[c];
    Affinity* b = &RaceAffinities[r];

    Affinity ret = {

        .hp = a->hp + b->hp,
        .mp = a->mp + b->mp,
        .str = a->str + b->str,
        .tou = a->tou + b->tou,
        .att = a->att + b->att,
        .def = a->def + b->def,
        .mag = a->mag + b->mag,
        .wil = a->wil + b->wil,
        .mv = a->mv + b->mv,
        .sight = a->sight + b->sight
    };

    return ret;
}