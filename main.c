#include <assert.h>
#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "typing.h"
#include "effects.h"
#include "getinput.h"
#include "getsheet.h"
#include "groups.h"
#include "makeaccount.h"
#include "requests.h"
#include "reductions.h"
#include "u8string.h"
#include "student.h"
#include "proposal.h"
#include "getenv.h"
#include "notes.h"

/// @brief Pauses flow until ENTER is pressed
void wait_until_enter() {
    printf("Press ENTER to return to menu. ");
    getchar(); while (getchar() != '\n');
}

/// @brief The flow will be in this loop as long as the user is logged in as a student account
/// @return 0 if no errors, non-zero integer if there were errors
int student_loop(account_t *acc) {
    int choice;

    while (1) {
        system("cls");

        char *c = student_menu(acc);
        printf("%s", c);
        free(c);

        choice = get_input(1, 7);

        if (choice == 1) {
            break;

        } else if (choice == 2) {
            int period;
            char sno_or_group[30];
            char place[10];

            printf("What period do you want to change? (1 or 2): ");
            scanf("%d", &period);
            printf("Enter student number or group name: ");
            scanf(" %s", sno_or_group);
            printf("Enter place. Eligible places are: \n\n");
            print_all_places();
            printf("\n>> ");
            scanf(" %s", place);

            if (str_is_sno(sno_or_group)) {
                set_sheet_by_sno(atoi(sno_or_group), period, place);
            } else {
                group_t *gr = find_group_by_name(sno_or_group);

                for (int i = 0; i < gr->size; i++) {
                    set_sheet_by_sno(gr->members[i], period, place);
                }
            }


        } else if (choice == 3) {
            char group_name[30];
            int group_size;
            int group_members[MAX_GROUP_SIZE];

            printf("Enter a name for the group: ");
            scanf("%s", group_name);
            printf("Enter group size: ");
            // scanf("%d", &group_size);
            group_size = get_input(1, get_max_group_size());
            printf("Enter student numbers for members. Use spaces/tabs/newlines as separators. ");

            for (int i = 0; i < group_size; i++) {
                scanf("%d", &group_members[i]);
            }

            make_group(group_name, group_members, group_size);

            wait_until_enter();

        } else if (choice == 4) {
            print_groups();
            printf("\n\n");

            wait_until_enter();

        } else if (choice == 5) {
            int idx;
            print_reductions();
            fputs("\n", stdout);

            printf("Select the reduction number: ");
            idx = get_input(0, reductions_cnt() - 1);

            request_reduction(acc, idx);

        } else if (choice == 6) {
            char curr_pw[100];
            char new_pw[100];

            printf("Current password: ");
            scanf(" %s", curr_pw);
            printf("New password: ");
            scanf(" %s", new_pw);

            if (strcmp(curr_pw, new_pw) == 0) {
                printf("Your new password cant be the same as the old one.\n");
                wait_until_enter();

            } else {
                account_t *sptr = account_ptr(acc->email);
                strcpy(sptr->password, new_pw);
                write_accounts();
            }

        } else if (choice == 7) {
            int sno;

            printf("Enter a student number: ");
            scanf("%d", &sno);

            fputs("\n", stdout);
            print_sheet_idx(sheet_idx(sno));
            fputs("\n", stdout);

            wait_until_enter();

        } else if (choice == 8) {
            scan_proposal(acc);
        }
    }

    return 0;
}

