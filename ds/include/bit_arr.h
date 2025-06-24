/********************
author: Tal Hindi
reviewer: 
status: 
********************/

#ifndef __ILRD_BIT_ARR__
#define __ILRD_BIT_ARR__

    typedef size_t bit_arr_t;

    bit_arr_t BitArrSetAllOn(bit_arr_t bit_arr);
	bit_arr_t BitArrSetAllOff(bit_arr_t bit_arr);
	bit_arr_t BitArrSetBit(bit_arr_t bit_arr, size_t index, int value);
	int BitArrGetBit(bit_arr_t bit_arr, size_t index);
	bit_arr_t BitArrSetOn(bit_arr_t bit_arr, size_t index);
	bit_arr_t BitArrSetOff(bit_arr_t bit_arr, size_t index);
    bit_arr_t BitArrRotateLeft(bit_arr_t bit_arr, size_t shift);
	bit_arr_t BitArrRotateRight(bit_arr_t bit_arr, size_t shift);
	bit_arr_t BitArrFlipBit(bit_arr_t bit_arr, size_t index);
	size_t BitArrCountOn(bit_arr_t bit_arr);
	size_t BitArrCountOff(bit_arr_t bit_arr);
	bit_arr_t BitArrMirror(bit_arr_t bit_arr);
	char *BitArrToString(bit_arr_t bit_arr, char* str);
	
#endif /* _ILRD_BIT_ARR_ */


