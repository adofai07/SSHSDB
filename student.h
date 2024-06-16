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

int print_available(account_t *acc, int p);
int print_place(int p, char *classroom);
int move_individual(int sno, int p, char *classroom, account_t *tch);
int move_group(group_t *grp, int p, char *classroom, account_t *tch);
int outing_individual(account_t *acc, int leave, int rtn, account_t *tch);
int outing_group(group_t *grp, int leave, int rtn, account_t *tch);
int home(account_t *acc, account_t *tch);

#endif