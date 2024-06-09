#ifndef MAKEACCOUNT_H
#include "makeaccount.h"
#endif // MAKEACCOUNT_H

#include "stdio.h"

#ifndef REQUESTS_H
#define REQUESTS_H

typedef struct request request_t;
int make_request_file(account_t *tch);
int check_regex(const char *expression, char *s, int len);
int request_is_valid(char *request, int option);
int option_from_path(char *path);
int len_request(FILE *fp);
int write_request(char *path, char *request);
int print_requests(char *path);
request_t *load_requests(const char *path);
int accept_request(request_t *requests, int idx);
int update_request(const char *path, request_t *requests);

#endif // REQUESTS
