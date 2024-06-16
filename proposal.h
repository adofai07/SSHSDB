#ifndef PROPOSAL_H_INCLUDED
#define PROPOSAL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typing.h"

#define MAX_TITLE_LENGTH 50
#define MAX_PROPOSALS 100
#define MAX_TEXT_LENGTH 500

struct proposal
{
    int index;
    int sno;
    char title[MAX_TITLE_LENGTH];
};

typedef struct proposal proposal_t;

extern struct proposal proposals[MAX_PROPOSALS];

void load_proposals_from_file();
void save_proposals_to_file();
void delete_oldest_proposal();
void scan_proposal(account_t *);
void print_proposal_list(int);
void print_proposal(int);

#endif
