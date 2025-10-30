#include <stdio.h>

int main()
{ 
    const int i = 3;    
    int *ip = (int*)&i;
    *ip = 5;
    int arr[i]; /* ISO C90 forbids mixed declarations and code , Also we must init the arr with size */  
                /* ISO C90 forbids variable length array ‘arr’ */

    printf("%d %d\n", i, *ip);
    return 0;
}

