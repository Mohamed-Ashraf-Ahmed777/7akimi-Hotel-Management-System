#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "structs.h"
#include "validate.h"
#include "constants.h"
#include "load.h"
#include "update.h"
#include "reserve.h"
#include "view.h"
#include "menu.h"

int main(void)
{
    srand((unsigned int)time(NULL));
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    printf("\t\t     -----------------------------\n\t\t");
    printf("    |");
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
    printf("   Hotel Management System ");
    SetConsoleTextAttribute(hConsole, 7);
    printf("  |");
    printf("\n\t\t     -----------------------------\n\t\t\t");

    int choice;
    do
    {
        printf("\n[1] Login\n[2] Exit\n");
        printf("Option: ");

        if (get_choice(&choice, 1, 2) == 1)
        {
            switch (choice)
            {
            case 1:
                while (1)
                {
                    if (login())
                    {
                        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
                        printf("\n\t\t        -- Successful Login --");
                        SetConsoleTextAttribute(hConsole, 7);

                        show_main_menu();
                        break;
                    }
                    else
                    {
                        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
                        printf("\n\tLogin failed. Please try again.\n\n");
                        SetConsoleTextAttribute(hConsole, 7);
                    }
                }
                break;

            case 2:
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
                printf("\n\tExiting program..\n");
                SetConsoleTextAttribute(hConsole, 7);
                return 0;

            default:
                printf("Internal error, exiting...\n");
                return 1;
            }
        }
        else
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
            printf("\n\tPlease enter a valid option!\n");
            SetConsoleTextAttribute(hConsole, 7);
        }
    } while (choice == -1);

    return 0;
}
