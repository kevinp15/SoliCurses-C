#include <stdlib.h>
#include "../include/array.h"

void Array_append(Array * a, card * c) {
    if (a->used == a->size) 
    {
        a->size *= 2;
        a->array = (card **) realloc(a->array, a->size * sizeof(card *));
    }

    a->array[a->used++] = c;
}

void Array_deallocate(Array * a)
{
    free(a->array);
    a->array = NULL;
    a->used = 0;
    a->size = 0;
}

void Array_init(Array * a, size_t initsize)
{
    a->array = (card **) malloc(initsize * sizeof(card *));
    a->used = 0;
    a->size = initsize;
}
