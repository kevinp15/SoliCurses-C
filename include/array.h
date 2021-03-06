#pragma once

#include "card.h"

typedef struct
{
    card ** array;
    size_t used;
    size_t size;
} Array;

// Returns last item in an array
// Segfault possible if a->used is 0
card * Array_last(Array * a);

// Adds object to end of array with size of array considered
void Array_append(Array * a, card * c);

// Deallocates a.array and sets a.used and a.size equal to 0
void Array_deallocate(Array * a);

// Constructor method for array
void Array_init(Array * a, size_t initsize);

// Removes all elements in an array following and including index i
void Array_recursive_remove(Array * a, int i);
