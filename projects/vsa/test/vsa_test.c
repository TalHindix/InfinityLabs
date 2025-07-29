/**************************************
Exercise:   Prog - VSA
Date:       27/07/2025
Developer:  Tal Hindi
Reviewer:   Yael
Status:		Approved
**************************************/

#include <stdio.h>   /* printf  */
#include <stdlib.h>  /* malloc  */
#include <assert.h>  /* assert  */
#include <stddef.h>  /* size_t  */

#include "vsa.h"     /* VSAInit */

#define POOL_SIZE 256

#ifndef WORD_SIZE
#define WORD_SIZE      (sizeof(size_t))
#endif

#ifndef ALIGN_UP
#define ALIGN_UP(x)    (((x) + (WORD_SIZE - 1)) & ~(WORD_SIZE - 1))
#endif


#ifndef ABS
#define ABS(x)         ((x) < 0 ? -(x) : (x))
#endif


typedef struct test_vsa
{
    void *end_pool;
} test_vsa_t;

typedef struct test_header
{
    long block_size;
#ifndef NDEBUG
    size_t magic_number;
#endif
} test_header_t;

#define TEST_HDR_SIZE  ((size_t)sizeof(test_header_t))
#define TEST_VSA_SIZE  ALIGN_UP(sizeof(test_vsa_t))

static test_header_t *TEST_FirstHdr(const test_vsa_t *vsa)
{
    return (test_header_t *)((char *)vsa + TEST_VSA_SIZE);
}

static int TEST_InPool(const test_vsa_t *vsa, const void *p)
{
    return (char *)p < (char *)vsa->end_pool;
}

static test_header_t *TEST_NextHdr(const test_vsa_t *vsa, const test_header_t *hdr)
{
    size_t jump = TEST_HDR_SIZE + (size_t)ABS(hdr->block_size);
    (void)vsa;
    return (test_header_t *)((char *)hdr + jump);
}

static size_t VSAT_CountBlocks(const vsa_t *base)
{
    const test_vsa_t *vsa = (const test_vsa_t *)base;
    test_header_t *hdr = TEST_FirstHdr(vsa);
    size_t cnt = 0;

    while (TEST_InPool(vsa, hdr))
    {
        ++cnt;
        hdr = TEST_NextHdr(vsa, hdr);
    }
    return cnt;
}

static size_t VSAT_CountFree(const vsa_t *base)
{
    const test_vsa_t *vsa = (const test_vsa_t *)base;
    test_header_t *hdr = TEST_FirstHdr(vsa);
    size_t cnt = 0;

    while (TEST_InPool(vsa, hdr))
    {
        if (hdr->block_size > 0) { ++cnt; }
        hdr = TEST_NextHdr(vsa, hdr);
    }
    return cnt;
}

static size_t VSAT_CountAlloc(const vsa_t *base)
{
    const test_vsa_t *vsa = (const test_vsa_t *)base;
    test_header_t *hdr = TEST_FirstHdr(vsa);
    size_t cnt = 0;

    while (TEST_InPool(vsa, hdr))
    {
        if (hdr->block_size < 0) { ++cnt; }
        hdr = TEST_NextHdr(vsa, hdr);
    }
    return cnt;
}


