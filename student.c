#include <stdio.h>
#include <string.h>
#include <time.h>

#include "place.h"
#include "student.h"
#include "typing.h"

/// @brief Sets the current time to the provided tm structure
/// @param t double pointer to a tm structure
/// @return 0 on success
int timeset(struct tm **t)
{
    time_t timer = time(NULL);
    *t = localtime(&timer); // Get the current time and convert it to local time
    return 0;
}

/// @brief Checks if a classroom is in the provided list
/// @param list array of classroom names
/// @param classroom name of the classroom to check
/// @return 1 if classroom is in the list, 0 otherwise
int in_list_classroom(const char *list[], const char *classroom)
{
    for (int i = 0; list[i] != NULL; i++)
    {
        if (strcmp(classroom, list[i]) == 0) // Compare the classroom with each entry in the list
            return 1;
    }
    return 0;
}

/// @brief Finds the index of a classroom in the provided list
/// @param list array of classroom names
/// @param classroom name of the classroom to find
/// @return index of the classroom in the list, INVLDCLSRM if not found
int index_classroom(const char *list[], const char *classroom)
{
    for (int i = 0; list[i] != NULL; i++)
    {
        if (strcmp(classroom, list[i]) == 0) // Compare the classroom with each entry in the list
            return i;
    }
    return INVLDCLSRM;
}

/// @brief Counts the occurrences of a specified classroom in a file
/// @param p period number (1 or 2)
/// @param classroom name of the classroom to count
/// @return count of the classroom occurrences, error codes on failure
int count_place(int p, const char *classroom)
{
    int count = 0;
    int student_number;
    char student_name[15];
    int some_count_1;
    int some_count_2;
    char place_1[5];
    char place_2[5];

    if (!in_list_classroom(all_plc, classroom))
    {
        return INVLDCLSRM;
    }

    FILE *fp_sheet = fopen("sheet.txt", "r");
    if (fp_sheet == NULL)
    {
        // Error opening file
        return FILEERROR;
    }

    // Reading the file and counting the matching classrooms
    while (fscanf(fp_sheet, "%d %s %d %s %d %s", &student_number, student_name,
                  &some_count_1, place_1, &some_count_2, place_2) != EOF)
    {
        if (p == 1 && strcmp(classroom, place_1) == 0) // Count for period 1
        {
            count++;
        }
        else if (p == 2 && strcmp(classroom, place_2) == 0) // Count for period 2
        {
            count++;
        }
    }

    fclose(fp_sheet);
    return count;
}

/// @brief Checks if a classroom is full for a given period
/// @param p period number (1 or 2)
/// @param classroom name of the classroom to check
/// @return 1 if the classroom is full, 0 otherwise, error codes on failure
int is_full_place(int p, const char *classroom)
{
    int count = count_place(p, classroom);
    if (count < 0)
    {
        return count; // Return the error code from count_place
    }

    // Check if the classroom is in the all_plc list and compare with the limit
    for (int i = 0; all_plc[i] != NULL; i++)
    {
        if (strcmp(classroom, all_plc[i]) == 0)
        {
            return count >= classroom_limits[i];
        }
    }

    return INVLDCLSRM; // classroom not found
}

/// @brief Finds the position of a student number in the file
/// @param sno student number to find
/// @param file pointer to the file to search
/// @return 0 on success, FILEERROR on failure
int find_sno(int sno, FILE *file)
{
    if (file == NULL)
    {
        return FILEERROR;
    }

    fseek(file, 0, SEEK_SET); // Move the file pointer to the start of the file
    int student_number = 0;
    char buffer[100];

    // Reading the file until finding student's number
    while (fscanf(file, "%d", &student_number) != EOF)
    {
        if (sno == student_number) // Break if the student number matches
            break;
        else
        {
            fgets(buffer, 100, file); // Read the rest of the line
        }
    }
    fseek(file, ftell(file) - 4, SEEK_SET); // Move the file pointer back to the start of the student number
    return 0;
}

