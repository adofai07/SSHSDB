#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

#include "makeaccount.h"

#define MAX_REQUEST 500

struct request {
    int idx;
    int accepted;
    char request[256];
};
typedef struct request request_t;

/// @brief generates request files for new teacher user
/// @param tch pointer to teacher account
/// @return 0 if successful, 1 if move file already exists, 2 if out file already exists, 3 if failed to create move file, 4 if failed to create out file
int make_request_file(char *name)
{
    FILE *fp_move, *fp_out;
    char move_filename[256], out_filename[256], rd_filname[256];

    // 파일 이름 포인터로 생성
    snprintf(move_filename, sizeof(move_filename), ".%s_move.txt", name);
    snprintf(out_filename, sizeof(out_filename), ".%s_out.txt", name);
    snprintf(rd_filname, sizeof(rd_filname), "./%s_reduction.txt", name)

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
        fp = fopen(path, "r");
        if (fp == NULL){
            // file not found
            fclose(fp);
            fp = NULL;
            return 1;
        } else {
            // file found
            fclose(fp);
            fp = NULL;
            fp = fopen(path, "a");
            if (len_request(fp) < MAX_REQUEST) {
                int idx = len_request(fp);
                fprintf(fp, "%3d 0 ", idx);      // '0' is for accepted == False
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

/// @brief load all requests in a request file to an array of request_t
/// @param path path to request file
/// @return a pointer pointing to the start of the array, NULL if error
request_t *load_requests(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        return NULL
    }
    int len = len_request(fp);
    request_t *requests = malloc(sizeof(request_t) * len);
    int idx;
    int accepted;
    char *request;
    for (int i=0; i<len; i++) {
        fscanf(fp, "%3d %d %s\n", &idx, &accepted, request);
        requests[i].idx = idx;
        requests[i].accepted = accepted;
        strcpy(requests[i].request, request);
    }
    return requests
}

/// @brief Sets 'accepted' to 1 for request with specified index
/// @param requests pointer to array of request_t
/// @param idx index to accept
/// @return 0 if successful, 1 if already accepted, 2 if idx not found
int accept_request(request_t *requests, int idx)
{
    int len = sizeof(requests) / sizeof(reqest_t);
    for (int i=0; i<len; i++) {
        if (requests[i].idx == idx) {
            if (requests[i].accepted == 0) {
                requests[i].accepted = 1;
                return 0;
            } else {
                return 1;
            }
        }
    }
    return 2;
}

/// @brief Rewrites reqeust file from updated request_t
/// @param path path to request file
/// @param requests pointer to array of request_t
/// @return 0 if successful, -1 if path invalid
int update_request(const char *path, request_t *requests)
{
    int len = sizeof(requests) / sizeof(reqest_t);
    int option = option_from_path(path);
    if (option <= 0) {
        return -1;
    }
    FILE *fp = fopen(path, "w");
    for (int i=0; i<len; i++) {
        if (request_is_valid(requests[i].request, option) == 1) {
            fprintf(fp, "%3d %d %s\n", requests[i].idx, requests[i].accepted, requests[i].request);
        } else {
            printf("Request with index %3d is invalid.\n", requests[i].idx);
            fprintf(fp, "%3d %d Invalid Request\n", requests[i].idx, requests[i].accepted);
        }
    }
    fclose(fp);
    fp = NULL;
    return 0;
}
