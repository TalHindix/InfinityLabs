/**************************************
Exercise: 	SA - Recursive Sorting
Date:		23/8/2025
Developer:	Tal Hindi
Reviewer: 	
Status:		
**************************************/

#ifndef __ILRD_REC_SORTING_H__
#define __ILRD_REC_SORTING_H__

#include <stddef.h> /* size_t */

size_t IterBinarySearch(const int arr[],size_t size, int target);
size_t RecBinarySearch(const int arr[],size_t size, int target);

void IMergeSort(int* arr_to_sort, size_t num_elements);
int RecMergeSort(int* arr_to_sort, size_t num_elements);

#endif /* __ILRD_REC_SORTING_H__ */
