#include <stdio.h>
#include <string.h>
#include "getinput.h"

// Check if string is a positive number.
int is_number(char* s, int len) {
    int chk = 1;

    for (int i = 0; i < len; i++) {
        if (!('0' <= s[i] && s[i] <= '9')) {
            chk = 0;
            break;
        }
    }

    return chk;
}

// Convert string to integer.
int str_to_int(char* s, int len) {
    int ret = 0;
    int p = 1;

    for (int i = len - 1; i >= 0; i--) {
        ret += (s[i] - '0') * p;
        p *= 10;
    }

    return ret;
}

// Get an input from the user until he inputs a number x such that st <= x <= ed. Then return the input.
int get_input(int st, int ed) {
    char s[50];

    scanf("%s", s);

    while (!(is_number(s, strlen(s)) && st <= str_to_int(s, strlen(s)) && str_to_int(s, strlen(s)) <= ed)) {
        printf("Please enter a number such that %d <= x <= %d. ", st, ed);
        scanf("%s", s);
    }

    return str_to_int(s, strlen(s));
}