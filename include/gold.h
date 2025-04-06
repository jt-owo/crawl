#ifndef _GOLD_H
#define _GOLD_H

#include <stddef.h>
#include "calc.h"

typedef struct Gold
{
    Point pos;
    int amount;
} Gold;

typedef struct GoldPiles
{
    Gold** data;
    size_t size;
} GoldPiles;

#endif