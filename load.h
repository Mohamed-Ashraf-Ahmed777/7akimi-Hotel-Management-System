#ifndef LOAD_H
#define LOAD_H
#include "structs.h"

User *load_users(int *user_count);
Room *load_rooms(int *room_count);
Reservation *load_res(int *res_count);

#endif