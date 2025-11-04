#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CirculerShift(char arr[],size_t size, size_t num_shift);
void Reverse(char* arr, size_t start, size_t end);
void Swap(char* a, char* b);
int main(void)
{
    char arr[4] = {'A','B','C','D'};
    size_t size = strlen(arr);
    size_t num_shift = 2;

    CirculerShift(arr,size,num_shift);

    return 0;
}

void CirculerShift(char arr[],size_t size, size_t num_shift)
{
    num_shift %= size;

    Reverse();
    Reverse();
    Reverse();
}

void Reverse(char* arr, size_t start, size_t end)
{
    while(*arr != '\0')
    {
        if(start < end)
        {
            Swap(&arr[start],&arr[end]);
            start++;
            end--;
        }
    }
}

void Swap(char* a, char* b)
{
    char temp = *a;
    *a = *b;
    *b = temp;
}