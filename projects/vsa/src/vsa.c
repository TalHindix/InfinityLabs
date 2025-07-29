/**************************************
Exercise:   Prog - VSA
Date:       27/07/2025
Developer:  Tal Hindi
Reviewer:   Yael
Status:     Approved
**************************************/

#include <assert.h> /* assert 	*/

#include "vsa.h"	/* VSAInit 	*/

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define WORD_SIZE sizeof(size_t)
#define ALIGN_UP(x)  (((size_t)(x) + (WORD_SIZE - 1)) & ~(WORD_SIZE - 1))
#define ALIGN_DOWN(x)  ((size_t)(x) & ~(WORD_SIZE - 1))
#define MIN_SPLIT_BYTES   (sizeof(header_t) + WORD_SIZE)

struct vsa 
{
	void* end_pool;
};

struct header
{
	long block_size;
	#ifndef NDEBUG
		size_t magic_number;
	#endif
};

static int InBounds(vsa_t* vsa, const void* p);
static header_t* FirstHdr(vsa_t* vsa);
static header_t* NextHdr(header_t* h);
static header_t* FindFitAndCommit(vsa_t* vsa, size_t need);
static header_t* CommitAlloc(header_t* block, size_t run_size, size_t request_size);
static void Defrag(vsa_t* vsa);

vsa_t* VSAInit(void* memory_pool, size_t total_size)
{
    vsa_t* vsa = NULL;
    header_t* first_block = NULL;
    size_t alignment_offset = 0;
    size_t aligned_pool_size = 0;
    size_t available_space = 0;

    vsa = (vsa_t*)ALIGN_UP(memory_pool);
    alignment_offset = (size_t)((char*)vsa - (char*)memory_pool);
    if (total_size <= alignment_offset)
    {
        return NULL;
    }

    aligned_pool_size = ALIGN_DOWN(total_size - alignment_offset);
    if (aligned_pool_size < sizeof(vsa_t) + sizeof(header_t) + WORD_SIZE)
    {
        return NULL;
    }

    first_block = (header_t*)((char*)vsa + sizeof(vsa_t));
    available_space = aligned_pool_size - sizeof(vsa_t);
    vsa->end_pool = (char*)vsa + aligned_pool_size;
    first_block->block_size = (long)(available_space - sizeof(header_t));

	#ifndef NDEBUG
    	first_block->magic_number = VSA_MAGIC_NUMBER;
	#endif

    return vsa;
}

void* VSAAlloc(vsa_t* vsa, size_t block_size)
{
    header_t* block_allocated = NULL;

    assert(vsa);

    block_size = ALIGN_UP(block_size);
    if (0 == block_size)
    {
        return NULL;
    }

    block_allocated = FindFitAndCommit(vsa, block_size);

    return block_allocated ? (char*)block_allocated + sizeof(header_t) : NULL;
}

void VSAFree(void* block)
{
	header_t* header = NULL;

    if (NULL == block)
    {
        return;
    }

    header = (header_t*)((char*)block - sizeof(header_t));

    assert(header->block_size < 0);
	#ifndef NDEBUG
		assert(header->magic_number == VSA_MAGIC_NUMBER);
	#endif

    header->block_size = -header->block_size;

	#ifndef NDEBUG
		header->magic_number = VSA_MAGIC_NUMBER;
	#endif
}

size_t VSALargestChunkAvailable(vsa_t* vsa)
{
	header_t* curr = NULL;
    size_t largest = 0;

    assert(vsa);

    Defrag(vsa);

    curr = FirstHdr(vsa);

    while (InBounds(vsa, curr))
    {
        if (curr->block_size > 0 && (size_t)curr->block_size > largest)
        {
            largest = (size_t)curr->block_size;
        }

        curr = NextHdr(curr);
    }

    return largest;
}

static header_t* FirstHdr(vsa_t* vsa)
{
    return (header_t*)((char*)vsa + sizeof(vsa_t));
}

static header_t* NextHdr(header_t* h)
{
    return (header_t*)((char*)h + sizeof(header_t) + ABS(h->block_size));
}

static int InBounds(vsa_t* vsa, const void* p)
{
    const void* start = (const void*)FirstHdr(vsa);
    return (p >= start) && (p < vsa->end_pool);
}

static header_t* FindFitAndCommit(vsa_t* vsa, size_t request_size)
{
    header_t* cur = NULL;
    header_t* run_start = NULL;
    size_t run_total_bytes = 0;
    size_t part_total_bytes = 0;

    assert(vsa);

    for (cur = FirstHdr(vsa); InBounds(vsa, cur); cur = NextHdr(cur))
    {
        if (cur->block_size > 0) /* free block */
        {
            part_total_bytes = sizeof(header_t) + (size_t)cur->block_size;

            if (0 == run_total_bytes)
            {
                run_start = cur;
            }

            run_total_bytes += part_total_bytes;

            if (run_total_bytes >= request_size + MIN_SPLIT_BYTES)
            {
                return CommitAlloc(run_start, run_total_bytes, request_size);
            }
        }
        else
        {
            /* reset run if block is allocated */
            run_start = NULL;
            run_total_bytes = 0;
        }
    }

    return NULL;
}

static header_t* CommitAlloc(header_t* block, size_t run_total_bytes, size_t request_size)
{
    size_t full_payload = run_total_bytes - sizeof(header_t);
    long remaining = (long)full_payload - (long)request_size;

    if (remaining >= (long)MIN_SPLIT_BYTES)
    {
        /* Create new free block after allocation */
        header_t* new_free = (header_t*)((char*)block + sizeof(header_t) + request_size);
        new_free->block_size = remaining - (long)sizeof(header_t);
        #ifndef NDEBUG
        new_free->magic_number = VSA_MAGIC_NUMBER;
        #endif

        block->block_size = -(long)request_size; /* mark allocated */
    }
    else
    {
        /* Not enough room to split - take entire run */
        block->block_size = -(long)full_payload;
    }

    #ifndef NDEBUG
    block->magic_number = VSA_MAGIC_NUMBER;
    #endif

    return block;
}

static void Defrag(vsa_t* vsa)
{
    header_t* curr = NULL;

    assert(vsa);

    for (curr = FirstHdr(vsa); InBounds(vsa, curr); curr = NextHdr(curr))
    {
        if (curr->block_size > 0)
        {
            header_t* next = NextHdr(curr);

            while (InBounds(vsa, next) && next->block_size > 0)
            {
                curr->block_size += (long)sizeof(header_t) + next->block_size;
                next = NextHdr(next);
            }
        }
    }
}