/// @brief The flow will be in this loop as long as the user is logged in as a teacher account
/// @return 0 if no errors, non-zero integer if there were errors
int teacher_loop(account_t *acc) {
    int choice;

    while (1) {
        system("cls");

        char *c = teacher_menu(acc);
        printf("%s", c);
        free(c);

        choice = get_input(1, 5);

        if (choice == 1) {
            break;

        } else if (choice == 2) {
            int prd;

            printf("Which period do you want to see? (1 or 2): ");
            scanf("%d", &prd);

            groupby_sheet(prd);

            wait_until_enter();

        } else if (choice == 3) {
            printf("Mode: login(teacher) -> Student requests\n\n");
            printf("1. Move requests\n2. Outing requests\n\n>> ");

            choice = get_input(1, 2);

            if (choice == 1) {
                char filename[256];
                printf("%s\n", filename);
                snprintf(filename, sizeof(filename), "%s_move.txt", acc->email);
                print_requests(filename);
                request_t *requests = load_requests(filename);

                int idx, e;

                printf("%s\n", filename);

                while (1) {
                    printf("Enter request index to accept, '-1' to exit:\n\n>> ");
                    scanf("%d", &idx);

                    if (idx == -1) {
                        update_request(filename, requests);
                        break;

                    } else {
                        e = accept_request(requests, idx);

                        if (e == 1) {
                            printf("Request of index %3d is already accepted\n\n", idx);

                        } else if (e == 2) {
                            printf("Index not found.\n\n");

                        }
                    }
                }

            } else if (choice == 2) {
                char filename[256];
                snprintf(filename, sizeof(filename), "%s_out.txt", acc->email);
                print_requests(filename);
                request_t *requests = load_requests(filename);

                int idx, e;

                while (1) {
                    printf("Enter request index to accept, '-1' to exit:\n\n>> ");
                    scanf("%d", &idx);

                    if (idx == -1) {
                        update_request(filename, requests);
                        break;

                    } else {
                        e = accept_request(requests, idx);

                        if (e == 1) {
                            printf("Request of index %3d is already accepted\n\n", idx);

                        } else if (e == 2) {
                            printf("Index not found.\n\n");

                        }
                    }
                }
            }

        } else if (choice == 4) {
            printf("Mode: login(teacher) -> Reductions\n\n");
            printf("1. Make new reduction\n2. Accept reductions\n\n>> ");

            choice = get_input(1, 2);

            if (choice == 1) {
                int idx, e;
                struct tm date = {0};
                int time;
                char loc[50];
                int num;
                while (1) {
                    printf("Enter 1 to make new reduction, '-1' to exit:\n\n>> ");
                    scanf("%d", &idx);
                    if (idx == -1) break;
                    else {
                        printf("Enter date[YY MM DD], time, location, number of student for new reduction:\n>> ");
                        scanf("%d %d %d %d %s %d", &date.tm_year, &date.tm_mon, &date.tm_mday, &time, loc, &num);
                        e = register_reduction(acc, &date, time, loc, num);
                        if (e == 1) {
                            printf("Time information is inappropriate.\n\n");
                        } else if (e == 2) {
                            printf("Location is not available.\n\n");
                        } else if (e == 4) {
                            printf("Error. Please try again.\n\n");
                        }
                    }
                }
            } else if (choice == 2) {
                print_tchreduction(acc);
                int idx, e;
                while (1) {
                    printf("Enter reduction index to accept, '-1' to exit:\n\n>> ");
                    scanf("%d", &idx);
                    if (idx == -1) break;
                    else {
                        e = accept_reduction(acc, idx);
                        if (e == 1) {
                            printf("Reduction of index %3d is already accepted.\n\n", idx);
                        } else if (e == 2) {
                            printf("Recruitment of the reduction exceeded.\n\n");
                        } else if (e == 3) {
                            printf("Index not found/\n\n");
                        } else if (e == 4) {
                            printf("Index not found.\n\n");
                        } else if (e == 0) {
                            update_reduction(acc);
                        }                        
                    }
                }
            } 
        } else if (choice == 5) {
            char curr_pw[100];
            char new_pw[100];

            printf("Current password: ");
            scanf(" %s", curr_pw);
            printf("New password: ");
            scanf(" %s", new_pw);

            if (strcmp(curr_pw, new_pw) == 0) {
                printf("Your new password cant be the same as the old one.\n");
                wait_until_enter();

            } else {
                account_t *sptr = account_ptr(acc->email);
                strcpy(sptr->password, new_pw);
                write_accounts();
            }

        }
    }
}

