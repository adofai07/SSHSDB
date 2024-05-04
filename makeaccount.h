#ifndef MAKEACCOUNT_H
#define MAKEACCOUNT_H

struct account;
struct account new_account();
void print_accounts();
int get_accounts();
int is_valid_email(char *, int);
int make_account(char *, char *, char *, char *, int, int);
void print_all_accounts();
int login(char *, char *);

#endif