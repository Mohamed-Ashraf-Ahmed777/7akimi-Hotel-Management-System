#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
#include "structs.h"
#include "validate.h"
#include "constants.h"
#include "load.h"
#include "reserve.h"
#include "update.h"

int generate_reserve_id()
{
    return (rand() % 900000) + 100000;
}

static void to_lower_copy(const char *src, char *dst, size_t dst_size)
{
    size_t i = 0;
    for (; src[i] != '\0' && i + 1 < dst_size; i++)
    {
        dst[i] = (char)tolower((unsigned char)src[i]);
    }
    dst[i] = '\0';
}

void start_cancel_reservation()
{
    char reservationID[16];
    char roomNumber[16];
    int choice;

    do
    {
        printf("Cancel Reservation:\n");
        printf("[1] By Reservation Id \n");
        printf("[2] By Room number \n");
        printf("[3] Back to Main Menu\n");
        printf("--------------------------------\n--------------------------------\n");
        printf("Option: ");

        if (get_choice(&choice, 1, 3))
        {
            switch (choice)
            {
            case 1:
                printf("Enter Reservation ID: ");
                if (scanf("%15s", reservationID) == 1)
                {
                    clear_input_buffer();
                    if (validateReserveId(atoi(reservationID)) == 1)
                    {
                        cancel_reservation(reservationID, NULL);
                    }
                    else
                    {
                        printf("Please enter a valid reservation ID!\n");
                    }
                }
                break;

            case 2:
                printf("Enter Room number to cancel: ");
                if (scanf("%15s", roomNumber) == 1)
                {
                    clear_input_buffer();
                    if (validateRoomNum(atoi(roomNumber)) == 1)
                    {
                        cancel_reservation(NULL, roomNumber);
                    }
                    else
                    {
                        printf("Please enter a valid room number!\n");
                    }
                }
                break;

            case 3:
                return;
            }
        }
        else
        {
            printf("Please enter a valid option!\n");
        }
    } while (1);
}

void cancel_reservation(char *cancelID, char *cancelRoom)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int res_count = 0, room_count = 0;
    Reservation *res = load_res(&res_count);
    Room *rooms = load_rooms(&room_count);

    int target_id = (cancelID != NULL) ? atoi(cancelID) : -1;
    int target_room = (cancelRoom != NULL) ? atoi(cancelRoom) : -1;
    int found_index = -1;

    for (int i = 0; i < res_count; i++)
    {
        if ((target_id != -1 && res[i].id == target_id) ||
            (target_room != -1 && res[i].roomnumber == target_room))
        {
            found_index = i;
            break;
        }
    }

    if (found_index == -1)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
        if (cancelID != NULL)
            printf("Reservation not found for ID: %s\n", cancelID);
        else
            printf("Reservation not found for Room number: %s\n", cancelRoom);
        SetConsoleTextAttribute(hConsole, 7);
        free(res);
        free(rooms);
        return;
    }

    if (strcmp(res[found_index].status, "confirmed") == 0)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
        printf("Cannot cancel confirmed reservation ID: %d.\n", res[found_index].id);
        SetConsoleTextAttribute(hConsole, 7);
        free(res);
        free(rooms);
        return;
    }

    int freed_room_num = res[found_index].roomnumber;

    for (int j = 0; j < room_count; j++)
    {
        if (rooms[j].number == freed_room_num)
        {
            strcpy(rooms[j].roomstatus, "Available");
            break;
        }
    }

    int new_count = 0;
    Reservation *updated_res = NULL;
    if (res_count - 1 > 0)
    {
        updated_res = (Reservation *)malloc(sizeof(Reservation) * (res_count - 1));
        for (int i = 0; i < res_count; i++)
        {
            if (i != found_index)
            {
                updated_res[new_count++] = res[i];
            }
        }
    }

    update_res(updated_res, new_count);
    update_rooms(rooms, room_count);

    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    printf("Reservation canceled successfully and Room %d is now Available.\n", freed_room_num);
    SetConsoleTextAttribute(hConsole, 7);

    free(updated_res);
    free(res);
    free(rooms);
}

