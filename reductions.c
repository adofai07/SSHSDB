#include <stdio.h>
#include <time.h>
#include <string.h>

#include "typing.h"
#include "u8string.h"
#include "makeaccount.h"
#include "reductions.h"

// Struct for reduction
struct reduction
{
    int id;
    struct tm date;
    int time;
    char loc[20];
    int num;
    char tch[50];
};

struct stureduc
{
    int id;
    int stu_no;
    char tch[50];
    int accepted;
};

typedef struct reduction reductions_t;
typedef struct stureduc stureduc_t;

// Set static to avoid name collisions with main.c
static reductions_t reductions[MAX_REDUCTION_CNT];
static stureduc_t stureducs[MAX_REDUCTION_CNT];
static int cnt_r = 0;
static int cnt_s = 0;

/// @brief Read reductions from file
/// @return 0 if no error, 1 if file read error
int get_reductions()
{
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
                   &reduc_time, reduc_loc, &reduc_num, reduc_tch))
    {
        reductions[c].id = reduc_id;
        reductions[c].date = reduc_date;
        reductions[c].time = reduc_time;
        strcpy(reductions[c].loc, reduc_loc);
        reductions[c].num = reduc_num;
        strcpy(reductions[c].tch, reduc_tch);
        c++;
        cnt_r++;
    }

    fclose(fptr);

    int id, _time, sno, accepted;
    char date[50], place[50];

    for (int i = 0; i < account_cnt(); i++) {
        account_t *ap = find_account_by_idx(i);

        if (ap->role == 2) {
            char filename[70];
            snprintf(filename, sizeof(filename), "%s_reduction.txt",
                    ap->email);

            fptr = fopen(filename, "r");

            while (~fscanf(fptr, "%d | %s %d | %s | %d %d",
                           &id, date, &_time, place, &sno, &accepted)) {
                stureducs[cnt_s].id = id;
                stureducs[cnt_s].stu_no = sno;
                stureducs[cnt_s].accepted = accepted;
                strcpy(stureducs[cnt_s].tch, ap->name);

                cnt_s++;
            }
        }
    }

    return 0;
}

/// @brief Prints all reductions
void print_reductions()
{
    printf("\n");
    printf("Reductions---------------------------------------\n");
    printf("\n");
    printf("id | date & time | location | staff num | teacher\n");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < cnt_r; i++)
    {
        printf("%2d | %02d.%02d.%02d %2d | %8s | %9d | %s\n",
               reductions[i].id, reductions[i].date.tm_year, reductions[i].date.tm_mon, reductions[i].date.tm_mday, reductions[i].time,
               reductions[i].loc, reductions[i].num, reductions[i].tch);
    }
}

/// @brief Returns the number of reductions
/// @return number of reductions
int reductions_cnt() {
    return cnt_r;
}

