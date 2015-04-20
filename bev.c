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
bool note_fits(struct note current_note, struct beat beat, int chord_num);
bool doubled_third(struct note current_note, struct beat beat);
int parallels(struct note current_note, struct beat beat, int chord_num);
bool correct_res(struct note current_note, struct beat beat, int chord_num);

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

    first = my_malloc(sizeof(struct node));
    first->value = song1.bassline[0].sopnote;
    first->next = NULL;
    last = first;

    for(i = 1; i < song1.num_notes; i++) {
        new_note = next_note(last, i);
        if(new_note.number != -1) {
            last = add_to_end(last, new_note);
            song1.bassline[i].sopnote = new_note;
        }
        else {
            printf("\nUnfortunately, there exists no soprano line that follows your\n");
            printf("input specifications (and thus whims of your heart) that also\n");
            printf("follows part writing rules (and thus the whims of Bach\'s\n");
            printf("heart). Sucks for you.\n\n");
            exit(EXIT_FAILURE);
        }
    }
    print_list(first);
}

//given (a pointer to) the previous soprano note, and the chord number,
//finds a possible next note for the melody and returns it
//Returns a failcase note struct if dead end
struct note next_note(struct node *last, int chord_num) {

    int i;
    struct note current_note;
    bool first_half;
    int radius;

    if(chord_num < (song1.num_notes - song1.num_notes/2))
        first_half = true;
    else
        first_half = false;

    for(radius = 4; radius <= 12; radius += 2) {
        if(first_half)
            for(i = last->value.number + radius; i > last->value.number - (radius+1); i--) {
                current_note.number = i;
                current_note = number_to_letter(current_note);
                if(note_fits(current_note, song1.bassline[chord_num], chord_num))
                    return current_note;
            }
        else
            for(i = last->value.number - radius; i < last->value.number + (radius+1); i++) {
                current_note.number = i;
                current_note = number_to_letter(current_note);
                if(note_fits(current_note, song1.bassline[chord_num], chord_num))
                    return current_note;
            }
    }

    current_note.number = -1;
    return current_note;
}

//returns true if this is an acceptable next note (ie the note is in the chord and the third isn't doubled)
bool note_fits(struct note current_note, struct beat beat, int chord_num) {

    bool last_chord = false;

    if(chord_num + 1 == song1.num_notes)
        last_chord = true;

    if(in_chord(current_note, beat.chord) && !doubled_third(current_note, beat) && !parallels(current_note, beat, chord_num)) {
        if(last_chord)
            if(!correct_res(current_note, beat, chord_num))
                return false;
        return true;
    }
    return false;
}

//returns true if the note is a member of the chord
bool in_chord(struct note current_note, struct chord chord) {

    if(same_note(current_note, chord.one) || same_note(current_note, chord.three) || same_note(current_note, chord.five))
        return true;
    return false;
}

//returns true if the third is indeed doubled for the given beat and note
bool doubled_third(struct note current_note, struct beat beat) {

    struct note third;
    third = beat.chord.three;
    if(same_note(current_note, third) && same_note(beat.bassnote, third))
        return true;
    return false;
}

//returns 5 if there are parallel fifths, 8 if parallel octaves, and 0 if it checks out.
int parallels(struct note current_note, struct beat beat, int chord_num) {

    song1.bassline[chord_num].interval = song1.bassline[chord_num].sopnote.number - song1.bassline[chord_num].bassnote.number;
    if(song1.bassline[chord_num-1].interval == 7 && song1.bassline[chord_num].interval == 7)
        return 5;
    if(song1.bassline[chord_num-1].interval == 12 && song1.bassline[chord_num].interval == 12)
        return 8;

    return 0;
}

//returns true if correct resolution at cadence
bool correct_res(struct note current_note, struct beat beat, int chord_num) {

    int penultimate = song1.bassline[chord_num-1].sopnote.number % 12;

    int one = song1.key.number % 12;
    int two = (song1.key.number + 2) % 12;
    int three;
    int four = (song1.key.number + 5) % 12;
    int five = (song1.key.number + 7) % 12;
    int seven = (song1.key.number + 11) % 12;

    if(major)
        three = (song1.key.number + 4) % 12;
    else
        three = (song1.key.number + 3) % 12;


    if(penultimate == two) {
        if(current_note.number == one)
            return true;
        else
            return false;
    }
    if(penultimate == four) {
        if(current_note.number == three)
            return true;
        else
            return false;
    }
    if(penultimate == seven) {
        if(current_note.number == one)
            return true;
        else
            return false;
    }
    return true;
}
