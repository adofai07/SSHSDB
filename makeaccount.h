#ifndef MAKEACCOUNT_H
#define MAKEACCOUNT_H

struct account;
int get_accounts();
int is_valid_email(char* s, int len);
int make_account(char* email, char* password, char* confirm_password, int role);
void print_all_accounts();
int login(char* email, char* password);

#endif