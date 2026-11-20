#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "structs.h"
#include "validate.h"
#include "constants.h"
#include "load.h"

static void to_lower_copy(const char *src, char *dst, size_t dst_size)
{
    size_t i = 0;
    for (; src[i] != '\0' && i + 1 < dst_size; i++)
    {
        dst[i] = (char)tolower((unsigned char)src[i]);
    }
    dst[i] = '\0';
}

void report()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char line[64];
    Date res_date;

    do
    {
        printf("Enter check-in date (day-month-year): ");
        if (!fgets(line, sizeof(line), stdin)) continue;
        line[strcspn(line, "\r\n")] = '\0';

        if (sscanf(line, "%d-%d-%d", &res_date.day, &res_date.month, &res_date.year) == 3 &&
            validateDate(res_date) == 1)
        {
            break;
        }
        printf("Please enter a valid date (DD-MM-YYYY)!\n");
    } while (1);

    int res_count = 0, found = 0;
    Reservation *res = load_res(&res_count);

    for (int i = 0; i < res_count; i++)
    {
        if (res[i].check_in_date.day == res_date.day &&
            res[i].check_in_date.month == res_date.month &&
            res[i].check_in_date.year == res_date.year)
        {
            found = 1;
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
            printf("\nReservation ID [%d] (Room: %d | Status: %s):\n", res[i].id, res[i].roomnumber, res[i].status);
            printf("   ----------------------- \n");
            printf("Customer Name: %s\n", res[i].name);
            printf("Mobile Number: %s\n", res[i].mobile);
            printf("E-mail: %s\n", res[i].email);
            printf("   ----------------------- \n");
            SetConsoleTextAttribute(hConsole, 7);
        }
    }

    if (!found)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
        printf("No customer details are recorded with this date\n\n");
        SetConsoleTextAttribute(hConsole, 7);
    }

    free(res);
}

void view_room()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int room_count = 0;
    Room *rooms = load_rooms(&room_count);

    printf("\n   ----------------------- \n");
    for (int i = 0; i < room_count; i++)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
        printf("Room [%d]:\n", i + 1);
        SetConsoleTextAttribute(hConsole, 7);
        printf("   ----------------------- \n");
        printf("Number: %d\n", rooms[i].number);
        printf("Status: %s\n", rooms[i].roomstatus);
        printf("Category: %s\n", rooms[i].roomcategory);
        printf("Price per night: %d EGP\n", rooms[i].price);
        printf("   ----------------------- \n");
    }

    free(rooms);
}

