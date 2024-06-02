#include <assert.h>
#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getinput.h"
#include "getsheet.h"
#include "groups.h"
#include "makeaccount.h"
#include "typing.h"
#include "u8string.h"
#include "getenv.h"

static int t_width = get_terminal_width();
static int t_height = get_terminal_height();

/// @brief Intro animation at starting page
void get_logo(int col, int row){
    FILE *fp = fopen("./logo.txt", "r");
    if (!fp) {
        printf("Failed to open logo file.\n");
        return;
    }

    char lines[7][97];
    for (int i=0; i<7; i++){
        if (fgets(lines[i], sizeof(lines[i]), fp) == NULL) {
            printf("Error reading file.\n");
            return;
        }
        lines[i][strcspn(lines[i], "\n")] = '\0';
    }

    int start_row = (row - 7) / 2;  // Center the logo vertically
    int start_col = (col - 96) / 2;
    int num_letters = 6;  // Number of letters (each 16 columns wide)

    // Display each letter one by one with a pause
    for (int j = 0; j < num_letters; j++) {
        for (int i = 0; i < 7; i++) {
            printf("\033[%d;%dH", start_row + i, start_col + 16 * j);
            printf("%.16s", &lines[i][16 * j]);
            fflush(stdout);  // Ensure the output is updated immediately
        }
        usleep(300000);  // Delay for 0.5 seconds
    }
    fclose(fp);
    usleep(1000000);
    printf("\033[%d;%dH", row - 5, 0);
}

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
        printf("You are currently logged in as %s.\n\n", acc->email);
        printf("1. Log out\n2. Check spreadsheet\n3. Edit spreadsheet\n4. Make group\n5. Check groups\n\n>> ");

        choice = get_input(1, 5);

        if (choice == 1) {
            break;

        } else if (choice == 2) {
            printf("\n");
            print_sheet_idx(sheet_idx(acc->student_no));
            printf("\n\n");

            wait_until_enter();

        } else if (choice == 3) {
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


        } else if (choice == 4) {
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

        } else if (choice == 5) {
            print_groups();
            printf("\n\n");

            wait_until_enter();
        }
    }

    return 0;
}

int start() {
    // utf-8 encoding
    system("chcp 65001");

    // printf("Welcome to our project\n\n");

    get_accounts();
    get_sheet();
    get_groups();

    int choice;
    int role;

    while (1) {
        system("cls");

        get_logo(t_width, t_height)
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
            } else {
                // system("cls");
                // printf("You are currently logged in as %s.\n\n", email);

                // Print the menu according to the role
                if (role == 0) {
                    student_loop(student_ptr(email));
                } else if (role == 1) {
                    printf("1. Log out\n2. Check spreadsheet\n3. Edit spreadsheet\n4. Make group\n5. Check groups\n6. Check accounts\n7. Delete account\n\n>> ");

                    choice = get_input(1, 7);
                } else {
                    printf("1. Log out\n2. Check pending requests\n\n>> ");

                    choice = get_input(1, 2);
                }
            }
        }
    }

    return 0;
}

int main() {
    return start();
}
