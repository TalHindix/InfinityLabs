/**************************************
Exercise: 	SA - Cmp Sorting
Date:		5/8/2025
Developer:	Tal Hindi
Reviewer: 	Avi Tobar
Status:		Approved
**************************************/

#ifndef ILRD__CMP_SORTING_H__
#define ILRD__CMP_SORTING_H__

#include <stddef.h> /* size_t */

/*
 * BubbleSort:
 * -----------
 * Sorts an array of integers in ascending order using the Bubble Sort algorithm.
 *
 * Parameters:
 *   arr  - Pointer to the array of integers to be sorted.
 *   size - Number of elements in the array.
 *
 * Time Complexity:
 *   Worst-case: O(n^2)
 *   Best-case:  O(n) (when array is already sorted)
 *
 * Space Complexity:
 *   O(1) - In-place sorting
 *
 * Notes:
 *   - Stable sort.
 *   - Simple to implement, but inefficient for large arrays.
 */
void BubbleSort(int* arr, size_t size);

/*
 * SelectionSort:
 * --------------
 * Sorts an array of integers in ascending order using the Selection Sort algorithm.
 *
 * Parameters:
 *   arr  - Pointer to the array of integers to be sorted.
 *   size - Number of elements in the array.
 *
 * Time Complexity:
 *   O(n^2) in all cases.
 *
 * Space Complexity:
 *   O(1) - In-place sorting
 *
 * Notes:
 *   - Not stable by default.
 *   - Minimizes the number of swaps.
 *   - Suitable when writes are expensive (e.g., flash memory).
 */
void SelectionSort(int* arr, size_t size);

/*
 * InsertionSort:
 * --------------
 * Sorts an array of integers in ascending order using the Insertion Sort algorithm.
 *
 * Parameters:
 *   arr  - Pointer to the array of integers to be sorted.
 *   size - Number of elements in the array.
 *
 * Time Complexity:
 *   Worst-case: O(n^2)
 *   Best-case:  O(n) (when array is nearly sorted)
 *
 * Space Complexity:
 *   O(1) - In-place sorting
 *
 * Notes:
 *   - Stable sort.
 *   - Efficient for small or nearly sorted arrays.
 *   - Often used as the base case for faster recursive sorts.
 */
void InsertionSort(int* arr, size_t size);

#endif /* __CMP_SORTING_H__ */
/**************************************
Exercise: 	SA - Cmp Sorting
Date:		5/8/2025
Developer:	Tal Hindi
Reviewer: 	Avi Tobar
Status:		Approved
**************************************/

#ifndef __CMP_SORTING_H__
#define __CMP_SORTING_H__

#include <stddef.h> /* size_t */

/*
 * BubbleSort:
 * -----------
 * Sorts an array of integers in ascending order using the Bubble Sort algorithm.
 *
 * Parameters:
 *   arr  - Pointer to the array of integers to be sorted.
 *   size - Number of elements in the array.
 *
 * Time Complexity:
 *   Worst-case: O(n^2)
 *   Best-case:  O(n) (when array is already sorted)
 *
 * Space Complexity:
 *   O(1) - In-place sorting
 *
 * Notes:
 *   - Stable sort.
 *   - Simple to implement, but inefficient for large arrays.
 */
void BubbleSort(int* arr, size_t size);

/*
 * SelectionSort:
 * --------------
 * Sorts an array of integers in ascending order using the Selection Sort algorithm.
 *
 * Parameters:
 *   arr  - Pointer to the array of integers to be sorted.
 *   size - Number of elements in the array.
 *
 * Time Complexity:
 *   O(n^2) in all cases.
 *
 * Space Complexity:
 *   O(1) - In-place sorting
 *
 * Notes:
 *   - Not stable by default.
 *   - Minimizes the number of swaps.
 *   - Suitable when writes are expensive (e.g., flash memory).
 */
void SelectionSort(int* arr, size_t size);

/*
 * InsertionSort:
 * --------------
 * Sorts an array of integers in ascending order using the Insertion Sort algorithm.
 *
 * Parameters:
 *   arr  - Pointer to the array of integers to be sorted.
 *   size - Number of elements in the array.
 *
 * Time Complexity:
 *   Worst-case: O(n^2)
 *   Best-case:  O(n) (when array is nearly sorted)
 *
 * Space Complexity:
 *   O(1) - In-place sorting
 *
 * Notes:
 *   - Stable sort.
 *   - Efficient for small or nearly sorted arrays.
 *   - Often used as the base case for faster recursive sorts.
 */
void InsertionSort(int* arr, size_t size);

#endif /* ILRD__CMP_SORTING_H__ */

