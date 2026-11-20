#ifndef VALIDATE_H
#define VALIDATE_H
#include "structs.h"

int validateName(char* name);
int validateRoomNum(int roomNum);
int validateReserveId(int reservationId);
int validateReserveStatus(char* status);
int validateDate(Date date);
int validateMobile(char* mobile);
int validateEmail(char* email);
int validateRoomStatus(char* status);
int validateRoomCategory(char* category);
int validateNationalId(char* nationalId);

#endif
