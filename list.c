#include "List.h"
#include <stdlib.h>
#include <string.h>

// Reallocate memory for the list when capacity is exceeded
static void Reallocate(struct List *l) {
    l->capacity = l->capacity == 0 ? 1 : l->capacity * 2; // Double the capacity
    void **new_data = (void **) malloc(sizeof(void *) * l->capacity);
    if (l->data) {
        memcpy(new_data, l->data, l->count * sizeof(void *));
        free(l->data);
    }
    l->data = new_data;
}

// Add an element to the list
void List_Add(struct List *l, void *element) {
    if (l->count == l->capacity) {
        Reallocate(l);
    }
    l->data[l->count] = element;
    l->count += 1;
}

// Free the list and its data
void List_Free(struct List *l) {
    if (l->data) {
        free(l->data);
    }
    l->capacity = 0;
    l->count = 0;
    l->data = NULL;
}

// Get an element from the list by index
void *List_Get(struct List *l, int index) {
    if (index < 0 || index >= l->count) {
        return NULL; // Out of bounds
    }
    return l->data[index];
}

// Initialize the list
void List_Init(struct List *l) {
    l->capacity = 16; // Initial capacity
    l->count = 0;
    l->data = malloc(sizeof(void *) * l->capacity);
    if (!l->data) {
        // Handle malloc failure
        exit(1);
    }
}

// Remove an element from the list by index
void List_Remove(struct List *l, int index) {
    if (index < 0 || index >= l->count) {
        return; // Out of bounds
    }
    // Shift elements to the left
    for (int i = index; i < l->count - 1; i++) {
        l->data[i] = l->data[i + 1];
    }
    l->count -= 1;
}

// Find the index of an element in the list
int List_Find(struct List *l, void *element, ListElementsEqualityFunction equals) {
    for (int i = 0; i < l->count; i++) {
        if (equals(l->data[i], element)) {
            return i;
        }
    }
    return -1; // Not found
}
