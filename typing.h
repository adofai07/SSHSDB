// A header that defines all the structs and enums

#ifndef TYPING_H
#define TYPING_H

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

typedef struct account account_t;

// One row of sheet
struct sheet_block {
    int student_no;
    char name[50];
    char p1[10];
    char p2[10];
};

typedef struct sheet_block sheet_block_t;

#endif