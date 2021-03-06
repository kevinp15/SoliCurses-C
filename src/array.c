#include <stdlib.h>
#include "../include/array.h"

card * Array_last(Array * a)
{
    return a->array[a->used - 1];
}

void Array_append(Array * a, card * c) 
{
    if (a->used == a->size) 
    {
        a->size++;
        a->array = (card **) realloc(a->array, a->size * sizeof(card *));
    }

    a->array[a->used++] = c;
}

void Array_recursive_remove(Array * a, int i)
{
    int count = 0;

    while(i < a->size)
    {
        a->array[i] = NULL;
        count++;
        i++;
    }

    a->used -= count;
    a->array = realloc(a->array, a->used * sizeof(card *));
    a->size = a->used;
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
