#include "makeaccount.h"
#include <stdio.h>
#include <string.h>
#include <regex.h>

struct account {
    char email[50];
    char password[50];

    // 0 | Student
    // 1 | Admin
    // 2 | Teacher

    int role;

    // Student stats
    int pos_pts;
    int neg_pts;
    char circle[50];
};

static struct account accounts[100000];
static int cnt;

// Read all accounts from file
int get_accounts() {
    FILE *fptr = fopen("accounts.txt", "r");
    char email[50];
    char password[50];
    int role;
    int idx = 0;

    // Error while opening file
    if (fptr == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }

    while (~fscanf(fptr, "%s %s %d", email, password, &role)) {
        // printf("%s %s %d\n", email, password, role);

        strcpy(accounts[cnt].email, email);
        strcpy(accounts[cnt].password, password);
        accounts[cnt].role = role;

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
        fprintf(fptr, "%s %s %d\n", accounts[i].email, accounts[i].password, accounts[i].role);
    }

    fclose(fptr);

    return 0;
}

// Email validation
int is_valid_email(char* s, int len) {
    const char *email_regex =   "^([a-z0-9])(([-a-z0-9._])*([a-z0-9]))*@([a-z0-9])"
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

// Make an account, and return a positive integer if there was an error
int make_account(char* email, char* password, char* confirm_password, int role) {
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

    strcpy(accounts[cnt].email, email);
    strcpy(accounts[cnt].password, password);
    accounts[cnt].role = role;

    cnt++;

    write_accounts();

    return 0;
}

// Log in user by email and password, and return the role
int login(char* email, char* password) {
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