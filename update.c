#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "structs.h"
#include "constants.h"
#include "validate.h"
#include "load.h"

static void to_lower_str(const char *src, char *dst, size_t dst_size)
{
    size_t i = 0;
    for (; src[i] != '\0' && i + 1 < dst_size; i++)
    {
        dst[i] = (char)tolower((unsigned char)src[i]);
    }
    dst[i] = '\0';
}

int save_changes()
{
    char line[32];
    printf("Do you want to save changes? (Y/N): ");
    if (!fgets(line, sizeof(line), stdin)) return 0;

    if (line[0] == 'y' || line[0] == 'Y')
    {
        return 1;
    }
    printf("No changes will be saved!\n");
    return 0;
}

void update_rooms(Room *rooms, int room_count)
{
    FILE *roomFile = fopen(ROOMS_FILE, "w");
    if (!roomFile)
    {
        printf("Error opening rooms file for writing.\n");
        return;
    }
    for (int i = 0; i < room_count; i++)
    {
        fprintf(roomFile, "%d %s %s %d\n",
                rooms[i].number,
                rooms[i].roomstatus,
                rooms[i].roomcategory,
                rooms[i].price);
    }
    fclose(roomFile);
}

void update_res(Reservation *reservations, int res_count)
{
    FILE *reservationFile = fopen(RESERVATIONS_FILE, "w");
    if (!reservationFile)
    {
        printf("Error opening reservations file for writing.\n");
        return;
    }
    for (int i = 0; i < res_count; i++)
    {
        fprintf(reservationFile, "%d,%d,%s,%s,%s,%d,%d-%d-%d,%s,%s,%s\n",
                reservations[i].id,
                reservations[i].roomnumber,
                reservations[i].status,
                reservations[i].name,
                reservations[i].nationalid,
                reservations[i].nights,
                reservations[i].check_in_date.day,
                reservations[i].check_in_date.month,
                reservations[i].check_in_date.year,
                reservations[i].email,
                reservations[i].mobile,
                reservations[i].category);
    }
    fclose(reservationFile);
}

