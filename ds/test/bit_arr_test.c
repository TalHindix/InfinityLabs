#include <stdio.h>
#include "../include/bit_arr.h" 

/*-------------------------------------------------------------*/
void TestBitArrSetAllOn()
{
    bit_arr_t val = 0;
    bit_arr_t expected = (bit_arr_t)~0;
    bit_arr_t result = BitArrSetAllOn(val);
    printf("SetAllOn: exp 0x%lX, got 0x%lX -> %s\n",
           (unsigned long)expected, (unsigned long)result,
           (result == expected) ? "PASS" : "FAIL");
}
/*-------------------------------------------------------------*/
void TestBitArrSetAllOff()
{
    bit_arr_t val = (bit_arr_t)~0;
    bit_arr_t expected = 0;
    bit_arr_t result = BitArrSetAllOff(val);
    printf("SetAllOff: exp 0x%lX, got 0x%lX -> %s\n",
           (unsigned long)expected, (unsigned long)result,
           (result == expected) ? "PASS" : "FAIL");
}
/*-------------------------------------------------------------*/
void TestBitArrSetOn()
{
    bit_arr_t expected = (bit_arr_t)1 << 5;
    bit_arr_t result = BitArrSetOn(0, 5);
    printf("SetOn(bit5): exp 0x%lX, got 0x%lX -> %s\n",
           (unsigned long)expected, (unsigned long)result,
           (result == expected) ? "PASS" : "FAIL");
}
/*-------------------------------------------------------------*/
void TestBitArrSetOff()
{
    bit_arr_t initial = BitArrSetOn(0, 5);
    bit_arr_t expected = 0;
    bit_arr_t result = BitArrSetOff(initial, 5);
    printf("SetOff(bit5): exp 0x%lX, got 0x%lX -> %s\n",
           (unsigned long)expected, (unsigned long)result,
           (result == expected) ? "PASS" : "FAIL");
}
/*-------------------------------------------------------------*/
void TestBitArrSetBit()
{
    bit_arr_t expected1 = (bit_arr_t)1 << 10;  
    bit_arr_t result1   = 0;               
    bit_arr_t expected0 = 0;                
    bit_arr_t result0   = 0;                 

    result1 = BitArrSetBit(0, 10, 1);
    printf("SetBit(10,1): exp 0x%lX, got 0x%lX -> %s\n",
           (unsigned long)expected1, (unsigned long)result1,
           (result1 == expected1) ? "PASS" : "FAIL");

    result0 = BitArrSetBit(result1, 10, 0);
    printf("SetBit(10,0): exp 0x%lX, got 0x%lX -> %s\n",
           (unsigned long)expected0, (unsigned long)result0,
           (result0 == expected0) ? "PASS" : "FAIL");
}
/*-------------------------------------------------------------*/
void TestBitArrGetBit()
{
    bit_arr_t val = BitArrSetOn(0, 3);
    int expected = 1;
    int result = BitArrGetBit(val, 3);
    printf("GetBit(3): exp %d, got %d -> %s\n",
           expected, result, (result == expected) ? "PASS" : "FAIL");
}
/*-------------------------------------------------------------*/
void TestBitArrFlipBit()
{
    bit_arr_t val       = 0;                   
    bit_arr_t expected0 = 0;                  
    bit_arr_t result0   = 0;                  
    bit_arr_t expected1 = 0;                  
    bit_arr_t result1   = 0;               

    val       = BitArrSetOn(0, 3);
    result0   = BitArrFlipBit(val, 3);
    printf("FlipBit off: exp 0x%lX, got 0x%lX -> %s\n",
           (unsigned long)expected0, (unsigned long)result0,
           (result0 == expected0) ? "PASS" : "FAIL");

    expected1 = (bit_arr_t)1 << 3;
    result1   = BitArrFlipBit(result0, 3);
    printf("FlipBit on : exp 0x%lX, got 0x%lX -> %s\n",
           (unsigned long)expected1, (unsigned long)result1,
           (result1 == expected1) ? "PASS" : "FAIL");
}
/*-------------------------------------------------------------*/
void TestBitArrRotateLeft()
{
    bit_arr_t val = (bit_arr_t)1 << 3;
    size_t shift = 1;
    bit_arr_t expected = (bit_arr_t)1 << 4;
    bit_arr_t result = BitArrRotateLeft(val, shift);
    printf("RotateLeft(%lu): exp 0x%lX, got 0x%lX -> %s\n",
           (unsigned long)shift, (unsigned long)expected,
           (unsigned long)result, (result == expected) ? "PASS" : "FAIL");
}
/*-------------------------------------------------------------*/
void TestBitArrRotateRight()
{
    bit_arr_t val = (bit_arr_t)1 << 3;
    size_t    shift = 1;
    bit_arr_t expected = (val >> shift) | (val << (BITARR_BITS - shift));
    bit_arr_t result = BitArrRotateRight(val, shift);
    printf("RotateRight(%lu): exp 0x%lX, got 0x%lX -> %s\n",
           (unsigned long)shift, (unsigned long)expected,
           (unsigned long)result, (result == expected) ? "PASS" : "FAIL");
}
/*-------------------------------------------------------------*/
void TestBitArrCountOn()
{
    bit_arr_t val = 0xF0F0F0F0F0F0F0F0;
    size_t expected = 32;
    size_t result = BitArrCountOn(val);
    printf("CountOn: exp %lu, got %lu -> %s\n",
           (unsigned long)expected, (unsigned long)result,
           (result == expected) ? "PASS" : "FAIL");
}
/*-------------------------------------------------------------*/
void TestBitArrCountOff()
{
    bit_arr_t val = 0xF0F0F0F0F0F0F0F0;
    size_t expected = (size_t)BITARR_BITS - 32;
    size_t result = BitArrCountOff(val);
    printf("CountOff: exp %lu, got %lu -> %s\n",
           (unsigned long)expected, (unsigned long)result,
           (result == expected) ? "PASS" : "FAIL");
}
/*-------------------------------------------------------------*/
void TestBitArrMirrorLUT()
{
    bit_arr_t val = 0x00000000000000F0;
    bit_arr_t expected = 0x0F00000000000000;
    bit_arr_t result = BitArrMirrorLUT(val);
    printf("Mirror LUT: exp 0x%lX, got 0x%lX -> %s\n",
           (unsigned long)expected, (unsigned long)result,
           (result == expected) ? "PASS" : "FAIL");
}
/*-------------------------------------------------------------*/
void TestBitArrCountOnLUT()
{
    bit_arr_t val = 0xF0F0F0F0F0F0F0F0;
    size_t expected = 32;
    size_t result = BitArrCountOnLUT(val);
    printf("CountOn LUT: exp %lu, got %lu -> %s\n",
           (unsigned long)expected, (unsigned long)result,
           (result == expected) ? "PASS" : "FAIL");
}
/*-------------------------------------------------------------*/
void TestBitArrToString()
{
    bit_arr_t val=0x8000000000000001;
    char expected[]="1000000000000000000000000000000000000000000000000000000000000001";
    char str[BITARR_BITS+1]={0};
    char *result=BitArrToString(val,str);
    printf("ToString: exp %s, got %s -> %s\n", expected, result, (0==strcmp(expected,result)) ? "PASS" : "FAIL");
}
int main()
{
    TestBitArrSetAllOn();
    TestBitArrSetAllOff();
    TestBitArrSetOn();
    TestBitArrSetOff();
    TestBitArrSetBit();
    TestBitArrGetBit();
    TestBitArrFlipBit();
    TestBitArrCountOff();
    TestBitArrCountOn();
	TestBitArrRotateLeft();
    TestBitArrRotateRight();
    TestBitArrMirrorLUT();
    TestBitArrCountOnLUT();
    TestBitArrToString();
    return 0;
}

