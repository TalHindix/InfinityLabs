/******************
author: Tal Hindi
reviewer: Menny Markovich
status: Approved
******************/

#include <stdio.h>

#define SIZE 256
#define ESC 27

typedef void (*key_func_t)();

void A_pressed() { printf("A-pressed\n"); }
void T_pressed() { printf("T-pressed\n"); }
void Empty()     { /* no action */ }

int main()
{
    key_func_t lut[SIZE];
    int ch;
    size_t i;
    
    system("stty -icanon -echo");

    for (i = 0; i < SIZE; ++i)
        lut[i] = Empty;

  
    lut['A'] = A_pressed;
    lut['T'] = T_pressed;


    while ((ch = getchar()))
    {
        if (ESC == ch)
            break;

        lut[(unsigned char)ch]();
    }

	system("stty icanon echo");
    return 0;
}

