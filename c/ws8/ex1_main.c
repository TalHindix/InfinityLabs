/****************
author: Tal Hindi
reviewer: 
status:
*****************/

#include <stdio.h>
#include "ex1.h"

int main()
{
	element_t elements[ELEMENT_SIZE];

	/* Group A */
	int int_val_a = 4;
	float float_val_a = 4.5f;
	char *string_val_a = "chapter";

	/* Group B */
	int int_val_b = 8;
	float float_val_b = 8.5f;
	char *string_val_b = "hindi";

	InitInt(&elements[0], int_val_a);
	InitFloat(&elements[1], float_val_a);
	InitString(&elements[2], string_val_a);
	InitInt(&elements[3], int_val_b);
	InitFloat(&elements[4], float_val_b);
	InitString(&elements[5], string_val_b);

	printf("Before add:\n");
	PrintAll(elements, ELEMENT_SIZE);

	AddAll(elements, ELEMENT_SIZE);

	printf("\nAfter add:\n");
	PrintAll(elements, ELEMENT_SIZE);

	CleanUpAll(elements, ELEMENT_SIZE);

	return 0;
}

