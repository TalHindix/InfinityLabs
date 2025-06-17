/****************
author: Tal Hindi
reviewer: 
status:
*****************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ex1.h"

/************************OPERATIONS ***************************************/
static void PrintInt(element_t *element_ptr)
{
	printf("%d\n", *(int *)&(element_ptr->data));
}

static void AddInt(element_t *element_ptr)
{
	*(int *)&(element_ptr->data) += 10;
}

static void PrintFloat(element_t *element_ptr)
{
	printf("%f\n", *(float *)&(element_ptr->data));
}

static void AddFloat(element_t *element_ptr)
{
	*(float *)&(element_ptr->data) += 10.0f;
}

static void PrintString(element_t *element_ptr)
{
	printf("%s\n", (char *)element_ptr->data);
}

static void AddString(element_t *element_ptr)
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

static void CleanUpString(void *ptr_string)
{
	free(ptr_string);
}

static void NoCleanup(void *ptr)
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

void PrintAll(element_t *arr, size_t size)
{
	size_t i;
	for (i = 0; i < size; ++i)
	{
		arr[i].print(&arr[i]);
	}
}

void AddAll(element_t *arr, size_t size)
{
	size_t i;
	for (i = 0; i < size; ++i)
	{
		arr[i].add(&arr[i]);
	}
}

void CleanUpAll(element_t *arr, size_t size)
{
	size_t i;
	for (i = 0; i < size; ++i)
	{
		arr[i].cleanup(arr[i].data);
	}
}

