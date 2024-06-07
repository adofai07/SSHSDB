#include <stdio.h>
#include <time.h>
#include <string.h>

#include "u8string.h"
#include "makeaccount.h"
#include "typing.h"


// Struct for reduction
struct reduction {
    int id;
    struct tm date;
    int time;
    char loc[20];
    int num;
    char tch[50];
};

typedef struct reduction reductions_t;

// Set static to avoid name collisions with main.c
static reductions_t reductions[MAX_REDUCTION_CNT];
static int cnt_r = 0;

/// @brief Read reductions from file
/// @return 0 if no error, 1 if file read error
int get_reductions() {
    FILE *fptr = fopen("reduction.txt", "r");
    int reduc_id;
    struct tm reduc_date;
    int reduc_time;
    char reduc_loc[20];
    int reduc_num;
    char reduc_tch[50];

    int c = 0;
    while (~fscanf(fptr, "%d %d.%d.%d %d %s %d %s", 
            &reduc_id, &reduc_date.tm_year, &reduc_date.tm_mon, &reduc_date.tm_mday, 
            &reduc_time, reduc_loc, &reduc_num, reduc_tch)) {
        reductions[c].id = reduc_id;
        reductions[c].date = reduc_date;
        reductions[c].time = reduc_time;
        strcpy(reductions[c].loc, reduc_loc);
        reductions[c].num = reduc_num;
        strcpy(reductions[c].tch, reduc_tch);
        c++;
    }

    fclose(fptr);

    return 0;
}

/// @brief Prints all reductions
void print_reductions() {
    printf("reductions\n");
    printf("id | date & time | location | staff num | teacher\n");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < cnt_r; i++) {
        printf("%3d | %02d.%02d.%02d  %2d | %s | %2d | %s\n", 
        reductions[i].id, reductions[i].date.tm_year - 100, reductions[i].date.tm_mon + 1, reductions[i].date.tm_mday, reductions[i].time, 
        reductions[i].loc, reductions[i].num, reductions[i].tch);
    }
}

/// @brief Application for a reduction in penalty points
/// @param acc pointer to account struct object (student)
/// @param red_id specified reduction id to request application
/// @return 0 if no errors, 1 if reduc_id not found, 2 if time is inappropriate, 
///         3 if num is inappropriate, 4 if error while writing to request file
int request_reduction(account_t *acc, int red_id) {
    // Error that reduc_id not found
    if (red_id >= cnt_r) return 1;

    // Error that time is inappropriate
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    if (mktime(&reductions[red_id].date) < mktime(tm_now)) return 2;

    // Error that num is inappropriate
    if (reductions[red_id].num == 0) return 3;

    // Error while writing to request file
    char rqr_filename[70];
    snprintf(rqr_filename, sizeof(rqr_filename), "%s_reduction.txt", reductions[red_id].tch);
    FILE *fp_rqr = fopen(rqr_filename, "a+");
    if (!fp_rqr) return 4;

    fprintf(fp_rqr, "%02d.%02d.%02d %d %02d %s\n",
            reductions[red_id].date.tm_year, reductions[red_id].date.tm_mon, reductions[red_id].date.tm_mday,
            acc->student_no, reductions[red_id].time, reductions[red_id].loc);

    fclose(fp_rqr);
    
    return 0;
}

/// @brief register reduction
/// @param tch pointer to account struct object (teacher)
/// @param date0 pointer to struct tm object for reduction date
/// @param time0 value to specify reduction time (changes necessary)
/// @param loc0 value to specify reduction location (enum?)
/// @param num0 maximum number of applicable students
/// @return 0 if no errors, 1 if time inappropriate, 2 if loc not available,
///         3 if not teacher, 4 if error while writing to request file
int register_reduction(account_t *tch, struct tm *date0, int time0, const char *loc0, int num0) {
    // Error that time is inappropriate
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    if (mktime(date0) < mktime(tm_now)) return 1;

    // Error that location is not available
    int found = -1;
    const char* reduc_loc[22] = {"sem1", "sem2", "sem3", "sem4", "libr",
                                 "emp1", "emp2", "emp3", "eng1", "eng2", "eng3", "phy1", "phy2",
                                 "phy3", "ear1", "ear2", "med1", "med2", "scix", "geox", "meet", "hist"};
    for (int i = 0; i < 22; i++) {
        if (strcmp(loc0, reduc_loc[i]) == 0) {
            found = i;
            break;
        }
    }
    if (found == -1) return 2;

    // Error that the request is not made to teacher
    if (tch->role != 2) return 3;

    // Error while writing to request file
    FILE *fp_rgr = fopen("reduction.txt", "a+");
    if (!fp_rgr) return 4;

    fprintf(fp_rgr, "%d %02d.%02d.%02d %d %s %d %s\n", 
    cnt_r, date0->tm_year, date0->tm_mon, date0->tm_mday, time0, loc0, num0, tch->name);

    reductions[cnt_r].id = cnt_r;
    reductions[cnt_r].date = *date0;
    reductions[cnt_r].time = time0;
    strcpy(reductions[cnt_r].loc, loc0);
    reductions[cnt_r].num = num0;
    strcpy(reductions[cnt_r].tch, tch->name);
    cnt_r++;

    fclose(fp_rgr);

    return 0;
}


// 디버깅 코드
// int main() {
//     account_t teacher = { .email = "teacher@example.com", .password = "password", .name = "John", .role = 2 };
//     struct tm reduction_date = { .tm_year = 124, .tm_mon = 7, .tm_mday = 5 }; // Date: 2024-08-05
//     int reduction_time = 14; // 2 PM
//     const char *reduction_loc = "eng1";
//     int reduction_num = 10;

//     int res = register_reduction(&teacher, &reduction_date, reduction_time, reduction_loc, reduction_num);
//     if (res == 0) {
//         printf("Reduction registered successfully.\n");
//     } else {
//         printf("Failed to register reduction. Error code: %d\n", res);
//     }
//     get_reductions();

//     printf("%d cnt_r\n", cnt_r);

//     printf("%d %02d.%02d.%02d\n", reductions[0].id, reductions[0].date.tm_year - 100, reductions[0].date.tm_mon + 1, reductions[0].date.tm_mday);
    

//     print_reductions();

//     account_t student = { .email = "student@example.com", .password = "password", .name = "Jane", .role = 0, .student_no = 123456 };
//     res = request_reduction(&student, cnt_r - 1); // Request reduction for the last registered reduction
//     if (res == 0) {
//         printf("Reduction request submitted successfully.\n");
//     } else {
//         printf("Failed to submit reduction request. Error code: %d\n", res);
//     }

//     return 0;
// }
