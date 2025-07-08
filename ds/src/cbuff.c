/**************************************
Exercise: 	DS - Circuler Buffer
Date:		7/7/2025
Developer:	Tal Hindi
Reviewer: 	Baruch Haimson
Status:		Approved
**************************************/

#include "cbuff.h" 		/* CBuffCreate() 			*/
#include <sys/types.h> 	/* ssize_t 					*/
#include <stdlib.h> 	/* malloc() free() size_t 	*/
#include <assert.h> 	/* assert 					*/
#include <string.h> 	/* memcpy 					*/

#define MIN(x,y) ((x) > (y) ? (y) : (x))

struct cbuff
{
    size_t front;
    size_t size;
    size_t capacity;
    char buffer[1];
};



cbuff_t* CBuffCreate(size_t capacity)
{
	cbuff_t* cbuff = NULL;
	
	capacity = (DEFAULT_SIZE < capacity) ? capacity : DEFAULT_SIZE;
	
	cbuff = (cbuff_t*)malloc(sizeof(cbuff_t) + (capacity - DEFAULT_SIZE));
	
	if(NULL == cbuff)
	{
		return NULL;
	}
	
	cbuff->capacity = capacity;
	cbuff->size = 0;
	cbuff->front = 0;
	
	return cbuff;
}

void CBuffDestroy(cbuff_t* cbuff)
{
	assert(cbuff);
	
	free(cbuff);	

	cbuff = NULL;
}

ssize_t CBuffWrite(cbuff_t* cbuff, const void* src, size_t bytes)
{
	size_t bytes_to_write = 0;
	size_t write_index    = 0;
	size_t first_chunk    = 0;
	size_t second_chunk   = 0;
	
	assert(cbuff);
	assert(src);
	
	if (NULL == src)
	{
		return -1;	
	}
		
	bytes_to_write = MIN(bytes, CBuffFreeSpace(cbuff));
	
	if (0 == bytes_to_write)
	{
		return 0;
	}
	
	write_index  = (cbuff->front + cbuff->size) % cbuff->capacity;
	first_chunk  = MIN(bytes_to_write, cbuff->capacity - write_index);
	
	memcpy(cbuff->buffer + write_index, src, first_chunk);
	
	second_chunk = bytes_to_write - first_chunk;
	if (second_chunk > 0)
	{
		memcpy(cbuff->buffer, (const char*)src + first_chunk, second_chunk);
	}
	
	cbuff->size += bytes_to_write;
	
	return (ssize_t)bytes_to_write;
}

ssize_t CBuffRead(cbuff_t* cbuff, void* dst, size_t bytes)
{
	size_t available = 0;
	size_t chunk     = 0;

	assert(cbuff);
	assert(dst);
	
	if (NULL == dst)
	{
		return -1;
	}
	
	available = cbuff->size;
	
	if (0 == available)
	{
		return 0;
	}
	
	if (bytes > available)
	{
		bytes = available;
	}
	
	if (0 == bytes)
	{
		return 0;
	}
	
	chunk = MIN(bytes, cbuff->capacity - cbuff->front);
	
	memcpy(dst, cbuff->buffer + cbuff->front, chunk);
	
	if (bytes != chunk)
	{
		memcpy((char *)dst + chunk, cbuff->buffer, bytes - chunk);
	}
		
	cbuff->front = (cbuff->front + bytes) % cbuff->capacity;
	
	cbuff->size -= bytes;
	
	return (ssize_t)bytes;
}


int CBuffIsEmpty(cbuff_t* cbuff)
{
	assert(cbuff);
	
	return (CBuffSize(cbuff) == 0);
}


size_t CBuffFreeSpace(cbuff_t* cbuff)
{
	assert(cbuff);
	
	return cbuff->capacity - cbuff->size;
}

size_t CBuffSize(cbuff_t* cbuff)
{
	assert(cbuff);
	
	return cbuff->size;
}
