#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <wchar.h>
#include <locale.h>

#include "makeaccount.h"
#include "u8string.h"

enum FIND_BY {
    EMAIL, NAME, STUDENT_NO
};

struct account {
    char email[50];
    char password[50];
    char name[50];

    // 0 | Student
    // 1 | Admin
    // 2 | Teacher

    int role;

    // Student stats
    int student_no;
    int pos_pts;
    int neg_pts;
};

// Set static to avoid name collision with main.c
static struct account accounts[100000];
static int cnt;

struct account new_account() {
    // Make it static so that all of its members are zero-initialized
    static struct account ret;

    return ret;
}

// Print all accounts
int print_accounts() {
    for (int i = 0; i < cnt; i++) {
        printf("%s %s %d\n", accounts[i].email, accounts[i].name, accounts[i].role);
    }

    return 0;
}

// Read all accounts from file
int get_accounts() {
    FILE *fptr = fopen("accounts.txt", "r");
    char email[50];
    char password[50];
    char name[50];
    int role;
    int student_no;
    int pos_pts;
    int neg_pts;
    int idx = 0;

    // Error while opening file
    if (fptr == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }

    while (~fscanf(fptr, "%s %s %s %d %d %d %d",
                   email, password, name, &role, &student_no, &pos_pts, &neg_pts)) {
        // printf("%s %s %d\n", email, password, role);

        strcpy(accounts[cnt].email, email);
        strcpy(accounts[cnt].password, password);
        u8strcpy(accounts[cnt].name, name);
        accounts[cnt].role = role;
        accounts[cnt].student_no = student_no;
        accounts[cnt].pos_pts = pos_pts;
        accounts[cnt].neg_pts = neg_pts;

        cnt++;
    }

    fclose(fptr);

    return 0;
}

// Write accounts to file
int write_accounts() {
    // Delete the file
    if (remove("accounts.txt")) {
        fprintf(stderr, "Failed to delete file.\n");
        return 1;
    }

    // Recreate the file
    FILE *fptr = fopen("accounts.txt", "w+");

    // Error while opening file
    if (fptr == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }

    for (int i = 0; i < cnt; i++) {
        fprintf(fptr, "%s %s %s %d %d %d %d\n",
                accounts[i].email, accounts[i].password, accounts[i].name, accounts[i].role,
                accounts[i].student_no, accounts[i].pos_pts, accounts[i].neg_pts);
    }

    fclose(fptr);

    return 0;
}

// Email validation
int is_valid_email(char *s, int len) {
    const char *email_regex = "^([a-z0-9])(([-a-z0-9._])*([a-z0-9]))*@([a-z0-9])"
                              "(([a-z0-9-])*([a-z0-9]))+(.([a-z0-9])([-a-z0-9_-])?"
                              "([a-z0-9])+)+$";
    regex_t regex;

    int reti = regcomp(&regex, email_regex, REG_EXTENDED);

    // Regex compilation error
    if (reti) {
        fprintf(stderr, "Regex compilation error.\n");
        return -1;
    }

    reti = regexec(&regex, s, 0, NULL, 0);

    regfree(&regex);

    if (!reti) {
        // Match
        return 1;
    } else if (reti == REG_NOMATCH) {
        // No match
        return 0;
    } else {
        // Error
        fprintf(stderr, "Regex match failed: %s\n", s);
        return -1;
    }
}

// Check if name is valid. Name should contain 3 korean characters and 0-1 english characters.
int is_valid_name(char *name) {
    // if (strlen(name) == 3) {
    //     return '가' <= name[0] && name[0] <= '힣' &&
    //            '가' <= name[1] && name[1] <= '힣' &&
    //            '가' <= name[2] && name[2] <= '힣';
    // } else if (strlen(name) == 4) {
    //     return '가' <= name[0] && name[0] <= '힣' &&
    //            '가' <= name[1] && name[1] <= '힣' &&
    //            '가' <= name[2] && name[2] <= '힣' &&
    //            'A' <= name[3] && name[3] <= 'Z';
    // } else {
    //     return 0;
    // }

    return 1;
}

// Make an account, and return a positive integer if there was an error
int make_account(char *email, char *name, char *password, char *confirm_password, int role, int student_no) {
    // If password and confirm_password are different
    if (strcmp(password, confirm_password) != 0) {
        return 1;
    }

    // If email is invalid
    if (!is_valid_email(email, strlen(email))) {
        return 2;
    }

    for (int i = 0; i < cnt; i++) {
        // If account with same email exists
        if (strcmp(accounts[i].email, email) == 0) {
            return 3;
            break;
        }
    }

    // If name is invalid
    if (!is_valid_name(name)) {
        return 4;
    } 

    if (student_no / 1000 <= 0 || student_no / 1000 >= 4) {
        return 5;
    }

    strcpy(accounts[cnt].email, email);
    u8strcpy(accounts[cnt].name, name);
    strcpy(accounts[cnt].password, password);
    accounts[cnt].role = role;
    accounts[cnt].student_no;

    cnt++;

    write_accounts();

    return 0;
}

// Log in user by email and password, and return the role
int login(char *email, char *password) {
    for (int i = 0; i < cnt; i++) {
        if (strcmp(accounts[i].email, email) == 0 && strcmp(accounts[i].password, password) == 0) {
            return accounts[i].role;
        }
    }

    return -1;
}

void print_all_accounts() {
    for (int i = 0; i < cnt; i++) {
        printf("%s %s %d\n", accounts[i].email, accounts[i].password, accounts[i].role);
    }
}