/**************************************
Exercise:   Prog - FSA
Date:       24/07/2025
Developer:  Tal Hindi
Reviewer:   Baruch Haimson
Status:     
**************************************/

#include <assert.h> /* assert */

#include "fsa.h"    /* FSAInit */

#define WORD_SIZE      (sizeof(size_t))
#define ALIGN_UP(x)    (((x) + (WORD_SIZE - 1)) & ~(WORD_SIZE - 1))
#define OFF_NULL 0

struct FSA
{
	size_t head_offset;
};

static char *PoolBase(const fsa_t *fsa);
static size_t PtrToOffset(const fsa_t *fsa, const void *ptr);
static void SetNextFree(void* block_ptr, size_t next_offset);
static void* GetBlockFromOffset(const fsa_t* fsa, size_t offset);

size_t FSASuggestSize(size_t block_amount, size_t block_size)
{
    size_t aligned_block = 0;
    size_t meta_byte = 0;

    if (block_amount == 0 || block_size == 0)
    {
        return 0;
    }

    aligned_block = ALIGN_UP(block_size);
    if (aligned_block < WORD_SIZE)
    {
        aligned_block = WORD_SIZE;
    }

    meta_byte = ALIGN_UP(sizeof(fsa_t));

    return meta_byte + block_amount * aligned_block;
}


fsa_t* FSAInit(void* pool, size_t pool_size, size_t block_size)
{
    fsa_t* fsa = NULL;
    size_t offset = 0;
    size_t aligned_block = 0;
    void* block = NULL;
    void* last = NULL;

    assert(pool != NULL);
    assert(pool_size > 0);
    assert(block_size > 0);

    aligned_block = ALIGN_UP(block_size);

    fsa = (fsa_t*)pool;
    offset = sizeof(fsa_t);

    SetNextFree(fsa, offset);

    while (offset + aligned_block < pool_size)
    {
        block = GetBlockFromOffset(fsa, offset);
        SetNextFree(block, offset + aligned_block);
        offset += aligned_block;
    }

    last = GetBlockFromOffset(fsa, offset);
    SetNextFree(last, OFF_NULL);

    fsa->head_offset = sizeof(fsa_t);

    return fsa;
}

void* FSAAlloc(fsa_t *fsa)
{
    char *base = NULL;
    void *block = NULL;

    assert(fsa);

    if (OFF_NULL == fsa->head_offset)
    {
        return NULL;
    }

    base = PoolBase(fsa);
    block = (void *)(base + fsa->head_offset);

    fsa->head_offset = *(size_t *)block;

    return block;
}

void FSAFree(fsa_t *fsa, void *block)
{
    size_t old_head = 0;

    assert(fsa);

    if (NULL == block)
    {
        return;
    }

    old_head = fsa->head_offset;
    *(size_t *)block = old_head;
    fsa->head_offset = PtrToOffset(fsa, block);
}

size_t FSACountFree(const fsa_t *fsa) 
{
    char *base = NULL;
    size_t offset = 0;
    size_t count = 0;

    assert(fsa != NULL);

    base = PoolBase(fsa);
    offset = fsa->head_offset;

    while (offset != OFF_NULL)
    {
        count++;
        offset = *(const size_t *)(base + offset);
    }

    return count;
}


static char *PoolBase(const fsa_t *fsa)
{
    return (char *)fsa;
}

static size_t PtrToOffset(const fsa_t *fsa, const void *ptr)
{
	return (size_t)((const char *)ptr - (const char *)fsa);
}

static void SetNextFree(void* block_ptr, size_t next_offset)
{
    size_t* p = (size_t *) block_ptr;
    *p = next_offset;
}
static void* GetBlockFromOffset(const fsa_t* fsa, size_t offset) 
{
    char* base = (char *) fsa;
    return (void *) (base + offset);
}

