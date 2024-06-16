#ifndef TYPING_H
#include "typing.h"
#endif

#ifndef STUDENT_H
#define STUDENT_H

// int timeset(struct tm **t);
// int in_list_classroom(const char *list[], const char *classroom);
// int index_classroom(const char *list[], const char *classroom);
// int count_place(int p, const char *classroom);
// int is_full_place(int p, const char *classroom);
// int find_sno(int sno, FILE *file);
// int save_sno(int sno, sheet_block_t *student);
// int update_sheet();

int print_available(account_t *, int);
int print_place(int, char *);
int move_individual(int, int, char *, account_t *);
int move_group(group_t *, int, char *, account_t *);
int outing_individual(account_t *, int, int, account_t *);
int outing_group(group_t *, int, int, account_t *);
int home(account_t *, account_t *);

#endif