/// @brief Saves student information to a sheet block
/// @param sno student number to save
/// @param student pointer to the sheet block to save the information
/// @return 0 on success, FILEERROR on failure
int save_sno(int sno, sheet_block_t *student)
{
    int student_number;
    char student_name[15];
    int some_count_1;
    int some_count_2;
    char place_1[5];
    char place_2[5];

    FILE *fp_sheet = fopen("sheet.txt", "r+");
    if (fp_sheet == NULL)
    {
        // Error opening file
        return FILEERROR;
    }

    // Reading the file until finding student's number
    while (sno != student_number && fscanf(fp_sheet, "%d %s %d %s %d %s", &student_number, student_name,
                                           &some_count_1, place_1, &some_count_2, place_2) != EOF)
    {
        // Do nothing
    }

    if (sno != student_number)
    {
        fclose(fp_sheet);
        return INVLDACNT;
    }

    // Save the student information to the sheet block
    student->student_no = student_number;
    strcpy(student->name, student_name);
    strcpy(student->p1, place_1);
    strcpy(student->p2, place_2);

    fclose(fp_sheet);
    return 0;
}

/// @brief Updates the sheet file with the latest counts for each classroom
/// @return 0 on success, FILEERROR on failure
int update_sheet()
{
    int student_number;
    char student_name[15];
    int some_count_1;
    int some_count_2;
    char place_1[5];
    char place_2[5];
    int point1;
    int point2;
    FILE *fp = fopen("sheet.txt", "r+");
    if (fp == NULL)
    {
        // Error opening file
        return FILEERROR;
    }

    // Read the file and update counts for each student
    while (fscanf(fp, "%d", &student_number) != EOF)
    {
        point1 = ftell(fp); // Save the current file position
        fscanf(fp, " %s %d %s %d %s", student_name, &some_count_1, place_1, &some_count_2, place_2);
        point2 = ftell(fp);              // Save the new file position
        fseek(fp, point1 - 4, SEEK_SET); // Move the file pointer back to the student number
        fprintf(fp, "%4d %-10s %03d %4s %03d %4s", student_number, student_name,
                count_place(1, place_1), place_1, count_place(2, place_2), place_2);
        fseek(fp, point2, SEEK_SET); // Move the file pointer to the next student
    }

    fclose(fp);
    return 0;
}

/// @brief Prints the available classrooms for a given period
/// @param acc pointer to the account structure
/// @param p period number (1 or 2)
/// @return 0 on success, errorcodes on failure
int print_available(account_t *acc, int p)
{
    if (p!=1 && p!=2)
    {
        return INVLDINDEX;
    }

    FILE *fp_sheet = fopen("sheet.txt", "r");
    if (fp_sheet == NULL)
    {
        // Failed to open file
        return FILEERROR;
    }

    printf("Available places for period %d:\n", p);

    // Print all classrooms that are not full
    for (int i = 0; all_plc[i] != NULL; i++)
    {
        if (!is_full_place(p, all_plc[i]))
        {
            printf("%s\n", all_plc[i]);
        }
    }

    fclose(fp_sheet);
    return 0;
}

