/**************************************
Exercise:   Prog - FSA
Date:       24/07/2025
Developer:  Tal Hindi
Reviewer:   Daniel Gamliel
Status:     
**************************************/

#include <stdio.h>   /* printf     */
#include <stdlib.h>  /* malloc     */
#include <string.h>  /* memset     */
#include <assert.h>  /* assert     */

#include "fsa.h"     /* FSAInit  */

#define CHECK(cond, msg)                                    \
    do {                                                    \
        if (cond) {                                         \
            ++g_pass;                                       \
            printf("PASS: %s\n", msg); \
        } else {                                            \
            ++g_fail;                                       \
            printf("FAIL: %s (line %d)\n", msg, __LINE__); \
        }                                                   \
    } while (0)

static unsigned g_pass = 0;
static unsigned g_fail = 0;

#define BLOCK_SIZE_SMALL   8    
#define BLOCK_SIZE_WEIRD   13  
#define BLOCK_AMOUNT       10

static void Test_SuggestSize_Normal(void);
static void Test_Init_And_FirstState(void);
static void Test_Alloc_All_Then_Free_All(void);
static void Test_Alloc_Exhaustion(void);
static void Test_Free_Null_Ignored(void);
static void Test_Free_OutOfOrder(void);
static void Test_Multiple_Init_Blocksizes(void);
static void Test_Recycle_Blocks(void);


int main(void)
{
    printf("=== FSA Unit Tests ===\n\n");

    Test_SuggestSize_Normal();
    Test_Init_And_FirstState();
    Test_Alloc_All_Then_Free_All();
    Test_Alloc_Exhaustion();
    Test_Free_Null_Ignored();
    Test_Free_OutOfOrder();
    Test_Multiple_Init_Blocksizes();
    Test_Recycle_Blocks();

    printf("Total: PASS=%u, FAIL=%u\n", g_pass, g_fail);
    
    return (g_fail == 0) ? 0 : 1;
}

static void *AllocPool(size_t block_amount, size_t block_size, fsa_t **out_fsa)
{
    size_t suggest = FSASuggestSize(block_amount, block_size);
    void *pool = malloc(suggest);
    fsa_t *fsa = NULL;

    assert(out_fsa);

    CHECK(pool != NULL, "malloc(pool) succeeded");
    if (!pool)
    {
        *out_fsa = NULL;
        return NULL;
    }

    memset(pool, 0xAA, suggest);
    fsa = FSAInit(pool, suggest, block_size);
    CHECK(fsa != NULL, "FSAInit returned non-NULL");

    *out_fsa = fsa;
    return pool;
}

/* 1. FSASuggestSize with normal inputs */
static void Test_SuggestSize_Normal(void)
{
    size_t s1 = FSASuggestSize(1, 8);
    size_t s2 = FSASuggestSize(10, 32);

    CHECK(s1 > 0, "SuggestSize returns >0 (1 block)");
    CHECK(s2 > s1, "SuggestSize grows with more/larger blocks");
}

/* 2. Init + initial state validation */
static void Test_Init_And_FirstState(void)
{
    fsa_t *fsa = NULL;
    void *pool = AllocPool(BLOCK_AMOUNT, BLOCK_SIZE_SMALL, &fsa);
    if (!pool) { return; }

    CHECK(FSACountFree(fsa) == BLOCK_AMOUNT, "All blocks free after init");
    free(pool);
}

/* 3. Allocate all blocks, then free all, check counts */
static void Test_Alloc_All_Then_Free_All(void)
{
    fsa_t *fsa = NULL;
    void *pool = AllocPool(BLOCK_AMOUNT, BLOCK_SIZE_SMALL, &fsa);
    void *blocks[BLOCK_AMOUNT];
    size_t i = 0;

    if (!pool) { return; }

    for (i = 0; i < BLOCK_AMOUNT; ++i)
    {
        blocks[i] = FSAAlloc(fsa);
        CHECK(blocks[i] != NULL, "Alloc returned non-NULL until exhaustion");
    }

    CHECK(FSAAlloc(fsa) == NULL, "Alloc returns NULL after pool is full");
    CHECK(FSACountFree(fsa) == 0, "FSACountFree == 0 after full allocation");

    for (i = 0; i < BLOCK_AMOUNT; ++i)
    {
        FSAFree(fsa, blocks[i]);
    }

    CHECK(FSACountFree(fsa) == BLOCK_AMOUNT, "All blocks free after freeing all");

    free(pool);
}

