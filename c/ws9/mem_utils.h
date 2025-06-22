#ifndef __MEM_UTILS_H__
#define __MEM_UTILS_H__


#include <stddef.h> /* size_t */


void *MemSet(void *s, int c, size_t n);
void *MemCpy(void *dest, const void *src, size_t n);
void *MemMove(void *dest, const void *src, size_t n);




#endif
