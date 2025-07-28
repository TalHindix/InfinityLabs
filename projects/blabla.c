/**************************************
Exercise:   Prog - VSA (Fast Fit, Improved Naming)
Date:       27/07/2025
Developer:  Tal Hindi
Reviewer:   Yael
Status:     
**************************************/

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "vsa.h"

#define WORD_SIZE      (sizeof(size_t))
#define ALIGN_UP(x)    (((x) + (WORD_SIZE - 1)) & ~(WORD_SIZE - 1))
#define ALIGN_DOWN(x)  ((x) & ~(WORD_SIZE - 1))

struct vsa { void *end_of_pool; };

typedef struct block_header
{
    size_t size_and_alloc_flag; // LSB = allocation flag, rest = size
#ifndef NDEBUG
    size_t debug_magic;
#endif
} block_header_t;

#define BLOCK_HEADER_SIZE    ALIGN_UP(sizeof(block_header_t))
#define VSA_META_SIZE       ALIGN_UP(sizeof(vsa_t))

#define IS_BLOCK_ALLOCATED(sz)   (((sz) & 1) != 0)
#define SET_BLOCK_ALLOC(sz)      (ALIGN_UP(sz) | 1)
#define SET_BLOCK_FREE(sz)       (ALIGN_DOWN(sz))
#define BLOCK_PAYLOAD_SIZE(sz)   (IS_BLOCK_ALLOCATED(sz) ? SET_BLOCK_FREE(sz) : (sz))

static char *GetBasePtr(const vsa_t *vsa);
static block_header_t *GetFirstBlockHeader(vsa_t *vsa);
static block_header_t *GetLastBlockHeader(vsa_t *vsa);
static block_header_t *GetNextBlockHeader(const vsa_t *vsa, block_header_t *current_header);
static block_header_t *GetPrevBlockHeader(const vsa_t *vsa, block_header_t *block_header);

size_t VSASuggestSize(size_t min_payload)
{
    size_t aligned_payload = ALIGN_UP(min_payload);
    return VSA_META_SIZE + BLOCK_HEADER_SIZE + aligned_payload + BLOCK_HEADER_SIZE;
}

vsa_t* VSAInit(void* pool, size_t pool_size)
{
    char *base = NULL;
    vsa_t *vsa = NULL;
    block_header_t *first_block = NULL;
    block_header_t *last_block = NULL;
    size_t usable_bytes = 0;

    assert(pool != NULL);

    base = (char *)pool;
    vsa = (vsa_t *)base;
    vsa->end_of_pool = base + pool_size;

    first_block = GetFirstBlockHeader(vsa);
    last_block  = GetLastBlockHeader(vsa);
    assert(first_block < last_block);

    usable_bytes = (size_t)((char *)last_block - ((char *)first_block + BLOCK_HEADER_SIZE));
    usable_bytes = ALIGN_DOWN(usable_bytes);
    assert(usable_bytes >= WORD_SIZE);

    first_block->size_and_alloc_flag = usable_bytes;
#ifndef NDEBUG
    first_block->debug_magic = 0xC0FFEEUL;
#endif

    last_block->size_and_alloc_flag = 0;
#ifndef NDEBUG
    last_block->debug_magic = 0xDEADBEEFUL;
#endif

    return vsa;
}

void* VSAAlloc(vsa_t* vsa, size_t requested_size)
{
    block_header_t *current_block = NULL;
    block_header_t *fit_block = NULL;
    size_t contiguous_free_bytes = 0;
    size_t needed_bytes = 0;

    assert(vsa != NULL);

    needed_bytes = ALIGN_UP(requested_size);

    if (needed_bytes == 0)
    {
        return NULL;
    }

    for (current_block = GetFirstBlockHeader(vsa); BLOCK_PAYLOAD_SIZE(current_block->size_and_alloc_flag) != 0; current_block = GetNextBlockHeader(vsa, current_block))
    {
        if (!IS_BLOCK_ALLOCATED(current_block->size_and_alloc_flag))
        {
            size_t current_free_bytes = BLOCK_PAYLOAD_SIZE(current_block->size_and_alloc_flag);

            if (contiguous_free_bytes == 0)
            {
                fit_block = current_block;
                contiguous_free_bytes = current_free_bytes;
            }
            else
            {
                contiguous_free_bytes += BLOCK_HEADER_SIZE;
                contiguous_free_bytes += current_free_bytes;
            }

            if (contiguous_free_bytes >= needed_bytes)
            {
                size_t leftover_bytes = contiguous_free_bytes - needed_bytes;
                block_header_t *remaining_block = NULL;

                fit_block->size_and_alloc_flag = contiguous_free_bytes;

                if (leftover_bytes >= (BLOCK_HEADER_SIZE + WORD_SIZE))
                {
                    // Split block
                    remaining_block = (block_header_t *)((char *)fit_block + BLOCK_HEADER_SIZE + needed_bytes);
                    remaining_block->size_and_alloc_flag = ALIGN_DOWN(leftover_bytes - BLOCK_HEADER_SIZE);
#ifndef NDEBUG
                    remaining_block->debug_magic = 0xC0FFEEUL;
#endif
                    fit_block->size_and_alloc_flag = needed_bytes;
                }

                fit_block->size_and_alloc_flag = SET_BLOCK_ALLOC(fit_block->size_and_alloc_flag);
#ifndef NDEBUG
                fit_block->debug_magic = 0xBADC0DEUL;
#endif
                return (char *)fit_block + BLOCK_HEADER_SIZE;
            }
        }
        else
        {
            fit_block = NULL;
            contiguous_free_bytes = 0;
        }
    }

    return NULL;
}