/* 4. Exhaustion test: allocate n+1 blocks */
static void Test_Alloc_Exhaustion(void)
{
    fsa_t *fsa = NULL;
    void *pool = AllocPool(BLOCK_AMOUNT, BLOCK_SIZE_SMALL, &fsa);
    void *p = NULL;
    size_t i = 0;

    if (!pool) { return; }

    for (i = 0; i < BLOCK_AMOUNT; ++i)
    {
        p = FSAAlloc(fsa);
        CHECK(p != NULL, "Alloc before exhaustion");
    }

    CHECK(FSAAlloc(fsa) == NULL, "Extra alloc returns NULL");

    free(pool);
}

/* 5. Free(NULL) should be ignored */
static void Test_Free_Null_Ignored(void)
{
    fsa_t *fsa = NULL;
    void *pool = AllocPool(BLOCK_AMOUNT, BLOCK_SIZE_SMALL, &fsa);
    void *p = NULL;

    if (!pool) { return; }

    p = FSAAlloc(fsa);
    CHECK(p != NULL, "Alloc OK for NULL test");

    FSAFree(fsa, NULL);
    CHECK(FSACountFree(fsa) == (BLOCK_AMOUNT - 1), "Free(NULL) didn't change count");

    FSAFree(fsa, p);
    CHECK(FSACountFree(fsa) == BLOCK_AMOUNT, "Free(real) restored count");

    free(pool);
}

/* 6. Free blocks in a different order to ensure freelist works */
static void Test_Free_OutOfOrder(void)
{
    fsa_t *fsa = NULL;
    void *pool = AllocPool(BLOCK_AMOUNT, BLOCK_SIZE_SMALL, &fsa);
    void *a = NULL, *b = NULL, *c = NULL;

    if (!pool) { return; }

    a = FSAAlloc(fsa);
    b = FSAAlloc(fsa);
    c = FSAAlloc(fsa);

    CHECK(FSACountFree(fsa) == (BLOCK_AMOUNT - 3), "Count after 3 allocs");

    FSAFree(fsa, b);
    FSAFree(fsa, a);
    FSAFree(fsa, c);

    CHECK(FSACountFree(fsa) == BLOCK_AMOUNT, "All back after out-of-order frees");

    free(pool);
}

/* 7. Multiple initializations with different block sizes (alignment edge cases) */
static void Test_Multiple_Init_Blocksizes(void)
{
    fsa_t *fsa1 = NULL, *fsa2 = NULL;
    void *pool1 = AllocPool(5, 1, &fsa1);
    void *pool2 = AllocPool(7, BLOCK_SIZE_WEIRD, &fsa2);

    if (pool1)
    {
        CHECK(FSACountFree(fsa1) == 5, "5 blocks free in first FSA");
        free(pool1);
    }

    if (pool2)
    {
        CHECK(FSACountFree(fsa2) == 7, "7 blocks free in second FSA");
        free(pool2);
    }
}

/* 8. Recycle blocks: alloc, free, alloc again */
static void Test_Recycle_Blocks(void)
{
    fsa_t *fsa = NULL;
    void *pool = AllocPool(BLOCK_AMOUNT, BLOCK_SIZE_SMALL, &fsa);
    void *p1 = NULL;
    void *p2 = NULL;

    if (!pool) { return; }

    p1 = FSAAlloc(fsa);
    CHECK(p1 != NULL, "Alloc p1");

    FSAFree(fsa, p1);
    CHECK(FSACountFree(fsa) == BLOCK_AMOUNT, "After free p1 all free again");

    p2 = FSAAlloc(fsa);
    CHECK(p2 != NULL, "Alloc p2 after free");
    CHECK(FSACountFree(fsa) == (BLOCK_AMOUNT - 1), "Count decremented again");

    FSAFree(fsa, p2);
    free(pool);
}


