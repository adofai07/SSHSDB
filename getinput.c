#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "getinput.h"

/// @brief Checks is string is a positive integer
/// @param s string
/// @param len length of string
/// @return 0 if string is not an integer, 1 if it is
int is_number(char *s, int len) {
    int chk = 1;

    for (int i = 0; i < len; i++) {
        if (!('0' <= s[i] && s[i] <= '9')) {
            chk = 0;
            break;
        }
    }

    return chk;
}

/// @brief Converts string to integer
/// @param s string
/// @param len length of string
/// @return integer value of string
int str_to_int(char *s, int len) {
    assert(is_number(s, len));

    int ret = 0;
    int p = 1;

    for (int i = len - 1; i >= 0; i--) {
        ret += (s[i] - '0') * p;
        p *= 10;
    }

    return ret;
}

/// @brief Gets an input from the user until he inputs a number x such that st <= x <= ed. Then return the input.
/// @param st minimum value
/// @param ed maximum value
/// @return integer from stdin
int get_input(int st, int ed) {
    char s[50];

    scanf("%s", s);

    while (!(is_number(s, strlen(s)) && st <= str_to_int(s, strlen(s)) && str_to_int(s, strlen(s)) <= ed)) {
        printf("Please enter a number such that %d <= x <= %d. ", st, ed);
        scanf("%s", s);
    }

    return str_to_int(s, strlen(s));
}