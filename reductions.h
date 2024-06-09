#ifndef REDUCTIONS_H
#define REDUCTIONS_H

#define MAX_REDUCTION_CNT 100

int get_reductions();
void print_reductions();
int reductions_cnt();
int request_reduction(account_t *, int);
int register_reduction(account_t *, struct tm *, int, const char *, int);
int print_tchreduction(account_t *);
int accept_reduction(account_t *, int);
int update_reduction(account_t *);

#endif