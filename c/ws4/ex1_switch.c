/****************
author: Tal Hindi
reviewer: Menny Markovich
status: Approved
****************/

#include <stdio.h>

#define ESC 27

int main()
{

	int ch;	
	system("stty -icanon -echo");
	
	while ((ch = getchar()))
    {
        switch (ch)
        {
            case ESC: 
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
