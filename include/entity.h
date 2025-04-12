#ifndef _ENTITY_H
#define _ENTITY_H

#include "calc.h"
#include "color.h"

typedef struct Entity
{
    Point           pos;
    char*           name;
    char            symbol;
    enum Color      color;
    int             sight;
    int             xpValue;
    int             mvcost;

    int             hp, baseHP;
    int             mp, baseMP;
    int             str, baseSTR;
    int             tou, baseTOU;
    int             att, baseATT;
    int             def, baseDEF;
    int             mag, baseMAG;
    int             wil, baseWIL;
} Entity;

/* Creates a new entity and allocates the memory. */
Entity* entity_new();
/* Frees the memory of an entity. */
void entity_free(Entity* e);

#endif