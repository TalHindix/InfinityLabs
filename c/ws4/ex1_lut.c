#include <stdio.h>

typedef void (*key_func_t)();

void A_pressed() { printf("A-pressed\n"); }
void T_pressed() { printf("T-pressed\n"); }
void empty()     { /* no action */ }

int main()
{
    key_func_t lut[256];
    int ch;
    size_t i;
    
    system("stty -icanon -echo");

    for (i = 0; i < 256; ++i)
        lut[i] = empty;

  
    lut['A'] = A_pressed;
    lut['T'] = T_pressed;

    printf("Press A / T, Esc to quit …\n");

    while ((ch = getchar()))
    {
        if (ch == 27)
            break;

        lut[(unsigned char)ch]();
    }

	system("stty icanon echo");
    return 0;
}