/// @brief The flow will be in this loop as long as the user is logged in as an admin account
/// @return 0 if no errors, non-zero integer if there were errors
int admin_loop(account_t *acc) {
    int choice;

    while (1) {
        system("cls");

        char *c = admin_menu(acc);
        printf("%s", c);
        free(c);

        choice = get_input(1, 5);

        if (choice == 1) {
            return 0;

        } else if (choice == 2) {
            const int WIDTH = 20;

            printf("\nemail");
            for (int i = 0; i < WIDTH - 5; i++) printf(" ");
            printf("pw");
            for (int i = 0; i < WIDTH - 2; i++) printf(" ");
            printf("+");
            for (int i = 0; i < WIDTH - 1; i++) printf(" ");
            printf("-");
            for (int i = 0; i < WIDTH - 1; i++) printf(" ");
            printf("name\n");
            for (int i = 0; i < WIDTH * 2 + 4; i++) printf("-");
            printf("\n");

            for (int i = 0; i < account_cnt(); i++) {
                account_t *a = find_account_by_idx(i);

                printf("%-*s%-*s%-*d%-*d%s\n", WIDTH, a->email, WIDTH, a->password,
                                               WIDTH, a->pos_pts, WIDTH, a->neg_pts, a->name);
            }

        } else if (choice == 3) {
            int sno;
            int pts;

            printf("Enter student number: ");
            scanf("%d", &sno);

            account_t *a = find_account_by_sno(sno);

            if (a == NULL) {
                printf("Student number not found.\n");
            } else {
                printf("Enter amount of points: ");
                scanf("%d", &pts);

                a->pos_pts += pts;

                write_accounts();

                printf("%s.pos_pts: %d -> %d\n", a->name, a->pos_pts - pts, a->pos_pts);
            }
        } else if (choice == 4) {
            int sno;
            int pts;

            printf("Enter student number: ");
            scanf("%d", &sno);

            account_t *a = find_account_by_sno(sno);

            if (a == NULL) {
                printf("Student number not found.\n");
            } else {
                printf("Enter amount of points: ");
                scanf("%d", &pts);

                a->neg_pts += pts;

                write_accounts();

                printf("%s.neg_pts: %d -> %d\n", a->name, a->neg_pts - pts, a->neg_pts);
            }

        } else if (choice == 5) {
            printf("There are %d note(s):\n\n", get_note_cnt());
            print_notes();

            printf("1. Pop oldest note\n2. Add new note\n>> ");
            choice = get_input(1, 2);

            if (choice == 1) {
                pop_note();
                write_notes();

            } else if (choice == 2) {
                printf("Enter a new note (max 100 chrs): ");
                while (getchar() != '\n');
                char buf[100];
                fgets(buf, 100, stdin);

                int s = strlen(buf);

                buf[s++] = '\n';
                buf[s] = '\0';

                add_note(buf);
                write_notes();
            }

        } else if (choice == 6) {
            int page;
            printf("Enter page of proposals to view. (1 ~ 10)\n\n>> ");
            scanf("%d", &page);
            print_proposal_list(page);

        } else if (choice == 7) {
            int index;
            printf("Enter index of proposal to view. (0 ~ 99)\n\n>> ");
            scanf("%d", &index);
            print_proposal(index);

        }

        printf("\n");
        wait_until_enter();

    }

    return 0;
}

int start() {
    // utf-8 encoding
    system("chcp 65001");

    effect_init();

    fputs(intro(), stdout);

    printf("Press ENTER to start >> ");
    while (getchar() != '\n');

    get_accounts();
    get_sheet();
    get_groups();
    get_reductions();
    get_notes();

    int choice;
    int role;

    while (1) {
        system("cls");

        printf("Terminal size: (%d, %d)\n", get_terminal_height(), get_terminal_width());
        printf("1. Exit\n2. Create account\n3. Sign in\n\n>> ");
        
        choice = get_input(1, 3);

        if (choice == 1) {
            // Exit
            break;
        } else if (choice == 2) {
            // Create account
            char email[50];
            char name[50];
            char password[50];
            char confirm_password[50];
            int student_no = 0;
            

            printf("Enter email: ");
            scanf("%s", email);
            printf("Enter name: ");
            scanf("%s", name);
            printf("Enter password: ");
            scanf("%s", password);
            printf("Confirm password: ");
            scanf("%s", confirm_password);
            printf("Enter desired role (0: Student, 1: Admin, 2: Teacher): ");
            scanf("%d", &role);

            // Enter student number if user is a student
            if (role == 0) {
                printf("Enter student number: ");
                scanf("%d", &student_no);
            } else if (role == 2) {
                make_request_file(email);
            }

            // Make account
            int is_valid = make_account(email, name, password, confirm_password, role, student_no);

            // In case make_account() failed, print an error message
            switch (is_valid) {
                case 1:
                    printf("Your didn't confirm your password correctly.\n\n");
                    break;

                case 2:
                    printf("Your email is invalid.\n\n");
                    break;

                case 3:
                    printf("There already exists an account with the same email.\n\n");
                    break;

                case 4:
                    printf("Your name is invalid. Name should contain 3 korean characters and 0-1 alphabets.\n\n");
                    break;

                case 5:
                    printf("Your student number is invalid.\n\n");
                    break;

                case 6:
                    printf("There are too many accounts.\n\n");
                    break;

                default:
                    printf("Account created successfully.\n\n");
            }

            wait_until_enter();

        } else if (choice == 3) {
            char email[50];
            char password[50];

            printf("Enter email: ");
            scanf("%s", email);
            printf("Enter password: ");
            scanf("%s", password);

            role = login(email, password);

            if (role == -1) {
                printf("Login failed.\n\n");

                wait_until_enter();
            } else {
                // system("cls");
                // printf("You are currently logged in as %s.\n\n", email);

                // Print the menu according to the role
                if (role == 0) {
                    student_loop(account_ptr(email));
                } else if (role == 1) {
                    admin_loop(account_ptr(email));
                } else {
                    teacher_loop(account_ptr(email));
                }
            }
        }
    }

    return 0;
}

int main() {
    return start();
}