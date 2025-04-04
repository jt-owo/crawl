#include "entity.h"
#include <stdlib.h>

Entity* entity_new()
{
    Entity* e = malloc(sizeof(Entity));

    return e;
}

void entity_free(Entity* e)
{
    if (e == NULL) return;
    free(e);
}