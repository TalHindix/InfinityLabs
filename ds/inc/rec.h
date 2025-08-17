/**************************************
Exercise:   DS - Recursion
Date:       17/8/2025
Developer:  Tal Hindi
Reviewer:   Avi Tobar
Status:     
**************************************/

#include <stddef.h> /* size_t */

#include "stack.h" /* StackCreate */

typedef struct node
{
    int data;
    struct node* next;
}node_t;


int Fibonacci(size_t element_index);

int RecFibonacci(size_t element_index);

int RecFibonacciMemo(size_t n);

node_t* FlipNode(node_t* head);

size_t StrLenRec(const char* str);

int StrCmpRec(const char* str1, const char* str2);

char* StrStrRec(const char* haystack, const char* needle);

stack_t* SortedStack(stack_t* s);