void Test_VSAInit_Alloc_Free()
{
    char pool[POOL_SIZE];
    vsa_t* vsa = NULL;
    void* block1 = NULL;
    void* block2 = NULL;
    size_t free_before = 0;
    size_t free_after = 0;

    printf("Test: VSAInit -> Alloc -> Free\n");

    vsa = VSAInit(pool, POOL_SIZE);

    printf("  After init: blocks=%lu free=%lu alloc=%lu\n",
        (unsigned long)VSAT_CountBlocks(vsa),
        (unsigned long)VSAT_CountFree(vsa),
        (unsigned long)VSAT_CountAlloc(vsa));

    block1 = VSAAlloc(vsa, 32);
    block2 = VSAAlloc(vsa, 16);

    printf("  Alloc block1 (32 bytes): %s\n", (block1 != NULL) ? "PASS" : "FAIL");
    printf("  Alloc block2 (16 bytes): %s\n", (block2 != NULL) ? "PASS" : "FAIL");

    printf("  After allocs: blocks=%lu free=%lu alloc=%lu\n",
        (unsigned long)VSAT_CountBlocks(vsa),
        (unsigned long)VSAT_CountFree(vsa),
        (unsigned long)VSAT_CountAlloc(vsa));

    free_before = VSALargestChunkAvailable(vsa);
    printf("  Free space before free: %lu\n", (unsigned long)free_before);

    VSAFree(block1);
    VSAFree(block2);

    printf("  After frees: blocks=%lu free=%lu alloc=%lu\n",
        (unsigned long)VSAT_CountBlocks(vsa),
        (unsigned long)VSAT_CountFree(vsa),
        (unsigned long)VSAT_CountAlloc(vsa));

    free_after = VSALargestChunkAvailable(vsa);
    printf("  Free space after free: %lu\n", (unsigned long)free_after);

    printf("  Defragmentation effective: %s\n\n", (free_after >= free_before) ? "PASS" : "FAIL");
}

void Test_VSALargestChunkAvailable_EmptyPool()
{
    char pool[POOL_SIZE];
    vsa_t *vsa = NULL;
    size_t largest = 0;

    printf("Test: VSALargestChunkAvailable on Empty Pool\n");

    vsa = VSAInit(pool, POOL_SIZE);
    largest = VSALargestChunkAvailable(vsa);

    printf("  After init: blocks=%lu free=%lu alloc=%lu\n",
        (unsigned long)VSAT_CountBlocks(vsa),
        (unsigned long)VSAT_CountFree(vsa),
        (unsigned long)VSAT_CountAlloc(vsa));

    printf("  Largest free chunk: %lu -> %s\n\n",
        (unsigned long)largest, (largest > 0) ? "PASS" : "FAIL");
}

void Test_VSAAlloc_Split()
{
    char pool[POOL_SIZE];
    vsa_t* vsa = NULL;
    void* b1 = NULL;
    void* b2 = NULL;
    void* b3 = NULL;

    printf("Test: VSAAlloc Multiple Small Blocks\n");

    vsa = VSAInit(pool, POOL_SIZE);
    b1 = VSAAlloc(vsa, 16);
    b2 = VSAAlloc(vsa, 16);
    b3 = VSAAlloc(vsa, 16);

    printf("  Block1 (16Byte): %s\n", (b1 != NULL) ? "PASS" : "FAIL");
    printf("  Block2 (16Byte): %s\n", (b2 != NULL) ? "PASS" : "FAIL");
    printf("  Block3 (16Byte): %s\n", (b3 != NULL) ? "PASS" : "FAIL");

    printf("  After allocs: blocks=%lu free=%lu alloc=%lu\n\n",
        (unsigned long)VSAT_CountBlocks(vsa),
        (unsigned long)VSAT_CountFree(vsa),
        (unsigned long)VSAT_CountAlloc(vsa));
}

void Test_VSAAlloc_TooLarge()
{
    char pool[POOL_SIZE];
    vsa_t *vsa = NULL;
    void *block = NULL;

    printf("Test: VSAAlloc Too Large\n");

    vsa = VSAInit(pool, POOL_SIZE);
    block = VSAAlloc(vsa, POOL_SIZE);

    printf("  After try: blocks=%lu free=%lu alloc=%lu\n",
        (unsigned long)VSAT_CountBlocks(vsa),
        (unsigned long)VSAT_CountFree(vsa),
        (unsigned long)VSAT_CountAlloc(vsa));

    printf("  Block allocation (%d bytes): %s\n\n",
        POOL_SIZE, (block == NULL) ? "PASS" : "FAIL");
}


int main(void)
{
    printf("==== VSA Tests ====\n\n");

    Test_VSAInit_Alloc_Free();
    Test_VSALargestChunkAvailable_EmptyPool();
    Test_VSAAlloc_Split();
    Test_VSAAlloc_TooLarge();


    return 0;
}

