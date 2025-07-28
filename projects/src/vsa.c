/**************************************
Exercise:   Prog - VSA
Date:       27/07/2025
Developer:  Tal Hindi
Reviewer:   Yael
Status:     
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
static header_t* FindAndAllocBlock(vsa_t* vsa, size_t need);
static header_t* CommitAlloc(header_t* block, size_t run_size, size_t request_size);
static void Coalesce(vsa_t* vsa);

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
    if (block_size == 0)
    {
        return NULL;
    }

    block_allocated = FindAndAllocBlock(vsa, block_size);

    return block_allocated ? (char*)block_allocated + sizeof(header_t) : NULL;
}

void VSAFree(void* block)
{
	header_t* header = NULL;

    if (block == NULL)
    {
        return;
    }

    header = (header_t*)((char*)block - sizeof(header_t));

    assert(header->block_size < 0);
	#ifndef NDEBUG
		assert(header->magic_number == 0xDEADBEEF);
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

    Coalesce(vsa);

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

static header_t* FindAndAllocBlock(vsa_t* vsa, size_t need)
{
    header_t* cur_header = NULL;
    header_t* run_header = NULL;
    size_t run_bytes = 0;
    size_t header_and_payload_size = 0;

    for (cur_header = FirstHdr(vsa); InBounds(vsa, cur_header); cur_header = NextHdr(cur_header))
    {
        if (cur_header->block_size > 0)
        {
            header_and_payload_size = sizeof(header_t) + (size_t)cur_header->block_size;

            if (run_bytes == 0)
            {
                run_header = cur_header;
            }

            run_bytes += header_and_payload_size;

            if (run_bytes >= need + sizeof(header_t))
            {
                return CommitAlloc(run_header, run_bytes, need);
            }
        }
        else
        {
            run_header = NULL;
            run_bytes = 0;
        }
    }

    return NULL;
}

static header_t* CommitAlloc(header_t* block, size_t run_size, size_t request_size)
{
    size_t payload_size = run_size - sizeof(header_t);
    long remain = (long)payload_size - (long)request_size;

    if (remain >= (long)MIN_SPLIT_BYTES) {
        header_t* new_block = (header_t*)((char*)block + sizeof(header_t) + request_size);
        new_block->block_size = remain - (long)sizeof(header_t);
        #ifndef NDEBUG
          new_block->magic_number = VSA_MAGIC_NUMBER;
        #endif

        block->block_size = -(long)request_size;
        #ifndef NDEBUG
          block->magic_number = 0xDEADBEEF;
        #endif
    } else {
        block->block_size = -(long)payload_size;
        #ifndef NDEBUG
          block->magic_number = 0xDEADBEEF;
        #endif
    }

    return block;
}

static void Coalesce(vsa_t* vsa)
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

