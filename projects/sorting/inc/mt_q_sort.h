/**************************************
Exercise: 	SA - Multithreaded Quick Sort
Date:		29/9/2025
Developer:	Tal Hindi
Reviewer: 	Yuval Hochman
Status:		Approved
**************************************/

#ifndef __ILRD_MT_QUICK_SORT_H__
#define __ILRD_MT_QUICK_SORT_H__

#include <stddef.h> /* size_t */

int MTQuickSort(char** arr, size_t length, size_t num_of_threads); 

void Shuffle(char** arr, size_t length); 

char* ReadDictionary(size_t* length);

char** CreateArrayPointers(char* buffer, size_t length, size_t* num_of_words);

char** DuplicatePointers(char** original, size_t num_of_words, size_t num_of_copies, size_t* total_words);

#endif /* __ILRD_MT_QUICK_SORT_H__ */