// Coalesce adjacent free blocks on free
void VSAFree(void* user_ptr)
{
    block_header_t *block_to_free = NULL;
    block_header_t *next_block = NULL;

    if (user_ptr == NULL) { return; }

    block_to_free = (block_header_t *)((char *)user_ptr - BLOCK_HEADER_SIZE);
    assert(IS_BLOCK_ALLOCATED(block_to_free->size_and_alloc_flag));

    block_to_free->size_and_alloc_flag = SET_BLOCK_FREE(block_to_free->size_and_alloc_flag);
#ifndef NDEBUG
    block_to_free->debug_magic = 0xC0FFEEUL;
#endif

    // Coalesce with next block if it's free
    next_block = GetNextBlockHeader(NULL, block_to_free);
    if (next_block && !IS_BLOCK_ALLOCATED(next_block->size_and_alloc_flag) && BLOCK_PAYLOAD_SIZE(next_block->size_and_alloc_flag) != 0)
    {
        block_to_free->size_and_alloc_flag += BLOCK_HEADER_SIZE + BLOCK_PAYLOAD_SIZE(next_block->size_and_alloc_flag);
    }
    // Coalesce with previous block if it's free -- see note in previous review
}

size_t VSALargestChunkAvailable(const vsa_t* vsa)
{
    const block_header_t *current_block = NULL;
    size_t max_contiguous_free_bytes = 0;
    size_t current_run_free_bytes = 0;

    assert(vsa != NULL);

    for (current_block = GetFirstBlockHeader((vsa_t *)vsa); BLOCK_PAYLOAD_SIZE(current_block->size_and_alloc_flag) != 0; current_block = GetNextBlockHeader(vsa, (block_header_t *)current_block))
    {
        if (!IS_BLOCK_ALLOCATED(current_block->size_and_alloc_flag))
        {
            size_t block_free_bytes = BLOCK_PAYLOAD_SIZE(current_block->size_and_alloc_flag);

            if (current_run_free_bytes == 0)
            {
                current_run_free_bytes = block_free_bytes;
            }
            else
            {
                current_run_free_bytes += BLOCK_HEADER_SIZE;
                current_run_free_bytes += block_free_bytes;
            }

            if (current_run_free_bytes > max_contiguous_free_bytes)
            {
                max_contiguous_free_bytes = current_run_free_bytes;
            }
        }
        else
        {
            current_run_free_bytes = 0;
        }
    }

    return max_contiguous_free_bytes;
}

static char *GetBasePtr(const vsa_t *vsa)
{
    return (char *)vsa;
}

static block_header_t *GetFirstBlockHeader(vsa_t *vsa)
{
    return (block_header_t *)(GetBasePtr(vsa) + VSA_META_SIZE);
}

static block_header_t *GetLastBlockHeader(vsa_t *vsa)
{
    return (block_header_t *)((char *)vsa->end_of_pool - BLOCK_HEADER_SIZE);
}

static block_header_t *GetNextBlockHeader(const vsa_t *vsa, block_header_t *current_header)
{
    size_t block_payload_size = BLOCK_PAYLOAD_SIZE(current_header->size_and_alloc_flag);
    if (block_payload_size == 0)
    {
        return NULL;
    }
    return (block_header_t *)((char *)current_header + BLOCK_HEADER_SIZE + block_payload_size);
}

// Optional: Implement GetPrevBlockHeader for full coalescing (not used for now)
static block_header_t *GetPrevBlockHeader(const vsa_t *vsa, block_header_t *block_header)
{
    block_header_t *current = GetFirstBlockHeader((vsa_t *)vsa);
    block_header_t *prev = NULL;

    while (current && current < block_header)
    {
        prev = current;
        current = GetNextBlockHeader(vsa, current);
    }
    return prev;
}
