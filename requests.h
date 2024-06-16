#ifndef MAKEACCOUNT_H
#include "makeaccount.h"
#endif // MAKEACCOUNT_H

#include "stdio.h"

#ifndef REQUESTS_H
#define REQUESTS_H

typedef struct request request_t;
int make_request_file(char *);
int check_regex(const char *, char *, int);
int request_is_valid(char *, int);
int option_from_path(char *);
int len_request(FILE *);
int write_request(char *, char *);
int print_requests(char *);
request_t *load_requests(const char *);
int accept_request(request_t *, int);
int update_request(const char *, request_t *);

#endif
