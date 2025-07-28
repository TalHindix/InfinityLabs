/**************************************
Exercise:   Prog - VSA
Date:       27/07/2025
Developer:  Tal Hindi
Reviewer:   Yael
Status:     
**************************************/

#include <assert.h>
#include <stddef.h>

#include "vsa.h"

#define WORD_SIZE      (sizeof(size_t))
#define ALIGN_UP(x)    (((x) + (WORD_SIZE - 1)) & ~(WORD_SIZE - 1))
#define ALIGN_DOWN(x)  ((x) & ~(WORD_SIZE - 1))

struct vsa { void *end_pool; };

typedef struct header
{
    size_t block_size;
#ifdef NDEBUG
    size_t magic_number;
#endif
} header_t;


#define HEADER_SIZE    ALIGN_UP(sizeof(header_t))
#define VSA_META_SIZE  ALIGN_UP(sizeof(vsa_t))

#define IS_ALLOC(sz)   (((sz) % WORD_SIZE) == 1)
#define TO_ALLOC(sz)   (ALIGN_UP(sz) + 1)
#define TO_FREE(sz)    (ALIGN_DOWN(sz))
#define PAYLOAD(sz)    (IS_ALLOC(sz) ? TO_FREE(sz) : (sz))

static char     *BasePtr(const vsa_t *vsa);
static header_t *FirstHdr(vsa_t *vsa);
static header_t *LastHdr(vsa_t *vsa);
static header_t *NextHdr(const vsa_t *vsa, header_t *hdr);

static size_t PtrToSize(const void *p) { return (size_t)(p); }

size_t VSASuggestSize(size_t min_payload)
{
    size_t want = ALIGN_UP(min_payload);
    return VSA_META_SIZE + HEADER_SIZE + want + HEADER_SIZE;
}

vsa_t* VSAInit(void* pool, size_t pool_size)
{
    char *base = NULL;
    vsa_t *vsa = NULL;
    header_t *first_hdr = NULL;
    header_t *last_hdr = NULL;
    size_t free_bytes = 0;

    assert(pool != NULL);

    base = (char *)pool;
    vsa = (vsa_t *)base;
    vsa->end_pool = base + pool_size;

    first_hdr = FirstHdr(vsa);
    last_hdr  = LastHdr(vsa);
    assert(first_hdr < last_hdr);

    free_bytes = (size_t)((char *)last_hdr - ((char *)first_hdr + HEADER_SIZE));
    free_bytes = ALIGN_DOWN(free_bytes);
    assert(free_bytes >= WORD_SIZE);

    first_hdr->block_size = free_bytes;
#ifdef NDEBUG
    first_hdr->magic_number = 0xC0FFEEUL;
#endif

    last_hdr->block_size = 0;
#ifdef NDEBUG
    last_hdr->magic_number = 0xDEADBEEFUL;
#endif

    return vsa;
}

void* VSAAlloc(vsa_t* vsa, size_t size)
{
    header_t *hdr = NULL;
    header_t *run_begin = NULL;
    size_t free_run_bytes = 0;
    size_t need = 0;

    assert(vsa);

    need = ALIGN_UP(size);
    if (need == 0)
    {
        return NULL;
    }

    for (hdr = FirstHdr(vsa); PAYLOAD(hdr->block_size) != 0; hdr = NextHdr(vsa, hdr))
    {
        if (!IS_ALLOC(hdr->block_size))
        {
            size_t part_bytes = PAYLOAD(hdr->block_size);

            if (free_run_bytes == 0)
            {
                run_begin = hdr;
                free_run_bytes = part_bytes;
            }
            else
            {
                free_run_bytes += HEADER_SIZE;
                free_run_bytes += part_bytes;
            }

            if (free_run_bytes >= need)
            {
                size_t remain = 0;
                header_t *rest_hdr = NULL;

                run_begin->block_size = free_run_bytes;
                remain = free_run_bytes - need;

                if (remain >= (HEADER_SIZE + WORD_SIZE))
                {
                    rest_hdr = (header_t *)((char *)run_begin + HEADER_SIZE + need);
                    rest_hdr->block_size = ALIGN_DOWN(remain - HEADER_SIZE);
#ifdef NDEBUG
                    rest_hdr->magic_number = 0xC0FFEEUL;
#endif
                }

                run_begin->block_size = TO_ALLOC(need);
                return (char *)run_begin + HEADER_SIZE;
            }
        }
        else
        {
            run_begin = NULL;
            free_run_bytes = 0;
        }
    }

    return NULL;
}

void VSAFree(void* ptr)
{
#ifndef NDEBUG
    assert(ptr != NULL);
#else
    if (ptr == NULL) { return; }
#endif

    {
        header_t *hdr = (header_t *)((char *)ptr - HEADER_SIZE);
        if (IS_ALLOC(hdr->block_size))
        {
            hdr->block_size = TO_FREE(hdr->block_size);
#ifdef NDEBUG
            hdr->magic_number = 0xC0FFEEUL;
#endif
        }
    }
}

size_t VSALargestChunckAvailable(const vsa_t* vsa)
{
    const header_t *hdr = NULL;
    size_t max_run_bytes = 0;
    size_t free_run_bytes = 0;

    assert(vsa != NULL);

    for (hdr = FirstHdr((vsa_t *)vsa); PAYLOAD(hdr->block_size) != 0; hdr = NextHdr(vsa, (header_t *)hdr))
    {
        if (!IS_ALLOC(hdr->block_size))
        {
            size_t part_bytes = PAYLOAD(hdr->block_size);

            if (free_run_bytes == 0)
            {
                free_run_bytes = part_bytes;
            }
            else
            {
                free_run_bytes += HEADER_SIZE;
                free_run_bytes += part_bytes;
            }

            if (free_run_bytes > max_run_bytes)
            {
                max_run_bytes = free_run_bytes;
            }
        }
        else
        {
            free_run_bytes = 0;
        }
    }

    return max_run_bytes;
}

static char *BasePtr(const vsa_t *vsa)
{
    return (char *)vsa;
}

static header_t *FirstHdr(vsa_t *vsa)
{
    return (header_t *)(BasePtr(vsa) + VSA_META_SIZE);
}

static header_t *LastHdr(vsa_t *vsa)
{
    return (header_t *)((char *)vsa->end_pool - HEADER_SIZE);
}

static header_t *NextHdr(const vsa_t *vsa, header_t *hdr)
{
    size_t sz = PAYLOAD(hdr->block_size);
    (void)vsa;
    if (sz == 0)
    {
        return NULL;
    }
    return (header_t *)((char *)hdr + HEADER_SIZE + sz);
}

