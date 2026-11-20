#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
    int day, month, year;
} Date;

typedef struct
{
    char username[30];
    char pass[30];
} User;

typedef struct
{
    int id;
    int roomnumber;
    char status[15];
    char name[50];
    char nationalid[15];
    int nights;
    Date check_in_date;
    char email[200];
    char mobile[15];
    char category[20];
} Reservation;

typedef struct
{
    int number, price;
    char roomstatus[30];
    char roomcategory[30];
} Room;

#endif
