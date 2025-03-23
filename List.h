#ifndef MINIC_LIST_H
#define MINIC_LIST_H

#include <stdbool.h>

// Function pointer type for comparing list elements
typedef bool (*ListElementsEqualityFunction)(void *, void *);

// List structure
struct List {
    int capacity;    // Maximum number of elements the list can hold
    int count;       // Current number of elements in the list
    void **data;     // Array of pointers to the elements
};

// Function prototypes

// Add an element to the list
void List_Add(struct List *l, void *element);

// Free the list and its data
void List_Free(struct List *l);

// Get an element from the list by index
void *List_Get(struct List *l, int index);

// Initialize the list with default capacity
void List_Init(struct List *l);

// Remove an element from the list by index
void List_Remove(struct List *l, int index);

// Find the index of an element in the list
int List_Find(struct List *l, void *element, ListElementsEqualityFunction equals);

#endif // MINIC_LIST_H
