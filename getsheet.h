#ifndef TYPING_H
#include "typing.h"
#endif

#ifndef GETSHEET_H
#define GETSHEET_H

int get_sheet();
int sheet_idx(int);
void print_sheet();
void print_sheet_idx(int);
char *find_name_by_sno(int);
sheet_block_t *get_sheet_by_sno(int);
int set_sheet_by_sno(int, int, char *);
void print_all_places();
void groupby_sheet(int);

#endif