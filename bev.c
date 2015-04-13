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
    find_input(fp);
    song1.key = read_note(fp);
    song1.major = major(fp);

    // get the bassline notes (letter, quality, and octave)
    find_input(fp);
    for(i=0;; i++) {
        song1.bassline[i].bassnote = read_note(fp);
        if(song1.bassline[i].bassnote.last)
            break;
    }
    song1.num_notes = i + 1;

    // get the soprano note (letter, quality, and octave)
    find_input(fp);
    song1.sopnote = read_note(fp);

    // figures out the note numbers for soprano note and bassline notes
    fill_out_l2n();

    // get the chord progression, store it as strings of roman numerals
    find_input(fp);
    for(i=0;; i++) {
        new_chord = read_chord(fp);
        if(!new_chord.last)
            song1.bassline[i].chord = new_chord;
        else
            break;
    }
    song1.num_chords = i;

    return 0;
}

// print out the information read from edit_this.txt
// this is mostly for debugging purposes
void print_specs(void) {

    int i;

    printf("\n");
    printf("Key: %c", song1.key.letter);
    if(song1.key.quality != ' ')
        printf("%c ", song1.key.quality);
    else
        printf(" ");
    if(song1.major)
        printf("major\n");
    else
        printf("minor\n");

    printf("Bassline: ");
    for(i=0; i<song1.num_notes; i++)
        print_note(song1.bassline[i].bassnote);   
    printf("\n");

    printf("Starting soprano note: ");
    print_note(song1.sopnote);
    printf("\n");

    printf("Chord Progression: ");
    for(i=0; i<song1.num_chords; i++)
        printf("%s ", song1.bassline[i].chord.roman_num);

    printf("\n");

    printf("Converted Bassline: ");
    for(i=0; i<song1.num_notes; i++) {
        printf("%d ", song1.bassline[i].bassnote.number);
    }

    printf("\n\n");

}