/// @brief Prints the students in a given classroom for a specified period
/// @param p period number (1 or 2)
/// @param classroom name of the classroom
/// @return 0 on success, errorcodes on failure
int print_place(int p, char *classroom)
{

    if (p!=1 && p!=2)
    {
        return INVLDINDEX;
    }

    if (!in_list_classroom(all_plc, classroom))
    {
        return INVLDCLSRM; // Cannot find the classroom
    }

    FILE *fp_sheet = fopen("sheet.txt", "r");
    if (fp_sheet == NULL)
    {
        // Failed to open file
        return FILEERROR;
    }

    int buffer_sno;
    char buffer_name[15];
    int buffer_count_1;
    int buffer_count_2;
    char buffer_place_1[5];
    char buffer_place_2[5];

    printf("Students in classroom %s during period %d:\n", classroom, p);

    // Read the file and print students in the specified classroom for the given period
    if (p == 1)
    {
        while (fscanf(fp_sheet, "%d %s %d %s %d %s", &buffer_sno, buffer_name,
                      &buffer_count_1, buffer_place_1, &buffer_count_2, buffer_place_2) != EOF)
        {
            if (strcmp(classroom, buffer_place_1) == 0)
            {
                printf("Student Number: %d, Name: %s\n", buffer_sno, buffer_name);
            }
        }
    }

    else if (p == 2)
    {
        while (fscanf(fp_sheet, "%d %s %d %s %d %s", &buffer_sno, buffer_name,
                      &buffer_count_1, buffer_place_1, &buffer_count_2, buffer_place_2) != EOF)
        {
            if (strcmp(classroom, buffer_place_2) == 0)
            {
                printf("Student Number: %d, Name: %s\n", buffer_sno, buffer_name);
            }
        }
    }

    fclose(fp_sheet);
    return 0;
}

/// @brief Moves an individual student to a specified classroom for a given period
/// @param sno student number
/// @param p period number (1 or 2)
/// @param classroom name of the classroom
/// @param tch pointer to the teacher's account
/// @return 0 on success, various error codes on failure
int move_individual(int sno, int p, char *classroom, account_t *tch)
{
    if (p!=1 && p!=2)
    {
        return INVLDINDEX;
    }

    if (!in_list_classroom(all_plc, classroom))
    {
        return INVLDCLSRM; // Cannot find the classroom
    }

    int is_full = is_full_place(p, classroom);
    if (is_full < 0)
    {
        return is_full; // Return the error code from is_full_place
    }

    if (is_full)
    {
        return FULLCLSRM; // The classroom is full
    }

    if (in_list_classroom(opt_plc, classroom))
    {
        sheet_block_t student;
        int res = save_sno(sno, &student); // Save the student information
        if (res != 0)
        {
            return res;
        }

        FILE *fp_sheet = fopen("sheet.txt", "r+");
        if (fp_sheet == NULL)
        {
            // Error opening file
            return FILEERROR;
        }

        res = find_sno(sno, fp_sheet); // Find the student number in the file
        if (res != 0)
        {
            fclose(fp_sheet);
            printf("error");
            return res;
        }

        if (p == 1)
        {
            fprintf(fp_sheet, "%d %-10s %03d %s %03d %s", student.student_no, student.name,
                    count_place(1, classroom) + 1, classroom, count_place(2, student.p2), student.p2);
        }
        else if (p == 2)
        {
            fprintf(fp_sheet, "%d %-10s %03d %s %03d %s", student.student_no, student.name,
                    count_place(1, student.p1), student.p1, count_place(2, classroom) + 1, classroom);
        }
        fclose(fp_sheet);
    }

    else
    {
        if (tch == NULL)
        {
            // Cannot find teacher account
            return INVLDACNT;
        }

        char filename[100];
        sprintf(filename, "%s_move.txt", tch->name);
        FILE *fp = fopen(filename, "a");

        if (fp == NULL)
        {
            // Error opening request file
            return FILEERROR;
        }

        struct tm *t;
        timeset(&t); // Set the current time
        fprintf(fp, "[%2d.%02d.%02d] [%d] [%d] [%s]\n", (t->tm_year) % 100,
                (t->tm_mon) + 1, (t->tm_mday), sno, p, classroom);
        fclose(fp);
    }

    // Apply change of student count to the sheet
    update_sheet();

    return 0;
}

