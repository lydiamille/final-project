//ideas.c
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ideas.h"


struct node *add_to_beginning(struct node *first, struct note value);
struct node *add_to_end(struct node *last, struct note value);
void print_list(struct node *first);
void *my_malloc(size_t size);

void *my_malloc(size_t size) {
    
    void *p;
    p = malloc(size);
    if(p == NULL) {
        printf("ERROR: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

struct node *add_to_beginning(struct node *first, struct note value) {

    struct node *new_node;
    new_node = my_malloc(sizeof(struct node));
    
    new_node->value = value;
    new_node->next = first;

    return new_node;
}

struct node *add_to_end(struct node *last, struct note value) {

    struct node *new_node;
    new_node = my_malloc(sizeof(struct node));

    new_node->value = value;
    new_node->next = NULL;
    last->next = new_node;
    
    return new_node;
}

void print_list(struct node *first) {
    
    struct node *p;

    for(p = first; p != NULL; p = p->next)
        printf("%d ", p->value.number);

    printf("\n");
}

