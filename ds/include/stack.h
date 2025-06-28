#ifndef _ILRD_STACK_
#define _ILRD_STACK_



typedef struct Stack stack_t;

/******************************************************************************
 * Function:   StackCreate
 *
 * Purpose:    Creates a new stack with a given capacity and element size.
 *
 * Params:     size_t capacity       – number of elements the stack can hold
 *             size_t element_size   – size of each element in bytes
 *
 * Returns:    stack_t* – pointer to the new stack, or NULL if failed
 *
 * Complexity: O(1)
 ******************************************************************************/
stack_t* StackCreate(size_t capacity, size_t element_size);

/******************************************************************************
 * Function:   StackDestroy
 *
 * Purpose:    Frees all memory used by the stack.
 *
 * Params:     stack_t* stack – pointer to the stack to destroy
 *
 * Returns:    void
 *
 * Complexity: O(1)
 ******************************************************************************/
void StackDestroy(stack_t* stack);

/******************************************************************************
 * Function:   StackPush
 *
 * Purpose:    Pushes an element onto the top of the stack.
 *
 * Params:     stack_t* stack      – pointer to the stack
 *             const void* element – pointer to the element to push
 *
 * Returns:    void
 *
 * Complexity: O(1)
 ******************************************************************************/
void StackPush(stack_t* stack, const void* element);

/******************************************************************************
 * Function:   StackPop
 *
 * Purpose:    Removes the top element from the stack.
 *
 * Params:     stack_t* stack – pointer to the stack
 *
 * Returns:    void
 *
 * Complexity: O(1)
 ******************************************************************************/
void StackPop(stack_t* stack);

/******************************************************************************
 * Function:   StackPeek
 *
 * Purpose:    Returns a pointer to the top element without removing it.
 *
 * Params:     const stack_t* stack – pointer to the stack
 *
 * Returns:    void* – pointer to the top element
 *
 * Complexity: O(1)
 ******************************************************************************/
void* StackPeek(const stack_t* stack);

/******************************************************************************
 * Function:   StackSize
 *
 * Purpose:    Returns the number of elements currently in the stack.
 *
 * Params:     const stack_t* stack – pointer to the stack
 *
 * Returns:    size_t – number of elements in the stack
 *
 * Complexity: O(1)
 ******************************************************************************/
size_t StackSize(const stack_t* stack);

/******************************************************************************
 * Function:   StackIsEmpty
 *
 * Purpose:    Checks if the stack is empty.
 *
 * Params:     const stack_t* stack – pointer to the stack
 *
 * Returns:    int – 1 if empty, 0 if not
 *
 * Complexity: O(1)
 ******************************************************************************/
int StackIsEmpty(const stack_t* stack);

/******************************************************************************
 * Function:   StackCapacity
 *
 * Purpose:    Returns the maximum number of elements the stack can hold.
 *
 * Params:     const stack_t* stack – pointer to the stack
 *
 * Returns:    size_t – total capacity of the stack
 *
 * Complexity: O(1)
 ******************************************************************************/
size_t StackCapacity(const stack_t* stack);

#endif /* _ILRD_STACK_ */
