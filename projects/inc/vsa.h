/**************************************
Exercise:   Prog - VSA
Date:       27/07/2025
Developer:  Tal Hindi
Reviewer:   Yael
Status:     
**************************************/

#ifndef __VSA_H__
#define __VSA_H__

#include <stddef.h> /* size_t */

#define VSA_MAGIC_NUMBER 0xDEADBEEF

typedef struct header header_t;
typedef struct vsa vsa_t;

/**
 * Initialize the VSA allocator.
 *
 * Arguments:
 *   - pool: memory buffer from malloc.
 *   - pool_size: size of the buffer.
 *
 * Returns:
 *   - Pointer to the allocator (vsa_t*) on success.
 *   - NULL if not enough space.
 *
 * Notes:
 *   - All metadata is stored in the buffer.
 *   - Alignment is handled automatically.
 */
vsa_t* VSAInit(void* pool, size_t pool_size);

/**
 * Allocate memory from the pool.
 *
 * Arguments:
 *   - vsa: allocator pointer.
 *   - size: number of bytes requested.
 *
 * Returns:
 *   - Pointer to usable memory if success.
 *   - NULL if not enough space.
 *
 * Notes:
 *   - Coalescing happens before allocation.
 */
void* VSAAlloc(vsa_t* vsa, size_t size);

/**
 * Free allocated memory.
 *
 * Arguments:
 *   - ptr: pointer returned from VSAAlloc.
 *
 * Notes:
 *   - If ptr is NULL, does nothing.
 *   - Pointer must be from VSAAlloc.
 */
void VSAFree(void* block);

/**
 * Get the size of the largest free block.
 *
 * Arguments:
 *   - vsa: allocator pointer.
 *
 * Returns:
 *   - Number of bytes in the largest free chunk.
 *
 * Notes:
 *   - Reflects current state after coalescing.
 */
size_t VSALargestChunkAvailable(vsa_t* vsa);

#endif /* __VSA_H__ */

