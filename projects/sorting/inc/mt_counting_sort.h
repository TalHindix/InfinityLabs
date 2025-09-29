/**************************************
Exercise: 	SA - Multithreaded Counting Sort
Date:		29/9/2025
Developer:	Tal Hindi
Reviewer: 	Guy Argaman
Status:		Wait For Review
**************************************/

#ifndef __ILRD_MT_COUNTING_SORT_H__
#define __ILRD_MT_COUNTING_SORT_H__

#include <stddef.h> /* size_t */

#define ARRAY_SIZE 20

int MTCountingSort(char* arr, size_t length, size_t num_of_threads); 

int MTCountingSortFalseSharing(char* arr, size_t length, size_t num_of_threads); 

char* ReadDictionary(size_t* length);

#endif /* __ILRD_MT_COUNTING_SORT_H__ */