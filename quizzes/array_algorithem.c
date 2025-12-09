#include <stdio.h>


int IsFound(char arr[],size_t size,char target)
{
    size_t i = 0;
    int found = 1;

    while(i < size)
    {
        found *= arr[i] ^ target;
        ++i;
    }

    return !found;
}

int main(void)
{
   char arr[] = {'a','b','c'};
   size_t size = sizeof(arr) / sizeof(arr[0]);
   char target = 'c';

   int is_found = IsFound(arr,size,target);
   
   printf("%d\n",is_found);
}

