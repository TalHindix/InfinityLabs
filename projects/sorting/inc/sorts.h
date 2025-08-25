/**************************************
Exercise:	SA - Recursive Sorting
Date:		23/8/2025
Developer:	Tal Hindi
Reviewer:   Yuval Hochman
Status:     Approved
**************************************/


#ifndef __ILRD_SORTS_H__
#define __ILRD_SORTS_H__

#include <stddef.h> /* size_t */

typedef int (*cmp_func_t)(const void* data1,const void* data2);

size_t IterBinarySearch(const int arr[],size_t size, int target);
size_t RecBinarySearch(const int arr[],size_t size, int target);

void IMergeSort(int* arr_to_sort, size_t num_elements);
int RecMergeSort(int* arr_to_sort, size_t num_elements);

void QuickSort(void* base, size_t num_items, size_t ele_size, cmp_func_t cmp_func);


#endif /* __ILRD_SORTS_H__ */
