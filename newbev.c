#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// TODO remember to fix this bug at some point: if there's a space after the last note written, the last note will be
//repeated.

//FILE ACCESS FUNCTIONS

int read_file(FILE *fp);
int getint(FILE *fp);
bool major(FILE *fp);
int getletter(FILE *fp);
struct note read_note(FILE *fp);
void print_specs(void);
struct chord read_chord(FILE *fp);
void find_input(FILE *fp);

//other functions
struct note letter_to_number(struct note);
struct note number_to_letter(struct note);
void determine_chords(void);
bool check_bassline(void);
void fill_out_l2n(void);
void fill_out_n2l(void);
void test_conversions(void);
void print_note(struct note note1);

struct note {
    int number; //1-40
    int letter; //A - G
    int quality; // #, b or 0
    int octave; //2-5
    bool last; //false, unless it's the last note
} sopnote;

struct chord {
    bool major;
    int scale_degree; //1-7
    char roman_num[3]; //I through VII, or i through vii in minor
    bool last;
    struct note one;
    struct note three;
    struct note five;
    struct note seven;
};

struct beat {
    struct note bassnote;
    struct chord chord;
};

struct song {
    bool major;
    struct note key;
    struct beat bassline[20];
    struct note sopnote;
    int num_notes;
    int num_chords;
} song1;

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
    print_specs();
    test_conversions();
    //check_bassline(); don't call this until you've written determine chords
}

int read_file(FILE *fp) {

    int ch, i;
    struct chord new_chord;

    // get the key
    return 0;
}