void view_customer_details()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int choice;

    do
    {
        printf("\nView by:\n");
        printf("[1] Reservation ID\n");
        printf("[2] Room Number\n");
        printf("[3] Back to Main Menu\n");
        printf("Option: ");

        if (!get_choice(&choice, 1, 3))
        {
            printf("Please enter a valid option!\n");
            continue;
        }

        if (choice == 3) return;

        char input_buf[32];
        int res_count = 0, found = 0;
        Reservation *res = load_res(&res_count);

        if (choice == 1)
        {
            printf("Enter Reservation ID: ");
            if (scanf("%31s", input_buf) != 1)
            {
                clear_input_buffer();
                free(res);
                continue;
            }
            clear_input_buffer();

            int res_uid = atoi(input_buf);
            if (validateReserveId(res_uid) != 1)
            {
                printf("Invalid Reservation ID, please try again!\n");
                free(res);
                continue;
            }

            for (int i = 0; i < res_count; i++)
            {
                if (res[i].id == res_uid)
                {
                    found = 1;
                    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED);
                    printf("\n   ----------------------- \n");
                    SetConsoleTextAttribute(hConsole, 7);
                    printf("Name        : %s\n", res[i].name);
                    printf("National ID : %s\n", res[i].nationalid);
                    printf("Room        : %d (%s)\n", res[i].roomnumber, res[i].status);
                    printf("Nights      : %d\n", res[i].nights);
                    printf("Mobile      : %s\n", res[i].mobile);
                    printf("E-mail      : %s\n", res[i].email);
                    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED);
                    printf("   ----------------------- \n");
                    SetConsoleTextAttribute(hConsole, 7);
                    break;
                }
            }

            if (!found)
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
                printf("No reservation was found with ID: %d\n", res_uid);
                SetConsoleTextAttribute(hConsole, 7);
            }
        }
        else if (choice == 2)
        {
            printf("Enter Room number: ");
            if (scanf("%31s", input_buf) != 1)
            {
                clear_input_buffer();
                free(res);
                continue;
            }
            clear_input_buffer();

            int room_num = atoi(input_buf);
            if (validateRoomNum(room_num) != 1)
            {
                printf("Invalid Room number, please try again!\n");
                free(res);
                continue;
            }

            for (int i = 0; i < res_count; i++)
            {
                if (res[i].roomnumber == room_num)
                {
                    found = 1;
                    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED);
                    printf("\n   ----------------------- \n");
                    SetConsoleTextAttribute(hConsole, 7);
                    printf("Name        : %s\n", res[i].name);
                    printf("Reservation : %d (%s)\n", res[i].id, res[i].status);
                    printf("Mobile      : %s\n", res[i].mobile);
                    printf("E-mail      : %s\n", res[i].email);
                    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED);
                    printf("   ----------------------- \n");
                    SetConsoleTextAttribute(hConsole, 7);
                    break;
                }
            }

            if (!found)
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
                printf("No reservation was found for room: %d\n", room_num);
                SetConsoleTextAttribute(hConsole, 7);
            }
        }

        free(res);
    } while (1);
}

void get_reservation_by_customer_name(char *customerName)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    FILE *reservationFile = fopen(RESERVATIONS_FILE, "r");
    if (!reservationFile)
    {
        printf("Error opening reservation file.\n");
        return;
    }

    char reservationLine[512];
    int found = 0;

    while (fgets(reservationLine, sizeof(reservationLine), reservationFile))
    {
        reservationLine[strcspn(reservationLine, "\r\n")] = '\0';
        if (strlen(reservationLine) == 0) continue;

        char reservationID[20], roomNumber[20], reservationStatus[20];
        char customer[100], nationalID[20], numNights[10], checkinDate[20], email[100], mobile[20], category[30];
        strcpy(category, "N/A");

        int parsed = sscanf(reservationLine, "%19[^,],%19[^,],%19[^,],%99[^,],%19[^,],%9[^,],%19[^,],%99[^,],%19[^,],%29[^\n]",
                            reservationID, roomNumber, reservationStatus, customer, nationalID, numNights,
                            checkinDate, email, mobile, category);

        if (parsed >= 9 && strcmp(customer, customerName) == 0)
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
            printf("\n   ----------------------- \n");
            SetConsoleTextAttribute(hConsole, 7);
            printf("Reservation ID: %s\n", reservationID);
            printf("Room Number   : %s\n", roomNumber);
            printf("Category      : %s\n", category);
            printf("Status        : %s\n", reservationStatus);
            printf("Customer Name : %s\n", customer);
            printf("National ID   : %s\n", nationalID);
            printf("No. of Nights : %s\n", numNights);
            printf("Check-in Date : %s\n", checkinDate);
            printf("Email         : %s\n", email);
            printf("Mobile        : %s\n", mobile);
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
            printf("   ----------------------- \n");
            SetConsoleTextAttribute(hConsole, 7);
            found = 1;
        }
    }

    if (!found)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
        printf("\nNo reservation found for customer: %s\n", customerName);
        SetConsoleTextAttribute(hConsole, 7);
    }

    fclose(reservationFile);
}

