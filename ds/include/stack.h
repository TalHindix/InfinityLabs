#ifndef _ILRD_STACK_
#define _ILRD_STACK_

typedef struct Stack stack_t;


stack_t* StackCreate(size_t capacity, size_t element_size);
void StackDestroy(stack_t* stack);
void StackPush(stack_t* stack, const void* element);
void StackPop(stack_t* stack);
void* StackPeek(const stack_t* stack);
size_t StackSize(const stack_t* stack);
int StackIsEmpty(const stack_t* stack);
size_t StackCapacity(const stack_t* stack);

#endif /* _ILRD_STACK_ */
