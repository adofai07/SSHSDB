#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char notes[100][100];
static int cnt = 0;

int get_notes() {
    FILE *fptr = fopen("notes.txt", "r");

    if (fptr == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }

    while (fgets(notes[cnt], 100, fptr) != NULL) {
        cnt++;
    }

    return 0;
}

int write_notes() {
    FILE *fptr = fopen("notes.txt", "w");

    if (fptr == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }

    for (int i = 0; i < cnt; i++) {
        fprintf(fptr, "%s", notes[i]);
    }

    return 0;
}

void print_notes() {
    for (int i = 0; i < cnt; i++) {
        printf("[%2d] %s", i + 1, notes[i]);
    }

    printf("\n");
}

int pop_note() {
    cnt--;

    for (int i = 0; i < cnt; i++) {
        memmove(notes[i], notes[i + 1], sizeof(char) * 100);
    }

    return cnt;
}

int add_note(char *s) {
    strcpy(notes[cnt], s);

    cnt++;

    return cnt;
}

int get_note_cnt() {
    return cnt;
}

char *get_ith_note(int i) {
    if (0 <= i && i < cnt) {
        return notes[i];
    }

    return NULL;
}