void edit_res()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int choice;

    do
    {
        printf("\nChoose Edit by:\n");
        printf("[1] Reservation ID\n");
        printf("[2] Room number\n");
        printf("[3] Back to Main Menu\n");
        printf("Option: ");

        if (!get_choice(&choice, 1, 3))
        {
            printf("Please enter a valid option!\n");
            continue;
        }

        if (choice == 3)
        {
            return;
        }

        int res_uid = -1, room_unum = -1;
        char input_buf[32];

        if (choice == 1)
        {
            printf("Enter Reservation ID: ");
            if (scanf("%31s", input_buf) != 1)
            {
                clear_input_buffer();
                printf("Invalid Reservation ID.\n");
                continue;
            }
            clear_input_buffer();
            res_uid = atoi(input_buf);
            if (validateReserveId(res_uid) != 1)
            {
                printf("Invalid Reservation ID.\n");
                continue;
            }
        }
        else if (choice == 2)
        {
            printf("Enter Room number: ");
            if (scanf("%31s", input_buf) != 1)
            {
                clear_input_buffer();
                printf("Invalid Room number.\n");
                continue;
            }
            clear_input_buffer();
            room_unum = atoi(input_buf);
            if (validateRoomNum(room_unum) != 1)
            {
                printf("Invalid Room number.\n");
                continue;
            }
        }

        int res_count = 0, room_count = 0, changed = 0;
        Reservation *res = load_res(&res_count);
        Room *rooms = load_rooms(&room_count);

        int target_idx = -1;
        for (int i = 0; i < res_count; i++)
        {
            if ((res_uid != -1 && res[i].id == res_uid) ||
                (room_unum != -1 && res[i].roomnumber == room_unum))
            {
                target_idx = i;
                break;
            }
        }

        if (target_idx == -1)
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
            printf("Reservation not found.\n");
            SetConsoleTextAttribute(hConsole, 7);
            free(res);
            free(rooms);
            continue;
        }

        char buffer[256];
        printf("\nEditing Reservation ID: %d (Room: %d)\n", res[target_idx].id, res[target_idx].roomnumber);

        printf("Current customer name: %s\n", res[target_idx].name);
        do
        {
            printf("Enter new customer name (Enter to skip): ");
            if (!fgets(buffer, sizeof(buffer), stdin)) continue;
            buffer[strcspn(buffer, "\r\n")] = '\0';
            if (strlen(buffer) == 0) break;
            if (validateName(buffer) == 1)
            {
                strcpy(res[target_idx].name, buffer);
                changed = 1;
                break;
            }
            printf("Invalid name. Try again.\n");
        } while (1);

        printf("Current mobile number: %s\n", res[target_idx].mobile);
        do
        {
            printf("Enter new mobile number (Enter to skip): ");
            if (!fgets(buffer, sizeof(buffer), stdin)) continue;
            buffer[strcspn(buffer, "\r\n")] = '\0';
            if (strlen(buffer) == 0) break;
            if (validateMobile(buffer) == 1)
            {
                strcpy(res[target_idx].mobile, buffer);
                changed = 1;
                break;
            }
            printf("Invalid mobile number. Try again.\n");
        } while (1);

        printf("Current e-mail: %s\n", res[target_idx].email);
        do
        {
            printf("Enter new email address (Enter to skip): ");
            if (!fgets(buffer, sizeof(buffer), stdin)) continue;
            buffer[strcspn(buffer, "\r\n")] = '\0';
            if (strlen(buffer) == 0) break;
            if (validateEmail(buffer) == 1)
            {
                strcpy(res[target_idx].email, buffer);
                changed = 1;
                break;
            }
            printf("Invalid email. Try again.\n");
        } while (1);

        printf("Current check-in date: %02d-%02d-%04d\n",
               res[target_idx].check_in_date.day,
               res[target_idx].check_in_date.month,
               res[target_idx].check_in_date.year);
        Date res_date;
        do
        {
            printf("Enter new check-in date (DD-MM-YYYY, Enter to skip): ");
            if (!fgets(buffer, sizeof(buffer), stdin)) continue;
            buffer[strcspn(buffer, "\r\n")] = '\0';
            if (strlen(buffer) == 0)
            {
                printf("No changes made to the check-in date.\n");
                break;
            }
            if (sscanf(buffer, "%d-%d-%d", &res_date.day, &res_date.month, &res_date.year) == 3 &&
                validateDate(res_date) == 1)
            {
                res[target_idx].check_in_date = res_date;
                changed = 1;
                break;
            }
            printf("Invalid date format or value (use DD-MM-YYYY).\n");
        } while (1);

        printf("Current number of nights: %d\n", res[target_idx].nights);
        do
        {
            printf("Enter new number of nights (Enter to skip): ");
            if (!fgets(buffer, sizeof(buffer), stdin)) continue;
            buffer[strcspn(buffer, "\r\n")] = '\0';
            if (strlen(buffer) == 0) break;
            int n = atoi(buffer);
            if (n > 0)
            {
                res[target_idx].nights = n;
                changed = 1;
                break;
            }
            printf("Please enter a positive number of nights.\n");
        } while (1);

        printf("Current reservation room category: %s\n", res[target_idx].category);
        do
        {
            printf("Enter new room category (SeaView/LakeView/GardenView, Enter to skip): ");
            if (!fgets(buffer, sizeof(buffer), stdin)) continue;
            buffer[strcspn(buffer, "\r\n")] = '\0';
            if (strlen(buffer) == 0) break;

            if (validateRoomCategory(buffer) == 1)
            {
                char req_cat_lower[30];
                to_lower_str(buffer, req_cat_lower, sizeof(req_cat_lower));

                int new_room_idx = -1;
                for (int j = 0; j < room_count; j++)
                {
                    char r_cat_lower[30];
                    to_lower_str(rooms[j].roomcategory, r_cat_lower, sizeof(r_cat_lower));
                    if (strcmp(r_cat_lower, req_cat_lower) == 0 && strcmp(rooms[j].roomstatus, "Available") == 0)
                    {
                        new_room_idx = j;
                        break;
                    }
                }

                if (new_room_idx == -1)
                {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
                    printf("No available rooms in category '%s'. Category kept unchanged.\n", buffer);
                    SetConsoleTextAttribute(hConsole, 7);
                    break;
                }

                for (int j = 0; j < room_count; j++)
                {
                    if (rooms[j].number == res[target_idx].roomnumber)
                    {
                        strcpy(rooms[j].roomstatus, "Available");
                        break;
                    }
                }

                strcpy(rooms[new_room_idx].roomstatus, "Reserved");
                res[target_idx].roomnumber = rooms[new_room_idx].number;
                strcpy(res[target_idx].category, rooms[new_room_idx].roomcategory);
                changed = 1;
                printf("Moved to Room %d (%s).\n", rooms[new_room_idx].number, rooms[new_room_idx].roomcategory);
                break;
            }
            printf("Invalid room category.\n");
        } while (1);

        if (changed)
        {
            if (save_changes() == 1)
            {
                update_res(res, res_count);
                update_rooms(rooms, room_count);
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
                printf("Successfully edited reservation!\n");
                SetConsoleTextAttribute(hConsole, 7);
            }
        }
        else
        {
            printf("No modifications were made.\n");
        }

        free(res);
        free(rooms);
    } while (1);
}
