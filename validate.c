#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"

int validateName(char* name) {
    int len = strlen(name);
    if (len == 0 || len > 49) return 0;
    int has_alpha = 0;
    for (int i = 0; i < len; i++) {
        if (isalpha((unsigned char)name[i])) has_alpha = 1;
        else if (name[i] != ' ') return 0;
    }
    return has_alpha;
}

int validateRoomNum(int roomNum) {
    return (roomNum >= 1000 && roomNum <= 9999) ? 1 : 0;
}

int validateReserveId(int reservationId) {
    return (reservationId >= 100000 && reservationId <= 999999) ? 1 : 0;
}

int validateReserveStatus(char* status) {
    int length = strlen(status);
    if (length >= 9 && length <= 11) {
        char clean[15];
        for (int i = 0; i < length; i++) {
            clean[i] = tolower((unsigned char)status[i]);
        }
        clean[length] = '\0';
        return ((strcmp(clean, "confirmed") == 0) || (strcmp(clean, "unconfirmed") == 0)) ? 1 : 0;
    }
    return 0;
}

int validateRoomStatus(char* status) {
    int length = strlen(status);
    if (length == 8 || length == 9) {
        char clean[15];
        for (int i = 0; i < length; i++) {
            clean[i] = tolower((unsigned char)status[i]);
        }
        clean[length] = '\0';
        return ((strcmp(clean, "available") == 0) || (strcmp(clean, "reserved") == 0)) ? 1 : 0;
    }
    return 0;
}

int validateNationalId(char* nationalId) {
    if (strlen(nationalId) != 14) return 0;
    for (int i = 0; i < 14; i++) {
        if (!isdigit((unsigned char)nationalId[i])) return 0;
    }
    return 1;
}

int validateRoomCategory(char* category) {
    int len = strlen(category);
    if (len == 0 || len >= 20) return 0;
    char clean[20];
    for (int i = 0; i < len; i++) {
        clean[i] = tolower((unsigned char)category[i]);
    }
    clean[len] = '\0';
    return ((strcmp(clean, "seaview") == 0) || (strcmp(clean, "gardenview") == 0) || (strcmp(clean, "lakeview") == 0)) ? 1 : 0;
}

int validateDate(Date date) {
    if (date.year < 2000 || date.year > 2100) return 0;
    if (date.month < 1 || date.month > 12) return 0;

    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int is_leap = ((date.year % 4 == 0 && date.year % 100 != 0) || (date.year % 400 == 0));
    if (is_leap) days_in_month[2] = 29;

    return (date.day >= 1 && date.day <= days_in_month[date.month]) ? 1 : 0;
}

int validateMobile(char* mobile) {
    if (strlen(mobile) != 11) return 0;
    for (int i = 0; i < 11; i++) {
        if (!isdigit((unsigned char)mobile[i])) return 0;
    }
    if (mobile[0] == '0' && mobile[1] == '1' &&
        (mobile[2] == '0' || mobile[2] == '1' || mobile[2] == '2' || mobile[2] == '5')) {
        return 1;
    }
    return 0;
}

int validateEmail(char* email) {
    char* at = strchr(email, '@');
    if (!at || at == email) return 0;

    char* dot = strchr(at, '.');
    if (!dot || dot == at + 1 || *(dot + 1) == '.' || *(dot + 1) == '\0') return 0;

    if (strlen(dot + 1) < 2) return 0;

    for (int i = 0; email[i] != '\0'; i++) {
        if (!isalnum((unsigned char)email[i]) && email[i] != '@' && email[i] != '.' && email[i] != '-' && email[i] != '_') {
            return 0;
        }
    }
    return 1;
}
