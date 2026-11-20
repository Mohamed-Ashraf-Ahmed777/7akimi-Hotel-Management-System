#include <stdio.h>
#include <string.h>

void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_choice(int *choice, int a, int b)
{
    int uchoice;
    int read = scanf("%d", &uchoice);
    clear_input_buffer();

    if (read == 1 && uchoice >= a && uchoice <= b)
    {
        *choice = uchoice;
        return 1;
    }
    *choice = -1;
    return 0;
}
