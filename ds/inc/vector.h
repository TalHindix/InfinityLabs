#ifndef __ILRD_VECTOR__
#define __ILRD_VECTOR__


typedef struct Vector vector_t;

#include <stddef.h>

#define DEFAULT_CAPACITY (8)

/* Create the dynamic vector O(1)*/
vector_t* VectorCreate(size_t init_capacity ,size_t element_size );

/* Destroy the dynamic vector O(1) */
void VectorDestroy(vector_t* vector);

/* Push a new elememt to the top of the vector O(1) */
int VectorPushBack(vector_t* vector ,const void* element);

/* Push a new elememt to the top of the vector, if size/capacity <= 0.25 ---> reserve capacity/2 O(1) */
void VectorPopBack(vector_t* vector);

/* Add/return an element in a desired index O(1) */
void* VectorGetAccessToElement(vector_t* vector ,size_t index);

/* Return the current vector size O(1) */
size_t VectorSize(const vector_t* vector);

/* Return the current vector capacity O(1) */
size_t VectorCapacity(const vector_t* vector);

/* Reserve memory for vector elements O(n)*/
int VectorReserve(vector_t* vector ,size_t new_capacity );  /* if (new_capacity < size) ----> shrink*/

/* Shrink the vector capacity to its size O(1) */
void VectorShrink(vector_t* vector); 


#endif /* __ILRD_VECTOR__ */
