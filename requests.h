#ifndef MAKEACCOUNT_H
#include "makeaccount.h"
#endif // MAKEACCOUNT_H

#ifndef REQUESTS_H
#define REQUESTS_H

int make_request_file(account_t *tch);
int check_regex(const char *expression, char *s, int len);
int request_is_valid(char *request, int option);
int option_from_path(char *path);
int len_request(FILE *fp);
int write_request(char *path, char *request);
int print_requests(char *path);
char **load_requests(char *path);

#endif // REQUESTS
