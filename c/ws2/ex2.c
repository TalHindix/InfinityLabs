#include <stdio.h> /*printf*/

/**************************
author:	Tal Hindi
reviewer: Baruch Haimson
approved: approved

**************************/


void CopyArray(const int *arr_src, int *arr_dest, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		*(arr_dest+i) = *(arr_src +i);
	}
}

void PrintArray(const int *arr, int size)
{
	int i;
	for (i=0; i < size; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
}


int main()
{
    int arr1[] = {1, 2, 3, 4, 5};
    int arr2[5] = {0, 0, 0, 0, 0};
    int arr3[] = {-1, -2, -3, -4, -5};
    int arr4[5] = {0, 0, 0, 0, 0};
    int arr5[] = {42};
    int arr6[1] = {0};
    int size1, size2;

    /* Test 1: Copy a regular array */
    size1 = sizeof(arr1) / sizeof(arr1[0]);
    printf("=== Test 1: Regular Array ===\n");
    printf("Before copy: ");
    PrintArray(arr2, size1);
    CopyArray(arr1, arr2, size1);
    printf("After copy:  ");
    PrintArray(arr2, size1);

    /* Test 2: Copy an array with negative numbers */
    size2 = sizeof(arr3) / sizeof(arr3[0]);
    printf("\n=== Test 2: Array with Negative Numbers ===\n");
    printf("Before copy: ");
    PrintArray(arr4, size2);
    CopyArray(arr3, arr4, size2);
    printf("After copy:  ");
    PrintArray(arr4, size2);

    /* Test 3: Copy an array with a single element */
    printf("\n=== Test 3: Single Element Array ===\n");
    printf("Before copy: ");
    PrintArray(arr6, 1);
    CopyArray(arr5, arr6, 1);
    printf("After copy:  ");
    PrintArray(arr6, 1);



	return 0;
}
