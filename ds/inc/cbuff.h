/**************************************
Exercise: 	DS - Circuler Buffer
Date:		7/7/2025
Developer:	Tal Hindi
Reviewer: 	Baruch Haimson
Status:		
**************************************/

#ifndef _ILRD_CBUFF_
#define _ILRD_CBUFF_

#include <stddef.h>		/* size_t  */
#include <sys/types.h> 	/* ssize_t */

#define DEFAULT_SIZE (8)

typedef struct cbuff cbuff_t;

/**
 * @brief  Allocate and initialize a circular buffer.
 *
 * @param  capacity  Logical capacity in bytes.  
 *                   Values below #DEFAULT_SIZE are rounded up to
 *                   #DEFAULT_SIZE.
 *
 * @return Pointer to the new buffer on success, or **NULL** on failure.
 *
 * @complexity O(1)
 *
 * @note  The user is responsible for calling @ref CBuffDestroy.
 */
cbuff_t *CBuffCreate(size_t capacity);

/**
 * @brief  Release all resources held by a circular buffer.
 *
 * @param  cbuff  Pointer returned by @ref CBuffCreate.  
 *                If **NULL**, the function does nothing.
 *
 * @complexity O(1)
 */
void CBuffDestroy(cbuff_t *cbuff);

/**
 * @brief  Write exactly @p bytes from @p src into the buffer.
 *
 * The function succeeds only if the buffer has enough free space to
 * accommodate *all* requested bytes.  Partial writes are **not**
 * performed.
 *
 * @param  cbuff  Destination buffer.
 * @param  src    Source memory block (must be non-NULL).
 * @param  bytes  Number of bytes to copy.
 *
 * @return
 *  * **@p bytes** (cast to cb_ssize_t) on success;  
 *  * **-1** if insufficient free space or if @p bytes == 0.
 *
 * @complexity O(1)
 *
 * @warning  The implementation is **not thread-safe**.  
 *           External synchronisation is required for concurrent access.
 */
ssize_t CBuffWrite(cbuff_t *cbuff, const void *src, size_t bytes);

/**
 * @brief  Read (and remove) exactly @p bytes from the buffer into @p dst.
 *
 * The function succeeds only if the buffer currently stores at least
 * @p bytes.  Partial reads are **not** performed.
 *
 * @param  cbuff  Source buffer.
 * @param  dst    Destination memory block (must be non-NULL).
 * @param  bytes  Number of bytes to read.
 *
 * @return
 *  * **@p bytes** (cast to cb_ssize_t) on success;  
 *  * **-1** if the buffer contains fewer than @p bytes
 *           or if @p bytes == 0.
 *
 * @complexity O(1)
 */
ssize_t CBuffRead(cbuff_t *cbuff, void *dst, size_t bytes);

/**
 * @brief  Test whether the buffer is empty.
 *
 * @param  cbuff  Buffer to query.
 *
 * @retval 1  The buffer is empty (size == 0).
 * @retval 0  The buffer contains data.
 *
 * @complexity O(1)
 */
int CBuffIsEmpty(cbuff_t *cbuff);

/**
 * @brief  Return the number of free bytes currently available.
 *
 * @param  cbuff  Buffer to query.
 *
 * @return Free space in bytes (0 ≤ value ≤ capacity).
 *
 * @complexity O(1)
 */
size_t CBuffFreeSpace(cbuff_t *cbuff);

/**
 * @brief  Return the number of bytes currently stored.
 *
 * @param  cbuff  Buffer to query.
 *
 * @return Occupied size in bytes (0 ≤ value ≤ capacity).
 *
 * @complexity O(1)
 */
size_t CBuffSize(cbuff_t *cbuff);

#endif /* ILRD_CBUFF_H */



