//ideas.c
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ideas.h"

struct note note1, note2, note3;

struct node *add_to_beginning(struct node *first, struct note value);
struct node *add_to_end(struct node *last, struct note value);
void print_list(struct node *first);
void *my_malloc(size_t size);

/*int main(void) {

    note1.number = 1;
    note2.number = 2;
    note3.number = 3;

    struct node *first;
    struct node *last;

    first = malloc(sizeof(struct node));
    first->value = note2;
    first->next = NULL;
    last = first;

    last = add_to_end(last, note3);
    first = add_to_beginning(first, note1);


    print_list(first);
}*/

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
    new_node->next[0] = first;

    return new_node;
}

struct node *add_to_end(struct node *last, struct note value) {

    struct node *new_node;
    new_node = my_malloc(sizeof(struct node));

    new_node->value = value;
    new_node->next[0] = NULL;
    last->next[0] = new_node;
    
    return new_node;
}


void print_list(struct node *first) {
    
    struct node *p;

    for(p = first; p != NULL; p = p->next[0])
        printf("%d ", p->value.number);

    printf("\n");
}