// reads the file to figure out if the key is major or minor.
// outputs true if major, false if minor
bool major(FILE *fp) {

    int ch;
    bool major;

    while((ch = fgetc(fp)) == ' ')
        ;
    while(ch != ' ') {
        if(ch == 'm') {
            ch = fgetc(fp);
            if(ch == 'a') {
                ch = fgetc(fp);
                if(ch == 'j')
                    return true;
                else {
                    printf("ERROR: bad format in key specification\n");
                    exit(EXIT_FAILURE);
                }
            }
            else if(ch == 'i') {
                ch = fgetc(fp);
                if(ch == 'n')
                    return false;
                else {
                    printf("ERROR: bad format in key specification\n");
                    exit(EXIT_FAILURE);
                }
            }
            else {
                printf("ERROR: bad format in key specification\n");
                exit(EXIT_FAILURE);
            }
        }
        else {
            printf("ERROR: bad format in key specification\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("don't quite know what happened here..\n");
    exit(EXIT_FAILURE);
}



//gets the next int from the file associated with fp
int getint(FILE *fp) {

    int ch;

    while((ch = fgetc(fp)) < 48 || ch > 57)
        ;
    return ch - 48;

}


//gets the next capital letter (returns as int)
int getletter(FILE *fp) {

    int ch;

    while((ch = fgetc(fp)) < 65 || ch > 90)
        ;
    return ch;
}


// creates a note struct, fills it with info read in from the file, and returns it.
// new_note.last is false unless last note.
struct note read_note(FILE *fp) {

    struct note new_note;
    int ch;

    new_note.last = false;
    new_note.quality = ' ';

    while((ch = fgetc(fp)) == ' ')
        ;
    while(ch != ' ' && ch != '\n') {

        if(ch >= 65 && ch <= 90) {
            if(ch < 72)
                new_note.letter = ch;
            else {
                printf("ERROR: %c is an invalid note letter\n", ch);
                exit(EXIT_FAILURE);
            }
        }

        if(ch == '#' || ch == 'b')
            new_note.quality = ch;

        if(ch >= 48 && ch <= 57) {
            if(ch >=50 && ch <= 53)
                new_note.octave = ch - 48;
            else {
                printf("ERROR: 1 or more notes out of range\n");
                printf("Bass Range: E2 to C4\n");
                printf("Soprano Range: C4 to G5\n");
                exit(EXIT_FAILURE);
            }
        }

        ch = fgetc(fp);
    }

    if(ch == '\n')
        new_note.last = true;

    return new_note;
}


// reads a new chord from the file, and outputs a struct.
struct chord read_chord(FILE *fp) {

    struct chord new_chord;
    char str[3];

    new_chord.last = false;
    new_chord.scale_degree = 0;

    if(fscanf(fp, "%s", str) != EOF) {

        strcpy(new_chord.roman_num, str);

        if(!strcmp(new_chord.roman_num, "I")) {
            new_chord.scale_degree = 1;
            new_chord.major = true;
        }
        if(!strcmp(new_chord.roman_num, "II")) {
            new_chord.scale_degree = 2;
            new_chord.major = true;
        }
        if(!strcmp(new_chord.roman_num, "III")) {
            new_chord.scale_degree = 3;
            new_chord.major = true;
        }
        if(!strcmp(new_chord.roman_num, "IV")) {
            new_chord.scale_degree = 4;
            new_chord.major = true;
        }
        if(!strcmp(new_chord.roman_num, "V")) {
            new_chord.scale_degree = 5;
            new_chord.major = true;
        }
        if(!strcmp(new_chord.roman_num, "VI")) {
            new_chord.scale_degree = 6;
            new_chord.major = true;
        }
        if(!strcmp(new_chord.roman_num, "VII")) {
            new_chord.scale_degree = 7;
            new_chord.major = true;
        }
        if(!strcmp(new_chord.roman_num, "i")) {
            new_chord.scale_degree = 1;
            new_chord.major = false;
        }
        if(!strcmp(new_chord.roman_num, "ii")) {
            new_chord.scale_degree = 2;
            new_chord.major = false;
        }
        if(!strcmp(new_chord.roman_num, "iii")) {
            new_chord.scale_degree = 3;
            new_chord.major = false;
        }
        if(!strcmp(new_chord.roman_num, "iv")) {
            new_chord.scale_degree = 4;
            new_chord.major = false;
        }
        if(!strcmp(new_chord.roman_num, "v")) {
            new_chord.scale_degree = 5;
            new_chord.major = false;
        }
        if(!strcmp(new_chord.roman_num, "vi")) {
            new_chord.scale_degree = 6;
            new_chord.major = false;
        }
        if(!strcmp(new_chord.roman_num, "vii")) {
            new_chord.scale_degree = 7;
            new_chord.major = false;
        }

        if(new_chord.scale_degree == 0) {
            printf("ERROR: incorrect format for roman numerals\n");
            exit(EXIT_FAILURE);
        }

    }
    else
        new_chord.last = true;

    //TODO eventually add capabilities for 7th chords, diminished chords

    return new_chord;
}


//checks to make sure the basslinei (and the first soprano note) fits with the chord progression. True if it's all good
bool check_bassline() {

    //first, make sure there are the same number of bassline notes as chords
    if(song1.num_notes != song1.num_chords) {
        printf("ERROR: Enter one chord for each bass note\n");
        exit(EXIT_FAILURE);
    }

    int i;
    struct beat beat;

    //make sure the bassline notes fit with the chords
    for(i=0; i<song1.num_notes; i++) {
        beat = song1.bassline[i];
        if(beat.bassnote.letter != beat.chord.one.letter &&
                beat.bassnote.letter != beat.chord.three.letter &&
                beat.bassnote.letter != beat.chord.five.letter &&
                beat.bassnote.letter != beat.chord.seven.letter) {
            printf("ERROR: make sure that your bassline fits with the chords!\n");
            exit(EXIT_FAILURE);
        }
    }

    //make sure the first soprano note fits in the chord
    if(sopnote.letter != song1.bassline[0].chord.one.letter &&
            sopnote.letter != song1.bassline[0].chord.three.letter &&
            sopnote.letter != song1.bassline[0].chord.five.letter &&
            sopnote.letter != song1.bassline[0].chord.seven.letter) {
        printf("ERROR: make sure that the first soprano note fits with the chords!\n");
        exit(EXIT_FAILURE);
    }

    //make sure the third isn't doubled on the first note
    if(sopnote.letter == song1.bassline[0].chord.three.letter) {
        printf("ERROR: Please don't double the third. That's egregious\n");
        exit(EXIT_FAILURE);
    }
    return true;
}


void fill_out_l2n(void) {
    int i;

    for(i=0; i<song1.num_notes; i++)
        song1.bassline[i].bassnote = letter_to_number(song1.bassline[i].bassnote);
        
    sopnote = letter_to_number(sopnote);
}

void fill_out_n2l(void) {

    int i;

    for(i=0; i<song1.num_notes; i++)
        song1.bassline[i].bassnote = number_to_letter(song1.bassline[i].bassnote);
        
    sopnote = number_to_letter(sopnote);

}
// previously, soprano notes and bassline notes were stored in letter-quality-octave form.
// for instance, C#5 (C4 is middle C, and C5 is the C an octave higher, etc).
// This function converts the note into number form, where the lowest note in the bass's
// range (E2) is designated '0', and each half step above that gets a new number. The
// soprano's range maxes out at a G5, which is note 40.
struct note letter_to_number(struct note current_note) {


        if(current_note.letter == 'E')
            current_note.number = (current_note.octave - 2)*12;
        if(current_note.letter == 'F')
            current_note.number = (current_note.octave - 2)*12 + 1;
        if(current_note.letter == 'G')
            current_note.number = (current_note.octave - 2)*12 + 3;
        if(current_note.letter == 'A')
            current_note.number = (current_note.octave - 2)*12 + 5;
        if(current_note.letter == 'B')
            current_note.number = (current_note.octave - 2)*12 + 7;
        if(current_note.letter == 'C')
            current_note.number = (current_note.octave - 2)*12 + 8;
        if(current_note.letter == 'D')
            current_note.number = (current_note.octave - 2)*12 + 10;

        if(current_note.quality == '#')
            current_note.number++;
        if(current_note.quality == 'b')
            current_note.number--;
    
        return current_note;    
}


//given a note struct, fills in the information about that note, including note letter, quality, and octave.
struct note number_to_letter(struct note note1) {

    int quotient, remainder;
    quotient = note1.number / 12;
    remainder = note1.number % 12;
    bool flats = false;
    bool sharps = false;
    bool black_key = false;
    note1.quality = ' ';

    if(song1.key.quality == ' ') {
        if(song1.key.letter == 'F')
            flats = true;
        else
            sharps = true;
    }
    else {
        if(song1.key.quality == '#')
            sharps = true;
        if(song1.key.quality == 'b')
            flats = true;
    }


    if(quotient == 0) {
        if(remainder < 8)
            note1.octave = 2;
        else
            note1.octave = 3;
    }
    if(quotient == 1) {
        if(remainder < 8)
            note1.octave = 3;
        else
            note1.octave = 4;
    }
    if(quotient == 2) {
        if(remainder < 8)
            note1.octave = 4;
        else
            note1.octave = 5;
    }
    if(quotient == 3)
        note1.octave = 5;


    switch(remainder) {
    case 0: note1.letter = 'E';
            break; 
    case 1: note1.letter = 'F';
            break;
    case 2: if(flats)
                note1.letter = 'G';
            else
               note1.letter = 'F';
            black_key = true;
            break;
    case 3: note1.letter = 'G';
            break;
    case 4: if(flats)
                note1.letter = 'A';
            else
                note1.letter = 'G';
            black_key = true;
            break;
    case 5: note1.letter = 'A';
            break;
    case 6: if(flats)
                note1.letter = 'B';
            else
                note1.letter = 'A';
            black_key = true;
            break;
    case 7: note1.letter = 'B';
            break;
    case 8: note1.letter = 'C';
            break;
    case 9: if(flats)
                note1.letter = 'D';
            else
                note1.letter = 'C';
            black_key = true;
            break;
    case 10: note1.letter = 'D';
             break;
    case 11: if(flats)
                 note1.letter = 'E';
             else
                 note1.letter = 'D';
             black_key = true;
             break;
    }
    
    if(black_key) {
        if(sharps)
            note1.quality = '#';
        if(flats)
            note1.quality = 'b';
    }    

    return note1;
}

void find_input(FILE *fp) {

    int ch;

    while((ch = fgetc(fp)) != '>')
        ;
    while((ch = fgetc(fp)) != ':')
        ;
}

void determine_chords(void) {

    int i;

    for(i=0; i<song1.num_notes; i++) {
//fill out 
    }

}

void test_conversions(void) {

    struct note note1;

    note1.letter = 'A';
    note1.quality = '#';
    note1.octave = 4;

    printf("note name: A#4\n");
    note1 = letter_to_number(note1);
    printf("note number: %d\n\n", note1.number);

    struct note note2;

    note2.number = 14;

    printf("note number: 14\n");
    note2 = number_to_letter(note2);

    printf("note name: ");
    print_note(note2);
    printf("\n");
}


void print_note(struct note note1) {
    printf("%c", note1.letter);
    if(note1.quality != ' ')
        printf("%c", note1.quality);
    printf("%d ", note1.octave);
}
