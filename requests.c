#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

#include "makeaccount.h"

#define MAX_REQUEST 500

/// @brief generates request files for new teacher user
/// @param tch pointer to teacher account
/// @return 0 if successful, 1 if move file already exists, 2 if out file already exists, 3 if failed to create move file, 4 if failed to create out file
int make_request_file(account_t *tch)
{
    FILE *fp_move, *fp_out;
    char move_filename[256], out_filename[256];

    // 파일 이름 포인터로 생성
    snprintf(move_filename, sizeof(move_filename), "./requests/%s_move.txt", tch->name);
    snprintf(out_filename, sizeof(out_filename), "./requests/%s_out.txt", tch->name);

    // move 파일 존재 여부 확인
    fp_move = fopen(move_filename, "r");
    if (fp_move != NULL){
        fclose(fp_move);
        return 1;
    }

    // move 파일 생성
    fp_move = fopen(move_filename, "w");
    if (fp_move == NULL){
        return 3;
    }
    fclose(fp_move);

    // out 파일 존재 여부 확인
    fp_out = fopen(out_filename, "r");
    if (fp_out != NULL){
        fclose(fp_out);
        return 2;
    }

    // out 파일 생성
    fp_out = fopen(out_filename, "w");
    if (fp_out == NULL){
        return 4;
    }
    fclose(fp_out);

    return 0;
}

/// @brief regex flow for different expressions
/// @param expression regex char
/// @param s string to check
/// @param len length of string
/// @return -1 if regex compilation or match error, 0 if no match, 1 if match
int check_regex(const char *expression, char *s, int len)
{
    regex_t regex;
    int reti = regcomp(&regex, expression, REG_EXTENDED);

    // Regex compilation error
    if (reti) {
        fprintf(stderr, "Regex compilation error.\n");
        return -1;
    }

    reti = regexec(&regex, s, 0, NULL, 0);
    regfree(&regex);

    if (!reti) {
        // Match
        return 1;
    } else if (reti == REG_NOMATCH) {
        // No match
        return 0;
    } else {
        // Error
        fprintf(stderr, "Regex match failed:%s\n", s);
        return -1;
    }
}

/// @brief checks is request format is valid based on option
/// @param request pointer to request string
/// @param option integer value for request type
/// @return 1 if request is valid, 0 if not, -1 if error
int request_is_valid(char *request, int option)
{
    // option 1 => [YY.MM.DD] [email] [password] [name] [role] [*args]
    // option 2 => [YY.MM.DD] [email] [password] [name] [role] [*args]
    // option 3 => [YY.MM.DD] [student_id] [period] [location]
    // option 4 => [YY.MM.DD] [student_id] [leave_timecode] [return_timecode]

    // 시각 데이터 regex
    const char *date_regex = "^(\d{2})\.(0[1-9]|1[0-2])\.(0[1-9]|[12][0-9]|3[01])$";
    // password regex
    const char *pw_regex = "^[A-Za-z\d!@#$%^&*()]$";
    // role regex
    const char *role_regex = "^[012]$";
    // student_id regex
    const char *sid_regex = "^[1-3][1-8](0[1-9]|1[0-8])$";
    // period regex
    const char *period_regex = "^[12]";
    //location regex
    const char *location_regex = "^[a-z]{3}[a-z1-9]$";
    // timecode regex
    const char *timecode_regex = "^[1-9][0-9]*$";
    // email, name은 makeaccount.c의 함수 is_valid_email, is_valid_name 사용

    // 각 option에 대한 판단을 위한 switch case
    char *token;
    int result;
    switch(option)
    {
    case 1: case 2:
        token = strtok(request, " ");
        if (token == NULL) {
            return 0;
        } else if ((result = check_regex(date_regex, token, strlen(token))) != 1) {
            // not match or error
            return result;
        }
        token = strtok(NULL, " ");
        if (token == NULL) {
            return 0;
        } else if ((result = is_valid_email(token, strlen(token))) != 1) {
            // not valid email or error
            return result;
        }
        token = strtok(NULL, " ");
        if (token == NULL) {
            return 0;
        } else if ((result = check_regex(pw_regex, token, strlen(token))) != 1) {
            // not match or error
            return result;
        }
        token = strtok(NULL, " ");
        if (token == NULL) {
            return 0;
        } else if ((result = is_valid_name(token)) != 1) {
            // not valid email or error
            return result;
        }
        token = strtok(NULL, " ");
        if (token == NULL) {
            return 0;
        } else if ((result = check_regex(role_regex, token, strlen(token))) != 1) {
            // not match or error
            return result;
        } else {
            // All regex matched
            return 1;
        }
        break;
    case 3:
        token = strtok(request, " ");
        if (token == NULL) {
            return 0;
        } else if ((result = check_regex(date_regex, token, strlen(token))) != 1) {
            // not match or error
            return result;
        }
        token = strtok(NULL, " ");
        if (token == NULL) {
            return 0;
        } else if ((result = check_regex(sid_regex, token, strlen(token))) != 1) {
            // not match or error
            return result;
        }
        token = strtok(NULL, " ");
        if (token == NULL) {
            return 0;
        } else if ((result = check_regex(period_regex, token, strlen(token))) != 1) {
            // not match or error
            return result;
        }
        token = strtok(NULL, " ");
        if (token == NULL) {
            return 0;
        } else if ((result = check_regex(location_regex, token, strlen(token))) != 1) {
            // not match or error
            return result;
        } else {
            // All regex matched
            return 1;
        }
        break;
    case 4:
        token = strtok(request, " ");
        if (token == NULL) {
            return 0;
        } else if ((result = check_regex(date_regex, token, strlen(token))) != 1) {
            // not match or error
            return result;
        }
        token = strtok(NULL, " ");
        if (token == NULL) {
            return 0;
        } else if ((result = check_regex(sid_regex, token, strlen(token))) != 1) {
            // not match or error
            return result;
        }
        token = strtok(NULL, " ");
        if (token == NULL) {
            return 0;
        } else if ((result = check_regex(timecode_regex, token, strlen(token))) != 1) {
            // not match or error
            return result;
        }
        token = strtok(NULL, " ");
        if (token == NULL) {
            return 0;
        } else if ((result = check_regex(timecode_regex, token, strlen(token))) != 1) {
            // not match or error
            return result;
        } else {
            // All regex matched
            return 1;
        }
        break;
    }
}

