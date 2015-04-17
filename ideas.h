#include "file_access.h"

#ifndef IDEAS_H
#define IDEAS_H

struct node {
    struct note value;
    struct node *next[10];
};

struct node *add_to_beginning(struct node *first, struct note value);
struct node *add_to_end(struct node *last, struct note value);
void print_list(struct node *first);
void *my_malloc(size_t size);

#endif

