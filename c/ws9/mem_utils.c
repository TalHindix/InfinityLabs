/**********************
author: Tal Hindi
reviewer: 
status: 

**********************/

#include "mem_utils.h"
#include <assert.h>

/* Helper function BuildPattern */
static unsigned long BuildPattern(int c)
{
	unsigned char byte = (unsigned char)c;
	unsigned long pattern = 0;
	size_t i;
	
	for(i = 0; i < sizeof(unsigned long); ++i)
	{
		pattern = pattern << 8; /* Clear space for the next byte */
		pattern = pattern | byte;
	}
	
	return pattern;
}

void *MemSet(void *s, int c, size_t n)
{
	unsigned char *byte_ptr; /* write one byte at a time */
	unsigned long *word_ptr; /* write word by word */
	size_t word_size; /* size of a word on this cpu */
	size_t i; /* for loop */
	unsigned long pat; /* word-sizeed reapting pattern */
	
	byte_ptr = (unsigned char *)s;
	word_size = sizeof(size_t); /* 8 */
	
	/* Byte by Byte until the word-aligned */
	
	while (((size_t)byte_ptr & (word_size - 1)) && n)
	{
		*byte_ptr = (unsigned char)c;
		++byte_ptr;
		--n;
	}
	
	
	/* Word at a time fill */
	
	if ( n > word_size ) /* */
	{
		pat = BuildPattern(c); /* if c=0x5A and sizeof(unsigned long) = 8 then: pat = 0x5A5A5A5A5A5A5A5A */

		word_ptr = (unsigned long *)byte_ptr; /* the pointer byte_ptr(pointing to destination memory as bytes) is cast to a unsigned long *, allowing word-sized writes*/
		
		/* This loop writes full word_size chunks to memory using the helper func - pat() */  /* n / word_size give the number of full words we can fit.*/
		for(i = n / word_size; i; --i) 
		{
			*word_ptr = pat; /* to fill each word */
			++word_ptr; /* move the pointer forward */
		}
		
		byte_ptr = (unsigned char *)word_ptr; /* byte_ptr is updated to point to the remaining byte area (if any) after all the word writes. */
		n = n % word_size;  /* sets n to the number of leftover bytes (less than word_size) that still need to be written byte by byte afterward */
				
	}
	
	/* Tail Bytes */
	while (n) /* In this point n less than word_size */
	{
		*byte_ptr = (unsigned char)c; 
		++byte_ptr;
		--n;
	}
	
	return s;
	
}










void *MemCpy(void *dest, const void *src, size_t byte_count)
{
	unsigned char *dest_byte = (unsigned char *)dest; /* to "WALK" byte byte on dest */
	const unsigned char *src_byte = (const unsigned char *)src; /* to "WALK" byte byte on src */
	
	const size_t word_size = sizeof(size_t); /* CHUNCK size */
	size_t i = 0; /* for loop */
	size_t word_to_copy = 0;
	size_t *dest_word; /* to "Walk" word word on dest */
	const size_t *src_word; /* to "Walk" word word on src */
	
	assert (NULL != dest && NULL != src);
	
	if ( byte_count == 0 || src == dest )  /* If the number of byte that we need to copy is zero  OR the pointers are equal - CHECK */
	{
		return dest;
	}
	
	while ( byte_count > 0 && (((sizeof(dest_byte) % word_size != 0) || (sizeof(src_byte) % word_size != 0))))
	{
		*dest_byte++ = *src_byte++;
		--byte_count;
	}
	
	dest_word = (size_t *)dest_byte;
	src_word = (const size_t *)src_byte;
	
	word_to_copy = byte_count / word_size;
	
	for(i = 0; i < word_to_copy; ++i)
	{
		dest_word[i] = src_word[i];
	}
	
	
	dest_byte = (unsigned char *)(dest_word + word_to_copy);
	src_byte = (unsigned char *)(src_word + word_to_copy);
	
	byte_count %=  word_size;
	
	while (byte_count > 0)
	{
		*dest_byte++ = *src_byte++;
		--byte_count;
	}
	
	return dest;
}


void *MemMove(void *dest, const void *src, size_t byte_count)
{

	/* Make it easy to move byte-by-byte */
	unsigned char *dest_byte = (unsigned char *)dest;
	const unsigned char *src_byte = (const unsigned char *)src; 
		
	assert( NULL != dest && NULL != src );
	
	if ( (dest == src) || (0 == byte_count) )
	{
		return dest;
	}
	
	/* If dest is before src, or far enough from src */
	if ( (dest_byte < src_byte) || (dest_byte >= src_byte + byte_count) ) /* */
	{
		/* copy from start to end */
		while ( byte_count > 0 )
		{
			*dest_byte++ = *src_byte++;
			--byte_count;		
		}	
		
	}
	else /* Overlapping case */
	{
		/* Move Pointers to the end */
		dest_byte += byte_count;
		src_byte += byte_count;
		
		/* Copy from end to start */
		while (byte_count > 0)
		{
			--dest_byte;
			--src_byte;
			*dest_byte = *src_byte;
			--byte_count;
		}	

	}
	
	return dest;
}


