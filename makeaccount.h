#ifndef TYPING_H
#include "typing.h"
#endif

#ifndef MAKEACCOUNT_H
#define MAKEACCOUNT_H

struct account;
struct account new_account();
int write_accounts();
void print_accounts();
int get_accounts();
int is_valid_email(char *, int);
int is_valid_name(char *);
int make_account(char *, char *, char *, char *, int, int);
void print_all_accounts();
int login(char *, char *);
int student_idx(char *);
account_t *account_ptr(char *);
account_t *find_account_by_sno(int);
account_t *find_account_by_name(char *);
account_t *find_account_by_idx(int);
int account_cnt();

#endif