#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <wchar.h>
#include <locale.h>

#include "makeaccount.h"
#include "u8string.h"
#include "typing.h"

#define MAX_ACCOUNT_CNT 1000

// Set static to avoid name collisions with main.c
static account_t accounts[MAX_ACCOUNT_CNT];
static int cnt;


/// @brief Makes a new account
/// @return new account
account_t new_account() {
    // Make it static so that all of its members are zero-initialized
    static account_t ret;

    return ret;
}

/// @brief Prints all accounts
void print_accounts() {
    for (int i = 0; i < cnt; i++) {
        printf("%s %s %d\n", accounts[i].email, accounts[i].name, accounts[i].role);
    }
}

/// @brief Reads all accounts from file
/// @return 0 if no errors, 1 if file read error
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

/// @brief Writes accounts to file
/// @return 0 if no errors, 1 if file write error
int write_accounts() {
    // Delete the file
    // if (remove("accounts.txt")) {
    //     fprintf(stderr, "Failed to delete file.\n");
    //     return 1;
    // }

    // Recreate the file
    FILE *fptr = fopen("accounts.txt", "w");

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

/// @brief Checks if email is valid
/// @param s email
/// @param len length of email
/// @return 1 if email is valid, 0 if email is invalid, -1 if error
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

/// @brief Checks if a given integer is korean
/// @param s integer
/// @return 1 if s is korean, 0 if it isn't
int is_korean(int s) {
    return 15380608 <= s && s <= 15572643;
}

/// @brief Checks if name is valid
/// @param name name
/// @return 1 if name is valid, 0 if it isn't
int is_valid_name(char *name) {
    if (u8strlen(name) == 3) {
        if (num_bytes(&name[0]) != 3) return 0;
        if (num_bytes(&name[3]) != 3) return 0;
        if (num_bytes(&name[6]) != 3) return 0;

        int bitcat[3] = {0, 0, 0};

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 24; j++) {
                bitcat[i / 3] |= ((1 << (j % 8)) & name[i]) << ((2 - i % 3) * 8);
            }
        }

        return is_korean(bitcat[0]) &&
               is_korean(bitcat[1]) &&
               is_korean(bitcat[2]);

    } else if (u8strlen(name) == 4) {
        if (num_bytes(&name[0]) != 3) return 0;
        if (num_bytes(&name[3]) != 3) return 0;
        if (num_bytes(&name[6]) != 3) return 0;

        int bitcat[3] = {0, 0, 0};

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 24; j++) {
                bitcat[i / 3] |= ((1 << (j % 8)) & name[i]) << ((2 - i % 3) * 8);
            }
        }

        return is_korean(bitcat[0]) &&
               is_korean(bitcat[1]) &&
               is_korean(bitcat[2]) &&
               'A' <= name[9] && name[9] <= 'Z';

    } else {
        return 0;
    }
}

/// @brief Makes an account
/// @param email email
/// @param name name (can be a utf-8 string)
/// @param password password
/// @param confirm_password password (again)
/// @param role role
/// @param student_no student number
/// @return 0 if account is successfully created, positive integer elsewise
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

    // If student number is invalid
    if (role == 0 && (student_no / 1000 <= 0 || student_no / 1000 >= 4)) {
        return 5;
    }

    // If there are too many accounts
    if (cnt == MAX_ACCOUNT_CNT) {
        return 6;
    }

    strcpy(accounts[cnt].email, email);
    strcpy(accounts[cnt].password, password);
    u8strcpy(accounts[cnt].name, name);
    accounts[cnt].role = role;
    accounts[cnt].student_no = student_no;
    accounts[cnt].pos_pts = 0;
    accounts[cnt].neg_pts = 0;

    cnt++;

    return write_accounts();
}

/// @brief Logs in user by email and password
/// @param email email
/// @param password password
/// @return role of the account, -1 if failed to login
int login(char *email, char *password) {
    for (int i = 0; i < cnt; i++) {
        if (strcmp(accounts[i].email, email) == 0 && strcmp(accounts[i].password, password) == 0) {
            return accounts[i].role;
        }
    }

    return -1;
}

/// @brief Prints all accounts
void print_all_accounts() {
    for (int i = 0; i < cnt; i++) {
        printf("%s %s %d\n", accounts[i].email, accounts[i].password, accounts[i].role);
    }
}

/// @brief Finds index by email
/// @param email email
/// @return index of the student account, -1 if not found
int student_idx(char *email) {
    for (int i = 0; i < cnt; i++) {
        if (strcmp(accounts[i].email, email) == 0) {
            return i;
        }
    }

    return -1;
}

/// @brief Finds index by email
/// @param email email
/// @return pointer to the student account, -1 if not found
account_t *account_ptr(char *email) {
    for (int i = 0; i < cnt; i++) {
        if (strcmp(accounts[i].email, email) == 0) {
            return &accounts[i];
        }
    }

    return NULL;
}

/// @brief Finds account by student number
/// @param sno student number
/// @return pointer to account_t object if found, NULL if not
account_t *find_account_by_sno(int sno) {
    for (int i = 0; i < cnt; i++) {
        if (accounts[i].student_no == sno) {
            return &accounts[i];
        }
    }

    return (account_t *) NULL;
}

/// @brief Finds account by name
/// @param name name
/// @return pointer to account_t object if found, NULL if not
account_t *find_account_by_name(char *name) {
    for (int i = 0; i < cnt; i++) {
        if (strcmp(accounts[i].name, name) == 0) {
            return &accounts[i];
        }
    }

    return (account_t *) NULL;
}

/// @brief Finds account by index
/// @param idx index
/// @return pointer to account_t object if found, NULL if not
account_t *find_account_by_idx(int idx) {
    if (0 <= idx && idx < cnt) {
        return &accounts[idx];
    }

    return (account_t *) NULL;
}

/// @brief Returns the number of accounts
/// @return number of accounts
int account_cnt() {
    return cnt;
}