#include <stdio.h>

int main()
{
	int ch;
	
	system("stty -icanon -echo");
	printf("Press A / T , Esc To quit ...\n");
	
	while ((ch = getchar()))
    {
        switch (ch)
        {
            case 27: 
                return 0;
            case 'A':
                printf("A-pressed\n");
                break;
            case 'T':
                printf("T-pressed\n");
                break;
            default:
                break;
        }
    }
    
    system("stty icanon echo");

	return 0;
}
