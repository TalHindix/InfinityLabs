/******************
author: Tal Hindi
reviewer: 
status: 
*******************/

#include <stdio.h>


#define SIZEOF_TYPE(type) \
    ((size_t)( \
        (unsigned char *)((type *)NULL + 1) - \
        (unsigned char *)((type *)NULL) \
    ))

#define PRINTF_SIZEOF_TYPE(type) \
    printf("Sizeof %s is: %lu\n", #type, SIZEOF_TYPE(type))

typedef struct {
    int id;
    char name[20];
    double value;
} element_t;


int main()
{

	PRINTF_SIZEOF_TYPE(int);
    PRINTF_SIZEOF_TYPE(double);
    PRINTF_SIZEOF_TYPE(element_t);
	return 0;
}
