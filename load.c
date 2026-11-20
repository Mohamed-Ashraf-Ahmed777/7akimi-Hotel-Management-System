#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "constants.h"

User *load_users(int *user_count)
{
    *user_count = 0;
    FILE *file = fopen(USERS_FILE, "r");
    if (!file)
    {
        printf("Can't open users file\n");
        return NULL;
    }

    User users[MAX_USERS];
    int count = 0;

    while (count < MAX_USERS && fscanf(file, "%29s %29s", users[count].username, users[count].pass) == 2)
    {
        count++;
    }
    fclose(file);

    *user_count = count;
    if (count == 0) return NULL;

    User *new_users = (User *)malloc(sizeof(User) * count);
    if (!new_users)
    {
        printf("Memory allocation failed\n");
        *user_count = 0;
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        new_users[i] = users[i];
    }

    return new_users;
}

Room *load_rooms(int *room_count)
{
    *room_count = 0;
    FILE *file = fopen(ROOMS_FILE, "r");
    if (!file)
    {
        printf("Can't open rooms file\n");
        return NULL;
    }

    Room rooms[MAX_ROOMS];
    int count = 0;

    while (count < MAX_ROOMS &&
           fscanf(file, "%d %29s %29s %d",
                  &rooms[count].number,
                  rooms[count].roomstatus,
                  rooms[count].roomcategory,
                  &rooms[count].price) == 4)
    {
        count++;
    }
    fclose(file);

    *room_count = count;
    if (count == 0) return NULL;

    Room *new_rooms = (Room *)malloc(sizeof(Room) * count);
    if (!new_rooms)
    {
        printf("Memory allocation failed\n");
        *room_count = 0;
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        new_rooms[i] = rooms[i];
    }

    return new_rooms;
}

Reservation *load_res(int *res_count)
{
    *res_count = 0;
    FILE *file2 = fopen(RESERVATIONS_FILE, "r");
    if (!file2)
    {
        printf("Can't open reservations file\n");
        return NULL;
    }

    Reservation reservations[MAX_RESERVATIONS];
    int count = 0;
    char line[512];

    while (count < MAX_RESERVATIONS && fgets(line, sizeof(line), file2))
    {
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) == 0) continue;

        Reservation temp;
        memset(&temp, 0, sizeof(Reservation));
        strcpy(temp.category, "Unknown");

        int fields = sscanf(line,
                            "%d,%d,%14[^,],%49[^,],%14[^,],%d,%d-%d-%d,%199[^,],%14[^,],%19[^\n]",
                            &temp.id,
                            &temp.roomnumber,
                            temp.status,
                            temp.name,
                            temp.nationalid,
                            &temp.nights,
                            &temp.check_in_date.day,
                            &temp.check_in_date.month,
                            &temp.check_in_date.year,
                            temp.email,
                            temp.mobile,
                            temp.category);

        if (fields == 11 || fields == 12)
        {
            reservations[count++] = temp;
        }
    }

    fclose(file2);

    *res_count = count;
    if (count == 0) return NULL;

    Reservation *res = (Reservation *)malloc(sizeof(Reservation) * count);
    if (!res)
    {
        printf("Memory allocation failed\n");
        *res_count = 0;
        return NULL;
    }

    for (int i = 0; i < count; i++)
    {
        res[i] = reservations[i];
    }

    return res;
}
