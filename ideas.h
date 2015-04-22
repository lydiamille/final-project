#include "file_access.h"

#ifndef IDEAS_H
#define IDEAS_H

struct node {
    struct note value;
    struct node *next;
};

//adds a node to the beginning of the linked list
struct node *add_to_beginning(struct node *first, struct note value);
//adds a node to the end
struct node *add_to_end(struct node *last, struct note value);
//prints each note in the list
void print_list(struct node *first);
//allocates mem, and also checks to make sure malloc didn't return NULL
void *my_malloc(size_t size);

#endif

