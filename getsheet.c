#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "makeaccount.h"
#include "getsheet.h"
#include "u8string.h"
#include "typing.h"

#define min(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})

#define max(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

// Stores only name and 1 place
struct name_and_place {
    char name[50];
    char place[50];
};

typedef struct name_and_place name_and_place_t;

static sheet_block_t sheet[1000];
static int cnt = 0;

/// @brief Reads sheet from file
/// @return 0 if no errors, 1 if file read error
int get_sheet() {
    FILE *fptr = fopen("sheet.txt", "r");
    int student_no;
    char name[30];
    int c1;
    char p1[10];
    int c2;
    char p2[10];

    // Error while opening file
    if (fptr == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }

    while (~fscanf(fptr, "%d %s %d %s %d %s",
                   &student_no, name, &c1, p1, &c2, p2)) {

        sheet[cnt].student_no = student_no;
        u8strcpy(sheet[cnt].name, name);
        strcpy(sheet[cnt].p1, p1);
        strcpy(sheet[cnt].p2, p2);

        cnt++;
    }
}

/// @brief Finds index in sheet by student number
/// @param student_no student number
/// @return index of the number, -1 if not found
int sheet_idx(int student_no) {
    for (int i = 0; i < cnt; i++) {
        if (sheet[i].student_no == student_no) {
            return i;
        }
    }

    return -1;
}

void print_sheet() {
    for (int i = 0; i < cnt; i++) {
        printf("%d | %s | %s\n", sheet[i].student_no, sheet[i].p1, sheet[i].p2);
    }
}

void print_sheet_idx(int idx) {
    for (int i = max(0, idx - 2); i < min(cnt, idx + 3); i++) {
        if (i == idx) {
            printf("--> %d | %s | %s\n", sheet[i].student_no, sheet[i].p1, sheet[i].p2);
        } else {
            printf("    %d | %s | %s\n", sheet[i].student_no, sheet[i].p1, sheet[i].p2);
        }
    }
}

/// @brief Finds and returns a sheet block by student number
/// @param sno student number
/// @return sheet block if found, NULL if not found
sheet_block_t *get_sheet_by_sno(int sno) {
    for (int i = 0; i < cnt; i++) {
        if (sheet[i].student_no == sno) {
            return &sheet[i];
        }
    }

    return NULL;
}

/// @brief Sets a student's place by student number
/// @param sno student number
/// @param prd period (1 or 2)
/// @param place place
/// @return 0 no error, 1 if error
int set_sheet_by_sno(int sno, int prd, char *place) {
    for (int i = 0; i < cnt; i++) {
        if (sheet[i].student_no == sno) {
            if (prd == 1) {
                strcpy(sheet[i].p1, place);
            } else if (prd == 2) {
                strcpy(sheet[i].p2, place);
            } else {
                return 1;
            }

            return 0;
        }
    }

    return 1;
}

/// @brief Finds name by student number
/// @param snum student number
/// @return name, NULL if not found
char *find_name_by_sno(int snum) {
    for (int i = 0; i < cnt; i++) {
        if (sheet[i].student_no == snum) {
            return sheet[i].name;
        }
    }

    return (char *) NULL;
}

/// @brief Prints all eligible places
void print_all_places() {
    FILE *fptr = fopen("places.txt", "r");

    char eng[10], kor[100];

    while(~fscanf(fptr, " %s %s", eng, kor)) {
        printf("%s | %s\n", eng, kor);
    }

    fclose(fptr);
}

/// @brief Translates english place to korean
/// @param s place in english
/// @return place in korean
char *eng_to_kor(char *s) {
    FILE *fptr = fopen("placesrev.txt", "r");

    char *eng = (char *) malloc(sizeof(char) * 10);
    char *kor = (char *) malloc(sizeof(char) * 100);

    while(~fscanf(fptr, " %s %s", kor, eng)) {
        if (strcmp(s, eng) == 0) {
            fclose(fptr);
            return kor;
        }
    }

    fclose(fptr);

    return NULL;
}

/// @brief Translates korean place to english
/// @param s place in korean
/// @return place in english
char *kor_to_eng(char *s) {
    FILE *fptr = fopen("placesrev.txt", "r");

    char *eng = (char *) malloc(sizeof(char) * 10);
    char *kor = (char *) malloc(sizeof(char) * 100);

    while(~fscanf(fptr, " %s %s", kor, eng)) {
        if (strcmp(s, kor) == 0) {
            fclose(fptr);
            return eng;
        }
    }

    fclose(fptr);

    return NULL;
}

int static cmp(const void *first, const void *second) {
    return strcmp(
        ((name_and_place_t *) first )->place,
        ((name_and_place_t *) second)->place
    );
}

/// @brief Prints everyone's name and place, grouped by place
/// @param prd period (1 or 2)
void groupby_sheet(int prd) {
    name_and_place_t *all_names = (name_and_place_t *) malloc(sizeof(name_and_place_t) * 1000);

    for (int i = 0; i < cnt; i++) {
        if (prd == 1) {
            strcpy(all_names[i].name, sheet[i].name);
            strcpy(all_names[i].place, eng_to_kor(sheet[i].p1));
        }
        if (prd == 2) {
            strcpy(all_names[i].name, sheet[i].name);
            strcpy(all_names[i].place, eng_to_kor(sheet[i].p2));
        }
    }

    qsort(all_names, cnt, sizeof(name_and_place_t), cmp);
    
    char c[50] = "\0";

    for (int i = 0; i < cnt; i++) {
        if (strcmp(c, all_names[i].place)) {
            strcpy(c, all_names[i].place);
            printf("\n\n%s\n", all_names[i].place);
        }

        printf("%s ", all_names[i].name);
    }

    printf("\n\n");
}