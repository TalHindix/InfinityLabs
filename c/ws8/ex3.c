/******************
author: Tal Hindi
reviewer: 
status: 
*******************/

#include <stdio.h> /* printf() */
#include <stddef.h> /* size_t */

#define SIZEOF_VAR(X) ((size_t)((unsigned char *)(&X + 1) - (unsigned char *)(&X)))

#define PRINT_SIZEOF(var) \
    printf("Sizeof %s is: %lu\n", #var, SIZEOF_VAR(var))

typedef enum
{
	Male,
	Female,
	Other
} Gender;

typedef struct {
	char* name;
	int age;
	Gender gender;
} Person;

int main()
{
	int x = 5;
	Person tal = {"tal",25,Male};
	
	PRINT_SIZEOF(x);
	PRINT_SIZEOF(tal);

	return 0;
}