/// @brief Moves a group of students to a specified classroom for a given period
/// @param grp pointer to the group structure
/// @param p period number (1 or 2)
/// @param classroom name of the classroom
/// @param tch pointer to the teacher's account
/// @return 0 on success, various error codes on failure
int move_group(group_t *grp, int p, char *classroom, account_t *tch)
{
    if (p!=1 && p!=2)
    {
        return INVLDINDEX;
    }

    if (!in_list_classroom(all_plc, classroom))
    {
        return INVLDCLSRM; // Cannot find the classroom
    }

    if (count_place(p, classroom) + grp->size > classroom_limits[index_classroom(all_plc, classroom)])
    {
        return FULLCLSRM;
    }

    if (grp == NULL)
    {
        return INVLDGROUP;
    }

    if (in_list_classroom(opt_plc, classroom))
    {
        for (int i = 0; i < grp->size; i++)
        {
            int res = move_individual((grp->members)[i], p, classroom, tch); // Move each student individually
            if (res != 0)
            {
                return res;
            }
        }
        return 0;
    }
    else
    {
        if (tch == NULL)
        {
            // Cannot find teacher account
            return INVLDACNT;
        }

        char filename[100];
        sprintf(filename, "%s_move.txt", tch->name);
        FILE *fp = fopen(filename, "a");

        if (fp == NULL)
        {
            // Error opening request file
            return FILEERROR;
        }

        struct tm *t;
        timeset(&t); // Set the current time
        for (int i = 0; i < grp->size; i++)
        {
            fprintf(fp, "[%2d.%02d.%02d] [%d] [%d] [%s]\n", (t->tm_year) % 100,
                    (t->tm_mon) + 1, (t->tm_mday), (grp->members)[i], p, classroom);
        }
        fclose(fp);
    }

    // Apply change of student count to the sheet
    update_sheet();

    return 0;
}

/// @brief Logs the outing time for an individual student
/// @param acc pointer to the student's account
/// @param leave leave time
/// @param rtn return time
/// @param tch pointer to the teacher's account
/// @return 0 on success, various error codes on failure
int outing_individual(account_t *acc, int leave, int rtn, account_t *tch)
{
    if (leave > rtn)
    {
        return ETCERROR; // Invalid time range
    }

    if (tch == NULL)
    {
        return INVLDACNT; // Teacher account not found
    }

    char filename[100];
    sprintf(filename, "%s_out.txt", tch->name);
    FILE *fp = fopen(filename, "a");
    if (fp == NULL)
    {
        // Error opening request file
        return FILEERROR;
    }

    struct tm *t;
    timeset(&t); // Set the current time
    fprintf(fp, "[%2d.%02d.%02d] [%d] [%d:00] [%d:00]\n", (t->tm_year) % 100,
            (t->tm_mon) + 1, (t->tm_mday), acc->student_no, leave, rtn);
    fclose(fp);
    return 0;
}

/// @brief Logs the outing time for a group of students
/// @param grp pointer to the group structure
/// @param leave leave time
/// @param rtn return time
/// @param tch pointer to the teacher's account
/// @return 0 on success, various error codes on failure
int outing_group(group_t *grp, int leave, int rtn, account_t *tch)
{
    if (leave > rtn)
    {
        return ETCERROR; // Invalid time range
    }

    if (tch == NULL)
    {
        return INVLDACNT; // Teacher account not found
    }

    if (grp == NULL)
    {
        return INVLDGROUP;
    }

    char filename[100];
    sprintf(filename, "%s_out.txt", tch->name);
    FILE *fp = fopen(filename, "a");
    if (fp == NULL)
    {
        // Error opening request file
        return FILEERROR;
    }

    struct tm *t;
    timeset(&t); // Set the current time
    for (int i = 0; i < grp->size; i++)
    {
        fprintf(fp, "[%2d.%02d.%02d] [%d] [%d:00] [%d:00]\n", (t->tm_year) % 100,
                (t->tm_mon) + 1, (t->tm_mday), (grp->members)[i], leave, rtn);
    }

    fclose(fp);
    return 0;
}

/// @brief Moves a student home for both periods
/// @param acc pointer to the student's account
/// @param tch pointer to the teacher's account
/// @return 0 on success
int home(account_t *acc, account_t *tch)
{
    int res1 = move_individual(acc->student_no, 1, "home", tch); // Move student home for period 1
    int res2 = move_individual(acc->student_no, 2, "home", tch); // Move student home for period 2
    return (res1 == 0 && res2 == 0) ? 0 : ETCERROR;
}
