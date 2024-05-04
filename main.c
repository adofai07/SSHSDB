#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <string.h>
#include <assert.h>

#include "getinput.h"
#include "makeaccount.h"
#include "u8string.h"
#include "getsheet.h"
#include "typing.h"

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

        } else if (choice == 3) {
            int period;

            printf("What period do you want to change? (1 or 2): ");
            scanf("%d", &period);


        } else if (choice == 4) {
            char group_name[30];

            printf("Enter a name for the group: ");
            scanf("%s", group_name);
        }
    }

    return 0;
}

int main() {
    // utf-8 encoding
    system("chcp 65001");

    printf("Welcome to our project\n\n");

    get_accounts();
    get_sheet();

    int choice;
    int role;

    while (1) {
        system("cls");

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

                default:
                    printf("Account created successfully.\n\n");
            }

            printf("Press ENTER to return to menu. ");
            getchar(); while (getchar() != '\n');

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