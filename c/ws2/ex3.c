#include <stdio.h>
#include <stdlib.h>


/**********************
author: Tal Hindi
reviewer: Baruch Haimson
status:	approved

**********************/

int main()
{
    static int s_i = 7;
    int i = 7;
    int* ptr = &i;
    int* ptr2 = (int *)malloc(sizeof(int));
    
    printf("Address of static variable s_i: %p\n", (void*)&s_i);
    printf("Address of local variable i: %p\n", (void*)&i);
    printf("Address of pointer ptr (on stack): %p\n", (void*)&ptr);
    printf("Value of ptr (address of i): %p\n", (void*)ptr);
    printf("Address of heap variable (*ptr2): %p\n", (void*)ptr2);
    printf("Address of pointer ptr2 (on stack): %p\n", (void*)&ptr2);

    if (ptr)
    {
        int** ptr3 = &ptr;
        printf("Address of pointer to ptr (ptr3): %p\n", (void*)&ptr3);
        printf("Value of ptr3 (address of ptr): %p\n", (void*)ptr3);
        printf("Value of *ptr3 (address of i): %p\n", (void*)*ptr3);
        printf("Value of **ptr3 (value of i): %d\n", **ptr3);
    }

    free(ptr2);

    return 0;
}

