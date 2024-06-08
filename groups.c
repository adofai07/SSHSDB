#include <stdio.h>
#include <string.h>

#include "typing.h"
#include "makeaccount.h"
#include "groups.h"
#include "u8string.h"
#include "getsheet.h"

// Set static to avoid name collisions with main.c
static group_t groups[MAX_GROUP_CNT];
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

            groups[cnt].members[i] = sno;
        }

        cnt++;
    }

    fclose(fptr);

    return 0;
}

/// @brief Writes groups to file
/// @return 0 if no errors, 1 if file write error
int write_groups() {
    // Delete the file
    if (remove("groups.txt")) {
        fprintf(stderr, "Failed to delete file.\n");
        return 1;
    }

    // Recreate the file
    FILE *fptr = fopen("groups.txt", "w+");

    // Error while opening file
    if (fptr == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }

    for (int i = 0; i < cnt; i++) {
        // printf("Line %d\n", i);
        fprintf(fptr, "%s %d", groups[i].name, groups[i].size);

        for (int j = 0; j < groups[i].size; j++) {
            fprintf(fptr, " %d", groups[i].members[j]);
        }

        fprintf(fptr, "\n");
    }

    fclose(fptr);

    return 0;
}

/// @brief Prints all groups
void print_groups() {
    for (int i = 0; i < cnt; i++) {
        printf("%s |", groups[i].name);

        for (int j = 0; j < groups[i].size; j++) {
            // Struct group stores numbers BUT prints names
            printf(" %s", find_name_by_sno(groups[i].members[j]));
        }

        printf("\n");
    }
}

/// @brief Makes a group
/// @param group_name name of group
/// @param sno_array student number array
/// @param size student number array size
/// @return 0 if successfully made group, positive integer elsewise
int make_group(char *group_name, const int *sno_array, int size) {
    // printf("%d %d %d\n", group_name, sno_array, size);
    strcpy(groups[cnt].name, group_name);
    groups[cnt].size = size;
    memcpy(groups[cnt].members, sno_array, sizeof(int) * size);

    // for (int i = 0; i < size; i++) {
    //     groups[cnt].members[i] = sno_array[i];
    // }
    
    cnt++;

    return write_groups();
}

/// @brief finds group by name
/// @param name name of group
/// @return pointer to group if found, NULL if not found
group_t *find_group_by_name(char *name) {
    for (int i = 0; i < cnt; i++) {
        if (strcmp(groups[i].name, name) == 0) {
            return &groups[i];
        }
    }

    return NULL;
}

/// @brief Returns the maximum group size
/// @return maximum group size
int get_max_group_size() {
    return MAX_GROUP_SIZE;
}