void get_room_details_by_room_number(char *roomNumber)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    FILE *roomFile = fopen(ROOMS_FILE, "r");
    if (!roomFile)
    {
        printf("Error opening room file.\n");
        return;
    }

    char roomLine[250];
    int found = 0;

    while (fgets(roomLine, sizeof(roomLine), roomFile))
    {
        char roomNum[20], roomStatus[20], roomCategory[50], price[10];
        if (sscanf(roomLine, "%19s %19s %49s %9s", roomNum, roomStatus, roomCategory, price) == 4)
        {
            if (strcmp(roomNum, roomNumber) == 0)
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
                printf("\n   ----------------------- \n");
                SetConsoleTextAttribute(hConsole, 7);
                printf("Room Number    : %s\n", roomNum);
                printf("Room Status    : %s\n", roomStatus);
                printf("Room Category  : %s\n", roomCategory);
                printf("Price per Night: %s EGP\n", price);
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
                printf("   ----------------------- \n");
                SetConsoleTextAttribute(hConsole, 7);
                found = 1;
            }
        }
    }

    if (!found)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
        printf("\nRoom number %s not found.\n", roomNumber);
        SetConsoleTextAttribute(hConsole, 7);
    }

    fclose(roomFile);
}

void get_rooms_by_status(char *status)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    FILE *roomFile = fopen(ROOMS_FILE, "r");
    if (!roomFile)
    {
        printf("Error opening room file.\n");
        return;
    }

    char target_lower[32];
    to_lower_copy(status, target_lower, sizeof(target_lower));

    char roomLine[250];
    int found = 0;

    while (fgets(roomLine, sizeof(roomLine), roomFile))
    {
        char roomNum[20], roomStatus[20], roomCategory[50], price[10];
        if (sscanf(roomLine, "%19s %19s %49s %9s", roomNum, roomStatus, roomCategory, price) == 4)
        {
            char status_lower[32];
            to_lower_copy(roomStatus, status_lower, sizeof(status_lower));

            if (strcmp(status_lower, target_lower) == 0)
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
                printf("\n   ----------------------- \n");
                SetConsoleTextAttribute(hConsole, 7);
                printf("Room Number    : %s\n", roomNum);
                printf("Room Status    : %s\n", roomStatus);
                printf("Room Category  : %s\n", roomCategory);
                printf("Price per Night: %s EGP\n", price);
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED);
                printf("   ----------------------- \n");
                SetConsoleTextAttribute(hConsole, 7);
                found = 1;
            }
        }
    }

    if (!found)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
        printf("\nNo rooms found with status: %s\n", status);
        SetConsoleTextAttribute(hConsole, 7);
    }

    fclose(roomFile);
}

void search()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char customerName[50], roomNumber[16], status[16];
    int choice;

    do
    {
        printf("\nChoose an option:\n");
        printf("[1] Lookup reservation by customer name\n");
        printf("[2] Lookup room by room number\n");
        printf("[3] View rooms by status (Available / Reserved)\n");
        printf("[4] Back to Main Menu\n");
        printf("Option: ");

        if (get_choice(&choice, 1, 4) == 1)
        {
            switch (choice)
            {
            case 1:
                printf("Enter customer name: ");
                if (fgets(customerName, sizeof(customerName), stdin))
                {
                    customerName[strcspn(customerName, "\r\n")] = '\0';
                    get_reservation_by_customer_name(customerName);
                }
                break;

            case 2:
                printf("Enter room number: ");
                if (fgets(roomNumber, sizeof(roomNumber), stdin))
                {
                    roomNumber[strcspn(roomNumber, "\r\n")] = '\0';
                    get_room_details_by_room_number(roomNumber);
                }
                break;

            case 3:
                printf("Enter room status (Available / Reserved): ");
                if (fgets(status, sizeof(status), stdin))
                {
                    status[strcspn(status, "\r\n")] = '\0';
                    get_rooms_by_status(status);
                }
                break;

            case 4:
                return;
            }
        }
        else
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
            printf("Invalid choice.\n");
            SetConsoleTextAttribute(hConsole, 7);
        }
    } while (1);
}
