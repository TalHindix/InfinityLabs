/********************
author: Tal Hindi
reviewer: 
status: 
********************/

#ifndef __ILRD_BIT_ARR__
#define __ILRD_BIT_ARR__

#include <limits.h>   /* CHAR_BIT */
#include <stddef.h>   /* size_t   */
#include <assert.h>   /* assert   */

typedef size_t bit_arr_t;
enum { BITARR_BITS = sizeof(bit_arr_t) * CHAR_BIT };
/*
	BitArrSetAllOn
	Pre-condition: caller supplies any bit_arr_t value
	The function does not validate the input.
	Returns: a copy of bit array with every bit set to 1
	Time Complexity : O(1) – constant time!
*/
bit_arr_t BitArrSetAllOn(bit_arr_t bit_arr);
/*
    BitArrSetAllOff
    Pre-condition: caller supplies any bit_arr_t value
	The function does not validate the input.
    Returns: a copy of bit array with every bit cleared to 0
    Time Complexity : O(1) – constant time!
*/
bit_arr_t BitArrSetAllOff(bit_arr_t bit_arr);
/*
	BitArrSetBit
	Pre-condition: caller supplies any bit_arr_t value , size_t index, and value (0,1)
	The function does not validate the input.
	Returns:a copy of bit array with the index bit set with the value you are given.
	Time Complexity : O(1) – constant time!
*/
bit_arr_t BitArrSetBit(bit_arr_t bit_arr, size_t index, int value);
/*
	BitArrGetBit
	Pre-condition: 
	The function does not validate the input.
	Returns:
	Time Complexity : O(1) – constant time!
*/
int BitArrGetBit(bit_arr_t bit_arr, size_t index);
/*
	BitArrSetOn
	Pre-condition: 
	The function does not validate the input.
	Returns:
	Time Complexity : O(1) – constant time!
*/
bit_arr_t BitArrSetOn(bit_arr_t bit_arr, size_t index);
/*
	BitArrSetOff
	Pre-condition: 
	The function does not validate the input.
	Returns:
	Time Complexity : O(1) – constant time!
*/
bit_arr_t BitArrSetOff(bit_arr_t bit_arr, size_t index);

bit_arr_t BitArrRotateLeft(bit_arr_t bit_arr, size_t shift);
bit_arr_t BitArrRotateRight(bit_arr_t bit_arr, size_t shift);
bit_arr_t BitArrFlipBit(bit_arr_t bit_arr, size_t index);
size_t BitArrCountOn(bit_arr_t bit_arr);
size_t BitArrCountOff(bit_arr_t bit_arr);
bit_arr_t BitArrMirror(bit_arr_t bit_arr);
char *BitArrToString(bit_arr_t bit_arr, char* str);
	
#endif /* _ILRD_BIT_ARR_ */


