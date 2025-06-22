/**********************
author: Tal Hindi
reviewer: 
status: 
**********************/

#include <stdio.h>
#include <string.h>     /* for the standard memset and memcpy and memmove */
#include "mem_utils.h"  /* my own MemSet and MemCpy and MemMove */

#define BUF_SIZE   32   /* Size of the test buffer */
#define FILL_BYTE  0xAA /* background value to detect overflow */

/* Macro to print test result */
#define TEST_RESULT(cond, msg) \
    printf("%s  %s\n", (cond) ? "PASS" : "FAIL", msg)


int RunMemSetTest(size_t offset, size_t len, int value)
{
    unsigned char buf_std[BUF_SIZE];
    unsigned char buf_my[BUF_SIZE];
    size_t i;

    for (i = 0; i < BUF_SIZE; ++i) {
        buf_std[i] = FILL_BYTE;
        buf_my[i]  = FILL_BYTE;
    }

    memset(buf_std + offset, value, len);
    MemSet(buf_my  + offset, value, len);

    return memcmp(buf_std, buf_my, BUF_SIZE) == 0;
}

static void TestMemSet()
{
    printf("\nTesting MemSet function:\n\n");

    TEST_RESULT(RunMemSetTest(0, 0, 0x00), "n == 0 leaves buffer unchanged");
    TEST_RESULT(RunMemSetTest(0, 1, 0x5A), "fill 1 byte at start");
    TEST_RESULT(RunMemSetTest(0, sizeof(unsigned long), 0xFF), "fill exactly one word");
    TEST_RESULT(RunMemSetTest(3, 13, 0x7E), "fill across alignment boundary");
    TEST_RESULT(RunMemSetTest(0, BUF_SIZE, 0x00), "fill entire buffer");

    printf("\n");
}


int RunMemCpyTest(size_t offset, size_t len)
{
    unsigned char src_buf[BUF_SIZE];
    unsigned char dst_std[BUF_SIZE];
    unsigned char dst_my [BUF_SIZE];
    size_t i;


    for (i = 0; i < BUF_SIZE; ++i)
    {
        src_buf[i] = (unsigned char)(i + 1);
        dst_std[i] = FILL_BYTE;
        dst_my[i]  = FILL_BYTE;
    }


    memcpy(dst_std + offset, src_buf + offset, len);
    MemCpy(dst_my  + offset, src_buf + offset, len);

    return memcmp(dst_std, dst_my, BUF_SIZE) == 0;
}

static void TestMemCpy()
{
    printf("\nTesting MyMemCpy function:\n");

    TEST_RESULT(RunMemCpyTest(0, 0),"n == 0 leaves buffer unchanged");
    TEST_RESULT(RunMemCpyTest(0, 1),"copy 1 byte at start");
    TEST_RESULT(RunMemCpyTest(0, sizeof(unsigned long)), "copy exactly one word");
    TEST_RESULT(RunMemCpyTest(3, 13), "copy unaligned span");
    TEST_RESULT(RunMemCpyTest(0, BUF_SIZE), "copy full buffer");

    printf("\n");
}


int RunMemMoveTest(size_t dest_offset, size_t src_offset, size_t len)
{
    unsigned char buf_std[BUF_SIZE];
    unsigned char buf_my [BUF_SIZE];
    size_t i;

    for (i = 0; i < BUF_SIZE; ++i) {
        buf_std[i] = (unsigned char)(i + 1);
        buf_my[i]  = (unsigned char)(i + 1);
    }

    memmove(buf_std + dest_offset, buf_std + src_offset, len);
    MemMove(buf_my  + dest_offset, buf_my  + src_offset, len);

    return memcmp(buf_std, buf_my, BUF_SIZE) == 0;
}

static void TestMemMove()
{
    printf("\nTesting MemMove function:\n");

    TEST_RESULT(RunMemMoveTest(0, 0, 0), "n == 0 leaves buffer unchanged");
    TEST_RESULT(RunMemMoveTest(0, 5, 5), "non-overlapping copy");
    TEST_RESULT(RunMemMoveTest(0, 3, 10), "overlap: dest < src (safe forward)");
    TEST_RESULT(RunMemMoveTest(3, 0, 10), "overlap: dest > src (needs backward)");
    TEST_RESULT(RunMemMoveTest(0, 0, BUF_SIZE), "copy full buffer");

    printf("\n");
}


int main()
{
    TestMemSet();
    TestMemCpy();
    TestMemMove();

    return 0;
}
