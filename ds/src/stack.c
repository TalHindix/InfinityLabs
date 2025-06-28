/********************
author: Tal Hindi
reviewer: Menachem Markovich 
status: Approved

*******************/


#include "../include/stack.h"

#include <assert.h>
#include <stddef.h> /* for size_t */
#include <stdlib.h> /* malloc , free */
#include <string.h> /* memcpy */


struct Stack
{
    size_t top;
    size_t element_size;
    size_t capacity;
    char *elements;
};

stack_t* StackCreate(size_t capacity, size_t element_size)
{
	stack_t* new_stack = NULL;
	char* buffer = NULL;
	
	if (0 == capacity || 0 == element_size )
	{
		return NULL;
	}
	
	new_stack = (stack_t*)malloc(sizeof(stack_t));
	
	if(NULL == new_stack) 
	{
		return NULL;
	}
	
	buffer = (char *)malloc(capacity * element_size);
	
	if (NULL == buffer)
	{
		free(new_stack);
		return NULL;
	}
	
	new_stack->top = 0;
	new_stack->element_size = element_size;
	new_stack->capacity = capacity;
	new_stack->elements = buffer;
	
	return new_stack;
	
}


void StackDestroy(stack_t* stack)
{
	if (NULL == stack)
	{
		return;
	}
	
	free(stack->elements);
	free(stack);
}


void StackPush(stack_t* stack, const void* element)
{
	char* byte_offset = NULL;

	assert(NULL != stack);
	assert(NULL != element);
	assert(stack->top < stack->capacity);
	
	byte_offset = stack->elements + (stack->top * stack->element_size); /* the offset in byte */
	
	memcpy(byte_offset,element,stack->element_size);
	++stack->top;
	
}


void StackPop(stack_t* stack)
{
	assert(NULL != stack);
    assert(stack->top > 0);
    
    --stack->top;
}


void* StackPeek(const stack_t* stack)
{
	assert(NULL != stack);
    assert(stack->top > 0);

    /* last valid slot is top-1 */                                     
    return (void *)(stack->elements + ((stack->top - 1) * stack->element_size));
}


size_t StackSize(const stack_t* stack)
{
	assert(NULL != stack);
    return stack->top;
}


int StackIsEmpty(const stack_t* stack)
{
	assert(NULL != stack);
    return (0 == stack->top);
}

size_t StackCapacity(const stack_t* stack)
{
	assert(NULL != stack);
    return stack->capacity;
}

