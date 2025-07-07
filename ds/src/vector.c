/****************
author: Tal Hindi
reviewer: Matan Shalom
status: Approved
****************/

#include <assert.h> 
#include <stdlib.h>
#include <string.h>
#include "../include/vector.h"

#define GROWTH_FACTOR (2)
#define SHRINK_THRESHOLD_DIVISOR (4)

struct Vector
{
	size_t size;
	size_t capacity;
	size_t element_size;
	char* buffer;
};


vector_t* VectorCreate(size_t init_capacity, size_t element_size)
{
    vector_t *vector = NULL;
    
	init_capacity = (init_capacity < DEFAULT_CAPACITY) ? DEFAULT_CAPACITY : init_capacity;
	
	vector = (vector_t *)malloc(sizeof(vector_t));
    
    if (NULL == vector)
    {
        return NULL;
    }
	
	
    vector->buffer = (char *)malloc(init_capacity * element_size);
    if (NULL == vector->buffer)
    {
        free(vector);
        return NULL;
    }

    vector->size = 0;
    vector->capacity = init_capacity;
    vector->element_size = element_size;

    return vector;
}

void VectorDestroy(vector_t* vector)
{
	assert(NULL != vector);
   
    free(vector->buffer);
    vector->buffer = NULL;
    free(vector);
}

int VectorPushBack(vector_t* vector ,const void* element)
{
	assert(NULL != vector);
	assert(NULL != vector->buffer);
	assert(NULL != element);
	
	if(vector->size == vector->capacity)
	{
		vector->capacity *= GROWTH_FACTOR;
				
		if (VectorReserve(vector,vector->capacity))
		{
			return -1;
		}
	}	
	memcpy(vector->buffer + (vector->size * vector->element_size), element, vector->element_size);
  	++vector->size;
	
	return 0;
}


void VectorPopBack(vector_t* vector)
{
	assert(NULL != vector);
	assert(NULL != vector->buffer);
	assert(0 < vector->size);

	if (vector->capacity / SHRINK_THRESHOLD_DIVISOR == vector->size)
	{
		vector->capacity /= GROWTH_FACTOR;
		VectorReserve(vector, vector->capacity);
	}
	--vector->size;
	
	return;

}

void* VectorGetAccessToElement(vector_t* vector ,size_t index)
{
	assert(NULL != vector);
	assert(NULL != vector->buffer);

	return (void*)(vector->buffer + (index * vector->element_size));
}

size_t VectorSize(const vector_t* vector)
{
    assert(vector);
    return vector->size;
}


size_t VectorCapacity(const vector_t* vector)
{
    assert(vector);
    return vector->capacity;
}


int VectorReserve(vector_t* vector ,size_t new_capacity ) /* if (new_capacity < size) ----> shrink*/
{
	char* new_buffer = NULL;
	
	new_capacity = (new_capacity < DEFAULT_CAPACITY) ? DEFAULT_CAPACITY : new_capacity;
 
    new_buffer = (char*)realloc(vector->buffer, new_capacity * vector->element_size);
        
    if (NULL == new_buffer)
    {
    	VectorDestroy(vector);
        return -1;
    }
       
    vector->buffer = new_buffer;
    vector->capacity = new_capacity;

    return 0;	
}


void VectorShrink(vector_t* vector)
{
	assert(NULL != vector);
	
	VectorReserve(vector, vector->size);
	
	return;
}

