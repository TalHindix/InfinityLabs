/****************
author: Tal Hindi
reviewer: 
status:
*****************/

#ifndef EX1_H
#define EX1_H

#include <stddef.h>

#define ELEMENT_SIZE 6

typedef struct element_s element_t;

struct element_s
{
	void *data;
	void (*print)(element_t *);
	void (*add)(element_t *);
	void (*cleanup)(void *);
};

/* Init functions */
void InitInt(element_t *elem, int value);
void InitFloat(element_t *elem, float value);
void InitString(element_t *elem, const char *str);

/* Utility functions */
void PrintAll(element_t *arr, size_t size);
void AddAll(element_t *arr, size_t size);
void CleanUpAll(element_t *arr, size_t size);

#endif /* EX1_H */

