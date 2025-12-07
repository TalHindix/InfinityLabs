#include <stdio.h>


int TargetFound(int* arr, size_t size, int target)
{
    size_t i = 0;
    int found = 1;

    for(i = 0; i < size; ++i)
    {
        found *= arr[i] ^ target;
    }

    return !found;
}

int main(void)
{
    int arr[] = {1,4,5,7,8};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    int y = TargetFound(arr,size,10);

    printf("Y:%d\n",y);


    return 0;
}
