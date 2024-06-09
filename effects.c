#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "getsheet.h"
#include "getenv.h"
#include "typing.h"
#include "effects.h"

#define SP(X,Y,...) \
({ \
sprintf(X,__VA_ARGS__); \
strncpy(Y,X,strlen(X)); \
})

int w = -1;
int h = -1;

void effect_init() {
    w = get_terminal_width();
    h = get_terminal_height();
}

/// @brief Gets the intro screen
/// @return String of intro
char *intro() {
    // Static variable to avoid stack overflow
    static char buf[1440001];

    FILE *fptr = fopen("image.txt", "r");

    fscanf(fptr, " %s", buf);

    fclose(fptr);

    char *c = (char *) malloc(sizeof(char) * (w + 1) * (h + 1));

    char *_c = c;

    for (int i = 0; i < h - 1; i++) {
        for (int j = 0; j < w; j++) {
            // *_c++ = 0x23;
            int idx = 1600 * (int) ((double) i * 900 / (h - 1)) + (int) ((double) j * 1600 / w);
            // printf("%d %d\n", idx, buf[idx]);
            *_c++ = buf[idx] - 3;
        }
        *_c++ = 0x0A;
    }

    *_c = 0x00;

    return c;
}

/// @brief Prints the menu for a student account
/// @param acc account
/// @return string
char *student_menu(account_t *acc) {
    char *c = (char *) malloc(sizeof(char) * (w + 1) * (h + 1));
    char *_c = c;

    for (int i = 0; i < h - 1; i++) {
        for (int j = 0; j < w; j++) {
            *_c++ = 0x20;
        }
        *_c++ = 0x0A;
    }

    *_c++ = 0x3E;
    *_c++ = 0x3E;
    *_c++ = 0x20;
    *_c = 0x00;

    char tmp[10000];

    // sprintf(tmp, "Name: %s", acc->name);
    // strncpy(c + (w + 1) * 0, tmp, strlen(tmp));
    SP(tmp, c + (w + 1) * 0 , "Name: %s", acc->name);
    SP(tmp, c + (w + 1) * 1 , "ID: %d", acc->student_no);
    SP(tmp, c + (w + 1) * 2 , "Points: %+d / %+d", acc->pos_pts, acc->neg_pts);
    SP(tmp, c + (w + 1) * 3 , "Period 1: %s", get_sheet_by_sno(acc->student_no)->p1);
    SP(tmp, c + (w + 1) * 4 , "Period 2: %s", get_sheet_by_sno(acc->student_no)->p2);
    SP(tmp, c + (w + 1) * 5 , "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    SP(tmp, c + (w + 1) * 9 , "MENUS");
    SP(tmp, c + (w + 1) * 11, "1. Log out");
    SP(tmp, c + (w + 1) * 12, "2. Edit spreadsheet");
    SP(tmp, c + (w + 1) * 13, "3. Make group");
    SP(tmp, c + (w + 1) * 14, "4. Check groups");
    SP(tmp, c + (w + 1) * 15, "5. Get points");
    SP(tmp, c + (w + 1) * 16, "6. Change password");
    SP(tmp, c + (w + 1) * 17, "7. Check spreadsheet");

    return c;
}

/// @brief Prints the menu for a teacher account
/// @param acc account
/// @return string
char *teacher_menu(account_t *acc) {
    char *c = (char *) malloc(sizeof(char) * (w + 1) * (h + 1));
    char *_c = c;

    for (int i = 0; i < h - 1; i++) {
        for (int j = 0; j < w; j++) {
            *_c++ = 0x20;
        }
        *_c++ = 0x0A;
    }

    *_c++ = 0x3E;
    *_c++ = 0x3E;
    *_c++ = 0x20;
    *_c = 0x00;

    char tmp[10000];

    // sprintf(tmp, "Name: %s", acc->name);
    // strncpy(c + (w + 1) * 0, tmp, strlen(tmp));
    SP(tmp, c + (w + 1) * 0 , "Name: %s", acc->name);
    SP(tmp, c + (w + 1) * 1 , "Email: %s", acc->email);
    SP(tmp, c + (w + 1) * 2 , "Points: None");
    SP(tmp, c + (w + 1) * 3 , "Period 1: None");
    SP(tmp, c + (w + 1) * 4 , "Period 2: None");
    SP(tmp, c + (w + 1) * 5 , "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    SP(tmp, c + (w + 1) * 9 , "MENUS");
    SP(tmp, c + (w + 1) * 11, "1. Log out");
    SP(tmp, c + (w + 1) * 12, "2. Print spreadsheet");
    SP(tmp, c + (w + 1) * 13, "3. Student requests");
    SP(tmp, c + (w + 1) * 14, "4. Reductions");
    SP(tmp, c + (w + 1) * 15, "5. Change password");

    return c;
}