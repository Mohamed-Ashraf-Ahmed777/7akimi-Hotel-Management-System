#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_RESERVATIONS 100
#define MAX_ROOMS 100
#define MAX_USERS 100
#define ROOMS_FILE "./Room.txt"
#define RESERVATIONS_FILE "./reservations.txt"
#define USERS_FILE "./users.txt"

void clear_input_buffer();
int get_choice(int *choice, int a, int b);

#endif
