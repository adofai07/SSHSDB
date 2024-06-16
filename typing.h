// A header that defines all the structs and enums

#ifndef TYPING_H
#define TYPING_H

// Struct for accounts
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

// Struct for groups
struct group {
    char name[50];
    int size;
    int members[100];
};

typedef struct group group_t;

// Struct for one row of sheet
struct sheet_block {
    int student_no;
    char name[50];

    // Period 1 place
    char p1[10];

    // Period 2 place
    char p2[10];
};

typedef struct sheet_block sheet_block_t;

enum ERRORCODES {
    ETCERROR = -1,
    MEMERROR = -2,
    FILEERROR = -3,
    INVLDINDEX = -4,
    PROPOSALFULL = -5,
    NOPROPOSALS = -6,
    INVLDCLSRM = -7,
    FULLCLSRM = -8,
    INVLDGROUP = -9,
    INVLDACNT = -10
};

#endif