#ifndef _ENTITY_H
#define _ENTITY_H

#include "calc.h"

typedef struct Entity
{
    Point pos;
    int sight;

    int hp, baseHP;
    int mp, baseMP;
    int str, baseSTR;
    int tou, baseTOU;
    int att, baseATT;
    int def, baseDEF;
} Entity;

/* Creates a new entity and allocates the memory. */
Entity* entity_new();
/* Frees the memory of an entity. */
void entity_free(Entity* e);

#endif