#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "structs.h"
#include "constants.h"
#include "load.h"
#include "reserve.h"
#include "check.h"
#include "view.h"
#include "update.h"

int login()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    char username[30];
    char password[30];

    int user_count = 0;
    User *users = load_users(&user_count);

    printf("\n\t\t\t");
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
    printf("-- Login Process --");
    printf("\n\nEnter the username :");
    SetConsoleTextAttribute(hConsole, 7);
    printf("  ");
    if (scanf("%29s", username) != 1)
    {
        clear_input_buffer();
        free(users);
        return 0;
    }
    clear_input_buffer();

    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
    printf("Enter the password :");
    SetConsoleTextAttribute(hConsole, 7);
    printf("  ");
    if (scanf("%29s", password) != 1)
    {
        clear_input_buffer();
        free(users);
        return 0;
    }
    clear_input_buffer();

    int authenticated = 0;
    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].pass, password) == 0)
        {
            authenticated = 1;
            break;
        }
    }

    free(users);
    return authenticated;
}

void show_main_menu()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
    printf("\n\n-- Main Menu --\n\n");
    SetConsoleTextAttribute(hConsole, 7);

    do
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
        printf("1.  Reserve a Room\n");
        printf("2.  Check-in\n");
        printf("3.  Cancel Reservation\n");
        printf("4.  Check-out\n");
        printf("5.  Check Room Availability\n");
        printf("6.  View Customer Details\n");
        printf("7.  Edit Reservation Details\n");
        printf("8.  Query(search)\n");
        printf("9.  Reservation Report\n");
        printf("10. Exit\n");
        SetConsoleTextAttribute(hConsole, 7);

        printf("Option: ");
        int choice;
        if (get_choice(&choice, 1, 10))
        {
            switch (choice)
            {
            case 1:
                reserve_room();
                break;
            case 2:
                checkin();
                break;
            case 3:
                start_cancel_reservation();
                break;
            case 4:
                checkout();
                break;
            case 5:
                view_room();
                break;
            case 6:
                view_customer_details();
                break;
            case 7:
                edit_res();
                break;
            case 8:
                search();
                break;
            case 9:
                report();
                break;
            case 10:
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
                printf("\n\tExiting program!\n");
                SetConsoleTextAttribute(hConsole, 7);
                exit(0);

            default:
                printf("Internal error, exiting...\n");
                return;
            }
        }
        else
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
            printf("\n\tPlease enter a valid option\n");
            SetConsoleTextAttribute(hConsole, 7);
        }
    } while (1);
}
