#include <stdio.h>
#include <string.h>

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
            printf("--> %4d | %4s | %4s\n", sheet[i].student_no, sheet[i].p1, sheet[i].p2);
        } else {
            printf("    %4d | %4s | %4s\n", sheet[i].student_no, sheet[i].p1, sheet[i].p2);
        }
    }
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
