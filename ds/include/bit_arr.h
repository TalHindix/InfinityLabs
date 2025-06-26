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
#include <stdlib.h>
#include <string.h>


typedef size_t bit_arr_t; /* */
enum { BITARR_BITS = sizeof(bit_arr_t) * CHAR_BIT }; /* */
enum { BYTE_VALUES_LUT_SIZE = 1 << CHAR_BIT }; /* 0-255 */

/*
	BitArrSetAllOn
	Set every bit in the given array to 1
	
	Pre-condition: caller supplies any bit_arr_t value
	Returns value: a copy of the original bit array with all bits set to 1.
	Time Complexity : O(1) – constant time!
*/
bit_arr_t BitArrSetAllOn(bit_arr_t bit_arr);
/*
    BitArrSetAllOff
    every bits in the given array sets to 0.
    
    Pre-condition: caller supplies any bit_arr_t value
    Returns value: a copy of the original bit array with every bit sets/cleared to 0
    Time Complexity : O(1) – constant time!
*/
bit_arr_t BitArrSetAllOff(bit_arr_t bit_arr);


/*
	BitArrSetBit
	set a single bit.
	
	Pre-condition: bit_arr_t value , 0 < index < BITARR_BITS , and value {0,1}
	Returns:a copy of bit array with the index bit set with the value you are given.
	Time Complexity : O(1) – constant time!
*/
bit_arr_t BitArrSetBit(bit_arr_t bit_arr, size_t index, int value);


/*
	BitArrGetBit
	Reads a single bit.
	
	Pre-condition: bit_arr_t value , 0 < index < BITARR_BITS
	Returns value: 1 if the bit is set, 0 otherwise.
	Time Complexity : O(1) – constant time!
*/
int BitArrGetBit(bit_arr_t bit_arr, size_t index);


/*
	BitArrSetOn
	Sets the bit at index to 1
	
	Pre-condition: bit_arr_t value , 0 ≤ index < BITARR_BITS
	Returns value: a copy of the bit_arr with the bit set in the index.
	Time Complexity : O(1) – constant time!
*/
bit_arr_t BitArrSetOn(bit_arr_t bit_arr, size_t index);


/*
	BitArrSetOff
	Clears the bit at index (sets to 0).
	
	Pre-condition: bit_arr_t value , 0 < index < BITARR_BITS
	Returns value: 1 if the bit is set, 0 otherwise.
	Time Complexity : O(1) – constant time!
*/
bit_arr_t BitArrSetOff(bit_arr_t bit_arr, size_t index);


/*
	BitArrRotateLeft
	Circular-shifts the bits left by shift positions.
	
	Pre-condition: bit_arr_t value , shift (any value because MOD)
	Returns value: rotated bit_arr
	Time Complexity : O(1) – constant time!
*/
bit_arr_t BitArrRotateLeft(bit_arr_t bit_arr, size_t shift);


/*
	BitArrRotateRight
	Circular-shifts the bits right by shift positions.
	
	Pre-condition: bit_arr_t value , shift (any value because MOD)
	Returns value: rotated bit_arr
	Time Complexity : O(1) – constant time!
*/
bit_arr_t BitArrRotateRight(bit_arr_t bit_arr, size_t shift);


/*
	BitArrFlipBit
	Toggles (XOR) the bit at index.
	
	Pre-condition: bit_arr_t value, 0 ≤ index < BITARR_BITS
	Returns value: copy of bitarray with the bit flipped
	Time Complexity : O(1) – constant time!
*/
bit_arr_t BitArrFlipBit(bit_arr_t bit_arr, size_t index);


/*
	BitArrCountOn
	Counts the number of set bits(1)
	
	Pre-condition: bit_arr_t value
	Returns value: number of the set bits 
	Time Complexity : O(1) – constant time!
*/
size_t BitArrCountOn(bit_arr_t bit_arr);


/*
	BitArrCountOff
	Counts the number of cleared bits(0)
	
	Pre-condition: bit_arr_t value
	Returns value: number of the 0-bits 
	Time Complexity : O(1) – constant time!
*/
size_t BitArrCountOff(bit_arr_t bit_arr);


/*
	BitArrMirror
	Reverses the bit order
	
	Pre-condition: bit_arr_t value
	Returns value: mirrored bitarray
	Time Complexity : O(BITARR_BITS) – constant time!
*/
bit_arr_t BitArrMirror(bit_arr_t bit_arr);


/*
	BitArrToString
	Write the bitArray in text/string "010111"
	
	Pre-condition: bit_arr_t value, str != NULL
	Returns value: pointer to str on success, NULL if str == NULL
	Time Complexity : O(BITARR_BITS) – constant time!
*/
char *BitArrToString(bit_arr_t bit_arr, char* str);



bit_arr_t BitArrMirrorLUT(bit_arr_t bit_arr);

size_t BitArrCountOnLUT(bit_arr_t value);
	
#endif /* _ILRD_BIT_ARR_ */


