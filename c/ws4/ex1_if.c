/*****************
author: Tal Hindi
reviewer: Menny Markovich
status: Approved
******************/


#include <stdio.h>

#define ESC 27
int main()
{
    int ch;

	system("stty -icanon -echo");	
	
    while ((ch = getchar()))
    {
        if (ch == ESC)     /* esc in ASCII */    
            break;
        else if (ch == 'A')
            printf("A-pressed\n");
        else if (ch == 'T')
            printf("T-pressed\n");
    }
    
    system("stty icanon echo");
    
    return 0;
}