void reserve_room()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    Reservation new_res;
    memset(&new_res, 0, sizeof(Reservation));

    char buffer[256];

    do
    {
        printf("Enter the customer name: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) continue;
        buffer[strcspn(buffer, "\r\n")] = '\0';

        if (validateName(buffer) == 1)
        {
            strcpy(new_res.name, buffer);
            break;
        }
        printf("Please enter a valid name!\n");
    } while (1);

    do
    {
        printf("Enter the customer national Id (14 digits): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) continue;
        buffer[strcspn(buffer, "\r\n")] = '\0';

        if (validateNationalId(buffer) == 1)
        {
            strcpy(new_res.nationalid, buffer);
            break;
        }
        printf("Please enter a valid 14-digit national ID!\n");
    } while (1);

    do
    {
        printf("Enter the customer email: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) continue;
        buffer[strcspn(buffer, "\r\n")] = '\0';

        if (validateEmail(buffer) == 1)
        {
            strcpy(new_res.email, buffer);
            break;
        }
        printf("Please enter a valid E-mail!\n");
    } while (1);

    do
    {
        printf("Enter the customer mobile number (010/011/012/015): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) continue;
        buffer[strcspn(buffer, "\r\n")] = '\0';

        if (validateMobile(buffer) == 1)
        {
            strcpy(new_res.mobile, buffer);
            break;
        }
        printf("Please enter a valid 11-digit mobile number!\n");
    } while (1);

    Date res_date;
    do
    {
        printf("Enter the check-in date (day-month-year): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) continue;
        buffer[strcspn(buffer, "\r\n")] = '\0';

        if (sscanf(buffer, "%d-%d-%d", &res_date.day, &res_date.month, &res_date.year) == 3 &&
            validateDate(res_date) == 1)
        {
            new_res.check_in_date = res_date;
            break;
        }
        printf("Please enter a valid date (DD-MM-YYYY)!\n");
    } while (1);

    do
    {
        printf("Enter the number of nights: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) continue;
        buffer[strcspn(buffer, "\r\n")] = '\0';

        int n = atoi(buffer);
        if (n > 0)
        {
            new_res.nights = n;
            break;
        }
        printf("Please enter a valid number of nights (> 0)!\n");
    } while (1);

    do
    {
        printf("Enter the room category (SeaView - LakeView - GardenView): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) continue;
        buffer[strcspn(buffer, "\r\n")] = '\0';

        if (validateRoomCategory(buffer) == 1)
        {
            strcpy(new_res.category, buffer);
            break;
        }
        printf("Please enter a valid room category!\n");
    } while (1);

    int room_count = 0;
    Room *rooms = load_rooms(&room_count);
    char target_cat[30];
    to_lower_copy(new_res.category, target_cat, sizeof(target_cat));

    int roomfound = 0;
    for (int i = 0; i < room_count; i++)
    {
        char room_cat_lower[30];
        to_lower_copy(rooms[i].roomcategory, room_cat_lower, sizeof(room_cat_lower));

        if (strcmp(room_cat_lower, target_cat) == 0 && strcmp(rooms[i].roomstatus, "Available") == 0)
        {
            strcpy(rooms[i].roomstatus, "Reserved");
            new_res.roomnumber = rooms[i].number;
            strcpy(new_res.category, rooms[i].roomcategory);
            roomfound = 1;
            break;
        }
    }

    if (!roomfound)
    {
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
        printf("No available rooms in category '%s'. Reservation was not created.\n", new_res.category);
        SetConsoleTextAttribute(hConsole, 7);
        free(rooms);
        return;
    }

    new_res.id = generate_reserve_id();
    strcpy(new_res.status, "unconfirmed");

    update_rooms(rooms, room_count);

    FILE *file1 = fopen(RESERVATIONS_FILE, "a");
    if (!file1)
    {
        printf("Can't open reservations file for writing\n");
        free(rooms);
        return;
    }

    fprintf(file1, "%d,%d,%s,%s,%s,%d,%d-%d-%d,%s,%s,%s\n",
            new_res.id,
            new_res.roomnumber,
            new_res.status,
            new_res.name,
            new_res.nationalid,
            new_res.nights,
            new_res.check_in_date.day,
            new_res.check_in_date.month,
            new_res.check_in_date.year,
            new_res.email,
            new_res.mobile,
            new_res.category);
    fclose(file1);

    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    printf("\n\t\t\tSuccessful Reservation\n");
    SetConsoleTextAttribute(hConsole, 7);
    printf("Your reservation ID is : %d\n", new_res.id);
    printf("Your room number is    : %d\n", new_res.roomnumber);

    free(rooms);
}
