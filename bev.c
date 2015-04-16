//bev.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "file_access.h"
#include "ideas.h"

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
    print_specs();
    check_bassline();
}