/// @brief register reduction
/// @param tch pointer to account struct object (teacher)
/// @param date0 pointer to struct tm object for reduction date
/// @param time0 value to specify reduction time (changes necessary)
/// @param loc0 value to specify reduction location (enum?)
/// @param num0 maximum number of applicable students
/// @return 0 if no errors, 1 if time inappropriate, 2 if loc not available,
///         3 if not teacher, 4 if error while writing to request file
int register_reduction(account_t *tch, struct tm *date0, int time0, const char *loc0, int num0)
{
    // Error that time is inappropriate
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    date0->tm_year += 100;
    date0->tm_mon -= 1;

    time_t t1 = mktime(date0);
    time_t t2 = mktime(tm_now);

    if (difftime(t1, t2) < 0)
        return 1;
        
    date0->tm_year -= 100;
    date0->tm_mon += 1;

    // Error that location is not available
    int found = -1;
    const char *reduc_loc[22] = {"sem1", "sem2", "sem3", "sem4", "libr",
                                 "emp1", "emp2", "emp3", "eng1", "eng2", "eng3", "phy1", "phy2",
                                 "phy3", "ear1", "ear2", "med1", "med2", "scix", "geox", "meet", "hist"};
    for (int i = 0; i < 22; i++)
    {
        if (strcmp(loc0, reduc_loc[i]) == 0)
        {
            found = i;
            break;
        }
    }
    if (found == -1)
        return 2;

    // Error that the request is not made to teacher
    if (tch->role != 2)
        return 3;

    // Error while writing to request file
    FILE *fp_rgr = fopen("reduction.txt", "a+");
    if (!fp_rgr)
        return 4;

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

/// @brief Application for a reduction in penalty points
/// @param acc pointer to account struct object (student)
/// @param red_id specified reduction id to request application
/// @return 0 if no errors, 1 if reduc_id not found, 2 if time is inappropriate,
///         3 if num is inappropriate, 4 if error while writing to request file
int request_reduction(account_t *acc, int red_id)
{
    // Error that reduc_id not found
    if (red_id >= cnt_r)
        return 1;

    // Error that time is inappropriate
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    reductions[red_id].date.tm_year += 100;
    reductions[red_id].date.tm_mon -= 1;

    time_t t1 = mktime(&reductions[red_id].date);
    time_t t2 = mktime(tm_now);

    if (difftime(t1, t2) < 0)
    {
        reductions[red_id].date.tm_year -= 100;
        reductions[red_id].date.tm_mon += 1;
        return 2;
    }
    reductions[red_id].date.tm_year -= 100;
    reductions[red_id].date.tm_mon += 1;
    // Error that num is inappropriate
    if (reductions[red_id].num == 0)
        return 3;

    // Error while writing to request file
    char rqr_filename[70];
    snprintf(rqr_filename, sizeof(rqr_filename), "%s_reduction.txt",
             find_account_by_name(reductions[red_id].tch)->email);
    FILE *fp_rqr = fopen(rqr_filename, "a+");
    if (!fp_rqr)
        return 4;

    fprintf(fp_rqr, "%03d | %02d.%02d.%02d %02d | %s | %d 0\n",
            red_id, reductions[red_id].date.tm_year, reductions[red_id].date.tm_mon, reductions[red_id].date.tm_mday,
            reductions[red_id].time, reductions[red_id].loc, acc->student_no);
    fclose(fp_rqr);

    stureducs[cnt_s].id = red_id;
    stureducs[cnt_s].stu_no = acc->student_no;
    strcpy(stureducs[cnt_s].tch, reductions[red_id].tch);
    stureducs[cnt_s].accepted = 0;
    cnt_s++;

    return 0;
}

/// @brief Prints reductions for each teacher in a file
/// @param tch pointer to account struct object (teacher)
/// @return 0 if successful, 1 if error
int print_tchreduction(account_t *tch)
{
    char filename[70];
    snprintf(filename, sizeof(filename), "%s_reduction.txt",
             find_account_by_name((*tch).name)->email);
    char line[256];
    FILE *fp = fopen(filename, "r");

    // Error that file don't exist
    if (fp == NULL)
        return 1;

    int reduc_id;
    struct tm reduc_date;
    int reduc_time;
    char reduc_loc[20];
    int stu_no;
    int accepted;

    while (~fscanf(fp, "%d | %d.%d.%d %d | %s | %d %d\n",
                   &reduc_id, &reduc_date.tm_year, &reduc_date.tm_mon, &reduc_date.tm_mday,
                   &reduc_time, reduc_loc, &stu_no, &accepted))
    {
        printf("%03d | %02d.%02d.%02d %02d | %s | %d %d\n",
               reduc_id, reduc_date.tm_year, reduc_date.tm_mon, reduc_date.tm_mday,
               reduc_time, reduc_loc, stu_no, accepted);
    }
    fclose(fp);
    return 0;
}

/// @brief Sets 'accepted' to 1 for reduction with specified index
/// @param tch pointer to array of stureduc_t
/// @param idx index to accept
/// @return 0 if successful, 1 if already accepted, 2 if number of student is inappropriate,
///         3 if the reduction is not made by the teacher, 4 if idx not found
int accept_reduction(account_t *tch, int idx)
{
    int len = sizeof(stureducs) / sizeof(stureduc_t);

    for (int i = 0; i < len; i++)
    {
        if (stureducs[i].id == idx)
        {
            // Already accepted
            if (stureducs[i].accepted == 1)
                return 1;
            // Recruitment exceeded
            if (reductions[i].num == 0)
                return 2;
            // reduction not made by the teacher
            if (strcmp(reductions[i].tch, tch->name) != 0)
                return 3;

            reductions[i].num--;
            stureducs[i].accepted = 1;
            return 0;
        }
    }
    return 4;
}

/// @brief Rewrites reduction file from updated stureduc_t
/// @param tch pointer to array of stureduc_t
/// @return 0 if successful, 1 if error
int update_reduction(account_t *tch)
{
    char filename[70];
    snprintf(filename, sizeof(filename), "%s_reduction.txt",
             find_account_by_name((*tch).name)->email);
    int len = sizeof(stureducs) / sizeof(stureduc_t);
    FILE *fp = fopen(filename, "w");

    int id;
    for (int i = 0; i < len; i++)
    {
        if (strcmp(stureducs[i].tch, (*tch).name) == 0)
        {
            id = stureducs[i].id;
            fprintf(fp, "%03d | %02d.%02d.%02d %02d | %s | %d %d\n",
                    id, reductions[id].date.tm_year, reductions[id].date.tm_mon, reductions[id].date.tm_mday,
                    reductions[id].time, reductions[id].loc, stureducs[i].stu_no, stureducs[i].accepted);
        }
    }
    fclose(fp);
    return 0;
}


