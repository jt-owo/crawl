#include "player.h"
#include "gui.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Player* player_new(enum Class c)
{
    Player* p = malloc(sizeof(Player));
    p->base = entity_new();
    p->base->name       = "Player";
    p->class            = c;
    p->gold             = 0;
    p->xp               = 0;
    p->level            = 0;
    p->base->sight      = 5;
    p->base->baseHP     = 30;
    p->base->baseMP     = 0;
    p->base->baseSTR    = 20;
    p->base->baseTOU    = 20;
    p->base->baseATT    = 20;
    p->base->baseDEF    = 20;
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
}

void player_status(Player* p)
{
    const char* const HP      = " HP: ";
    const char* const MP      = " MP: ";
    const char* const GP      = " GP: ";
    const char* const SLASH   = "/";

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
        case WIZARD:
            return "WIZARD";
        default: 
            return "UNKNOWN_CLASS";
    }
}