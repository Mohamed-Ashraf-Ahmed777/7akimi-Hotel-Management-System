#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "structs.h"
#include "update.h"
#include "load.h"
#include "validate.h"
#include "constants.h"

static Date get_current_date(void)
{
    time_t t = time(NULL);
    struct tm *tm_ptr = localtime(&t);

    Date currentDate;
    currentDate.day = tm_ptr->tm_mday;
    currentDate.month = tm_ptr->tm_mon + 1;
    currentDate.year = tm_ptr->tm_year + 1900;

    return currentDate;
}

static int can_check_in(Date currentDate, Date otherDate)
{
    if (currentDate.year != otherDate.year)
        return (currentDate.year > otherDate.year) ? 1 : 0;

    if (currentDate.month != otherDate.month)
        return (currentDate.month > otherDate.month) ? 1 : 0;

    return (currentDate.day >= otherDate.day) ? 1 : 0;
}

void checkin()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    char reservationID[16];
    printf("Enter Reservation ID: ");
    if (scanf("%15s", reservationID) != 1)
    {
        clear_input_buffer();
        printf("Invalid Reservation ID.\n");
        return;
    }
    clear_input_buffer();

    int res_uid = atoi(reservationID);
    if (validateReserveId(res_uid) != 1)
    {
        printf("Invalid Reservation ID.\n");
        return;
    }

    int res_count = 0, room_count = 0;
    Reservation *res = load_res(&res_count);
    Room *rooms = load_rooms(&room_count);

    int found_res_idx = -1;
    for (int i = 0; i < res_count; i++)
    {
        if (res[i].id == res_uid)
        {
            found_res_idx = i;
            break;
        }
    }

    if (found_res_idx == -1)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
        printf("\n\tReservation ID %d not found!\n\n", res_uid);
        SetConsoleTextAttribute(hConsole, 7);
        free(res);
        free(rooms);
        return;
    }

    if (strcmp(res[found_res_idx].status, "confirmed") == 0)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
        printf("\n\tReservation already confirmed!\n\n");
        SetConsoleTextAttribute(hConsole, 7);
        free(res);
        free(rooms);
        return;
    }

    Date res_date = res[found_res_idx].check_in_date;
    Date present_date = get_current_date();

    if (!can_check_in(present_date, res_date))
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
        printf("\n\tCurrent date is before check-in date, please come back later!\n\n");
        SetConsoleTextAttribute(hConsole, 7);
        free(res);
        free(rooms);
        return;
    }

    strcpy(res[found_res_idx].status, "confirmed");
    for (int j = 0; j < room_count; j++)
    {
        if (rooms[j].number == res[found_res_idx].roomnumber)
        {
            strcpy(rooms[j].roomstatus, "Reserved");
            break;
        }
    }

    update_res(res, res_count);
    update_rooms(rooms, room_count);

    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    printf("\n\tReservation checked in successfully!\n\n");
    SetConsoleTextAttribute(hConsole, 7);

    free(res);
    free(rooms);
}

void checkout()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    char roomNum[16];
    printf("Enter Room number: ");
    if (scanf("%15s", roomNum) != 1)
    {
        clear_input_buffer();
        printf("Invalid Room number.\n");
        return;
    }
    clear_input_buffer();

    int room_num = atoi(roomNum);
    if (validateRoomNum(room_num) != 1)
    {
        printf("Invalid Room number.\n");
        return;
    }

    int res_count = 0, room_count = 0;
    Reservation *res = load_res(&res_count);
    Room *rooms = load_rooms(&room_count);

    int room_index = -1;
    for (int i = 0; i < room_count; i++)
    {
        if (rooms[i].number == room_num)
        {
            room_index = i;
            break;
        }
    }

    if (room_index == -1)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
        printf("Room %d not found in Room.txt\n", room_num);
        SetConsoleTextAttribute(hConsole, 7);
        free(res);
        free(rooms);
        return;
    }

    int confirmed_res_index = -1;
    for (int k = 0; k < res_count; k++)
    {
        if (res[k].roomnumber == room_num && strcmp(res[k].status, "confirmed") == 0)
        {
            confirmed_res_index = k;
            break;
        }
    }

    if (confirmed_res_index == -1)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
        printf("\n\tNo confirmed reservation found for room %d.\n", room_num);
        SetConsoleTextAttribute(hConsole, 7);
        free(res);
        free(rooms);
        return;
    }

    int total_bill = res[confirmed_res_index].nights * rooms[room_index].price;
    strcpy(rooms[room_index].roomstatus, "Available");

    int new_count = 0;
    Reservation *new_res = NULL;
    if (res_count - 1 > 0)
    {
        new_res = (Reservation *)malloc(sizeof(Reservation) * (res_count - 1));
        for (int k = 0; k < res_count; k++)
        {
            if (k != confirmed_res_index)
            {
                new_res[new_count++] = res[k];
            }
        }
    }

    update_rooms(rooms, room_count);
    update_res(new_res, new_count);

    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    printf("\nTotal bill for room %d (%d nights x %d EGP): %d EGP\n",
           room_num, res[confirmed_res_index].nights, rooms[room_index].price, total_bill);
    printf("Check-out completed, Room %d is now available for reservation!\n\n", room_num);
    SetConsoleTextAttribute(hConsole, 7);

    free(new_res);
    free(res);
    free(rooms);
}
