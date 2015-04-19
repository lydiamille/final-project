//bev.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "file_access.h"
#include "ideas.h"

void write_best_line(void);
struct note next_note(struct node *last, int chord_num);
bool in_chord(struct note current_note, struct chord chord);
bool note_fits(struct note current_note, struct beat beat);
bool doubled_third(struct note current_note, struct beat beat);

// TODO remember to fix this bug at some point: if there's a space after the last note written, the last note will be
//repeated.

int main(int argc, char *argv[]) {

    if(argc != 2) {
        printf("specify one filename\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp;

    if((fp = fopen(argv[1], "r")) == NULL) {
        printf("%s can't be opened\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    read_file(fp);
    //print_specs();
    check_bassline();
    write_best_line();

}


void write_best_line(void) {

    int i;
    struct note new_note;

    struct node *first;
    struct node *last;

    first = malloc(sizeof(struct node));
    first->value = song1.sopnote;
    first->next = NULL;
    last = first;

    for(i = 1; i < song1.num_notes; i++) {
        new_note = next_note(last, i);
        if(new_note.number != -1)
            last = add_to_end(last, new_note);
    }
    print_list(first);
}

//given the previous soprano note, and the chord number, 
//finds a possible next note for the melody and returns it
//Returns a failcase note struct if dead end
struct note next_note(struct node *last, int chord_num) {

    int i;
    struct note current_note;

    for(i = last->value.number - 2; i < last->value.number + 3; i++) {
        current_note.number = i;
        number_to_letter(current_note);
        if(note_fits(current_note, song1.bassline[chord_num]))
            return current_note;
    }
    current_note.number = -1;
    return current_note;
}

bool note_fits(struct note current_note, struct beat beat) {

    if(in_chord(current_note, beat.chord) && !doubled_third(current_note, beat))
        return true;
   

    return false;
}

bool in_chord(struct note current_note, struct chord chord) {

    if(same_note(current_note, chord.one) || same_note(current_note, chord.three) || same_note(current_note, chord.five))
            return true;
    return false;
}

bool doubled_third(struct note current_note, struct beat beat) {

    struct note third;
    third = beat.chord.three;
    if(same_note(current_note, third) && same_note(beat.bassnote, third))
       return true; 
    return false;
}


