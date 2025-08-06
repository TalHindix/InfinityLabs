/**************************************
Exercise: 	SA - Linear Sorting
Date:		6/8/2025
Developer:	Tal Hindi
Reviewer: 	Avi Tobar
Status:		
**************************************/

#ifndef ILRD__LINEAR_SORTING_H__
#define ILRD__LINEAR_SORTING_H__

#include <stddef.h> /* size_t */

/*
 * CountingSort:
 * --------------
 * Sorts an array of integers in ascending order using the Counting Sort algorithm.
 *
 * Parameters:
 *   arr  - Pointer to the array of integers to be sorted.
 *   size - Number of elements in the array.
 *
 * Time Complexity:
 *   O(n + k), where k is the range of input values.
 *
 * Space Complexity:
 *   O(n + k), additional memory used for counting and output arrays.
 *
 * Notes:
 *   - Stable sort.
 *   - Efficient when the range of values is not significantly greater than the number of elements.
 *   - Supports both positive and negative integers.
 *   - Modifies the input array in place.
 */
void CountingSort(int* arr, size_t size);

/*
 * RadixSort:
 * ----------
 * Sorts an array of non-negative integers in ascending order using the Radix Sort algorithm (base-10).
 *
 * Parameters:
 *   arr  - Pointer to the array of non-negative integers to be sorted.
 *   size - Number of elements in the array.
 *
 * Time Complexity:
 *   O(d × (n + k)), where d is the number of digits and k is the base (10).
 *
 * Space Complexity:
 *   O(n + k), additional memory used during digit-wise counting sort.
 *
 * Notes:
 *   - Stable sort.
 *   - Only works with non-negative integers.
 *   - Modifies the input array in place.
 *   - Internally uses Counting Sort for each digit position.
 */
void RadixSort(int* arr, size_t size);

#endif /* ILRD__LINEAR_SORTING_H__ */

