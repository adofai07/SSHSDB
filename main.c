#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <string.h>
#include <assert.h>

#include "getinput.h"
#include "makeaccount.h"

int main() {
    printf("Welcome to our project\n\n");

    get_accounts();

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
            char password[50];
            char confirm_password[50];

            printf("Enter email: ");
            scanf("%s", email);
            printf("Enter password: ");
            scanf("%s", password);
            printf("Confirm password: ");
            scanf("%s", confirm_password);
            printf("Enter desired role (0: Student, 1: Admin, 2: Teacher): ");
            scanf("%d", &role);

            // Make account
            int is_valid = make_account(email, password, confirm_password, role);

            // In case make_account() failed, print an error message
            if (is_valid == 1) {
                printf("Your didn't confirm your password correctly.\n\n");
            } else if (is_valid == 2) {
                printf("Your email is invalid.\n\n");
            } else if (is_valid == 3) {
                printf("There already exists an account with the same email.\n\n");
            } else {
                assert(is_valid == 0);
                printf("Account created successfully.\n\n");
            }
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
                system("cls");
                printf("You are currently logged in as %s.\n\n", email);
                // Print the menu according to the role
                if (role == 0) {
                    while (1) {
                        printf("1. Log out\n2. Check spreadsheet\n3. Edit spreadsheet\n4. Make group\n5. Check groups\n\n>> ");

                        choice = get_input(1, 5);
                    }
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