/// @brief evaluates option number from file path
/// @param path path to check
/// @return -1 if error, 0 if no match, 1 if admin_accountRequest, 2 if admin_recovery, 3 if move, 4 if out
int option_from_path(char *path)
{
    if (strcmp(path, "admin_accountRequest.txt") == 0) {
        // match with admin_accountRequest.txt
        return 1;
    } else if (strcmp(path, "admin_recovery.txt") == 0) {
        // match with admin_recovery.txt
        return 2;
    } else {
        char *token;
        int result;
        token = strtok(path, "_");
        if (token == NULL) {
            return 0;
        } else if ((result = is_valid_name(token)) != 1) {
            return result;
        } else {
            // teacher_name part matches
            token = strtok(NULL, "_");
            if (token == NULL) {
                return 0;
            } else if (strcmp(token, "move.txt") == 0) {
                // move file
                return 3;
            } else if (strcmp(token, "out.txt") == 0) {
                // out file
                return 4;
            } else {
                // no match
                return 0;
            }
        }
    }
}

/// @brief returns number of requests in request file
/// @param fp FILE pointer to file
/// @return -1 error, integer number of requests in request file if else
int len_request(FILE *fp)
{
    int count = 0;
    int c;
    while ((c = getc(fp)) != EOF) {
        // add count if c == '\n' (각 줄마다 add count)
        count += (c == '\n');
    }
    fclose(fp);
    fp = NULL;
    return count;
}


/// @brief writes request to specified file
/// @param path path to request file
/// @param request pointer to request string
/// @return 0 if successful, 1 if file not found, 2 if request not valid, 3 if too many requests in file
int write_request(char *path, char *request)
{
    int option  = option_from_path(path);
    if (request_is_valid(request, option) == 1) {
        // request is valid
        FILE *fp;
        char req_path[256];
        snprintf(req_path, sizeof(req_path), "./requests/%s", path);
        fp = fopen(req_path, "r");
        if (fp == NULL){
            // file not found
            fclose(fp);
            fp = NULL;
            return 1;
        } else {
            // file found
            fclose(fp);
            fp = NULL;
            fp = fopen(req_path, "a");
            if (len_request(fp) < MAX_REQUEST) {
                fprintf(fp, "%s\n", request);
                fclose(fp);
                fp = NULL;
                return 0;
            } else {
                fclose(fp);
                fp = NULL;
                return 3;
            }
        }
    } else {
        // request is not valid
        return 2;
    }
}

/// @brief Prints all requests in a request file
/// @param path path to request file
/// @return 0 if successful, 1 if error
int print_requests(char *path)
{
    char line[256];
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        return 1;
    } else {
        while (fscanf(fp, "%s\n", &line) != EOF) {
            printf("%s\n", line);
        }
        return 0;
    }
}

/// @brief load all requests in a request file to an array
/// @param path 
/// @return a double pointer pointing to the start of the array, NULL if error
char **load_requests(char *path)
{
    char loader[MAX_REQUEST][256];
    char line[256];
    int i = 0
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        return NULL;
    }

    char **lines = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        // remove newline character
        if (lne[read - 1] == '\n') {
            line[read - 1] = '\0';
            read--;
        }

        // Allocate space for new line
        char **new_lines = realloc(lines, (count + 1) * sizeof(char *));
        if (!new_lines) {
            perror("Failed to allocate memory");
            for (int i=0; i<count; i++) {
                free(lines[i]);
            }
            free(lines);
            free(line);
            fclose(fp);
            return NULL;
        }
        lines = new_lines;
        lines[count] = strdup(line);    // Copy line into array
        if (!lines[count]) {
            perror("Failed to allocate memory for line");
            break;
        }
        count++;
    }
    free(line);     // Free the vuffer allocated by getline
    fclose(fp);
    return lines;
}
