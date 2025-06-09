#include <stdio.h>

int main(void)
{
    int ch;

	system("stty -icanon -echo");	
	
    printf("Press A / T, Esc to quit …\n");

    while ((ch = getchar()))
    {
        if (ch == 27)     /* esc in ASCII */    
            break;
        else if (ch == 'A')
            printf("A-pressed\n");
        else if (ch == 'T')
            printf("T-pressed\n");
    }
    
    system("stty icanon echo");
    
    return 0;
}
