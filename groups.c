#include <stdio.h>
#include <string.h>

#include "typing.h"
#include "makeaccount.h"
#include "groups.h"
#include "u8string.h"
#include "getsheet.h"

const int MAX_GROUP_CNT = 100;
const int MAX_GROUP_SIZE = 100;

// Set static to avoid name collisions with main.c
static group_t groups[100];
static int cnt = 0;

/// @brief Read groups from file
/// @return 0 if no error, 1 if file read error
int get_groups() {
    FILE *fptr = fopen("groups.txt", "r");
    char group_name[50];
    int group_size;
    int sno;

    while (~fscanf(fptr, "%s %d", group_name, &group_size)) {
        strcpy(groups[cnt].name, group_name);
        groups[cnt].size = group_size;

        for (int i = 0; i < group_size; i++) {
            fscanf(fptr, "%d", &sno);

            u8strcpy(groups[cnt].members[i], find_name_by_sno(sno));
        }

        cnt++;
    }
}

/// @brief Prints all groups
void print_groups() {
    for (int i = 0; i < cnt; i++) {
        printf("%s |", groups[i].name);

        for (int j = 0; j < groups[i].size; j++) {
            printf(" %s", groups[i].members[j]);
        }

        printf("\n");
    }
}

/// @brief Makes a group
/// @param group_name name of group
/// @param sno_array student number array
/// @param size student number array size
/// @return 0 if successfully made group, positive integer elsewise
int make_group(char *group_name, int *sno_array, int size) {
    strcpy(groups[cnt].name, group_name);
    groups[cnt].size = size;
    memcpy(groups[cnt].members, sno_array, sizeof(int) * size);
    
    cnt++;
}