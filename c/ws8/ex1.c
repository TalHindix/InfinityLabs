#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define ELEMENT_SIZE 6

typedef struct element_s element_t;

struct element_s
{
	void *data;
	void (*print)(element_t *);
	void (*add)(element_t *);
	void (*cleanup)(void *);
};


/************************OPERATIONS ***************************************/
void PrintInt(element_t *element_ptr)
{
	printf("%d\n", *(int *)&(element_ptr->data));
}

void AddInt(element_t *element_ptr)
{
	*(int *)&(element_ptr->data) += 10;
}



void AddFloat(element_t *element_ptr)
{
	*(float *)&(element_ptr->data) += 10.0f;
}

void PrintFloat(element_t *element_ptr)
{
	printf("%f\n", *(float *)&(element_ptr->data));
}

void PrintString(element_t *element_ptr)
{
    printf("%s\n", (char *)element_ptr->data);
}

void AddString(element_t *element_ptr)
{
    char *str = (char *)element_ptr->data;
    char buf[12];
    char *tmp;
    size_t new_len;

    if (!str)
    {
        printf("AddString: NULL\n");
        return;
    }

    sprintf(buf, "%d", 10);
    new_len = strlen(str) + strlen(buf) + 1;
    tmp = realloc(str, new_len);
    if (!tmp)
    {
        printf("realloc failed\n");
        return;
    }

    strcat(tmp, buf);
    element_ptr->data = tmp;
}

void CleanUpString(void *ptr_string)
{
    free(ptr_string);
}

void NoCleanup(void *ptr)
{
	(void)ptr;
}


/*********************INIT Funcs ***********************************/

void InitInt(element_t *elem, int value)
{
	elem->data = NULL;
	*(int *)&(elem->data) = value;
	elem->print = PrintInt;
	elem->add = AddInt;
	elem->cleanup = NoCleanup;
}

void InitFloat(element_t *elem, float value)
{
	elem->data = NULL;
	*(float *)&(elem->data) = value;
	elem->print = PrintFloat;
	elem->add = AddFloat;
	elem->cleanup = NoCleanup;
}

void InitString(element_t *elem, const char *str)
{
    char *copy;

    if (!str)
    {
        elem->data = NULL;
        elem->print = NULL;
        elem->add = NULL;
        elem->cleanup = NULL;
        return;
    }

    copy = (char *)malloc(strlen(str) + 1);
    if (!copy)
    {
        elem->data = NULL;
        return;
    }

    strcpy(copy, str);
    elem->data = copy;
    elem->print = PrintString;
    elem->add = AddString;
    elem->cleanup = CleanUpString;
}


/************************UTIL ***************************************/

static void PrintAll(element_t *arr, size_t size)
{
	size_t i;
	for (i = 0; i < size; ++i)
	{
		arr[i].print(&arr[i]);
	}
}

static void AddAll(element_t *arr, size_t size)
{
	size_t i;
	for (i = 0; i < size; ++i)
	{
		arr[i].add(&arr[i]);
	}
}

static void CleanUpAll(element_t *arr, size_t size)
{
	size_t i;
	for (i = 0; i < size; ++i)
	{
		arr[i].cleanup(arr[i].data);
	}
}


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

