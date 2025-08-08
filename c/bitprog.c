#include <stdio.h>


#define SET_BIT(var, n)   ((var) |= (1 << (n)))
#define UNSET_BIT(var, n) ((var) &= ~(1 << (n)))
#define IS_BIT_SET(var, n) (((var) >> (n)) & 1)
#define TOGGLE_BIT(var,n) ((var) ^= (1 << (n)))
#define COMPLEMENT(var) ((var) = ~(var))

void PrintBinary(int c);

int main(void)
{
    int num = 0;
    printf("%d\n",num);
    printf("Binary: ");
    PrintBinary(num);
    SET_BIT(num,0);
    printf("Binary: "); 
    PrintBinary(num);
    return 0;
}


void PrintBinary(int c)
{
    int i = 0;
    for (i = sizeof(int) * 8 - 1; i >= 0; --i)
    {
        char bit = (c >> i) & 1;
        putchar(bit ? '1' : '0');
    }
    putchar('\n');
}