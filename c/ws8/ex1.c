/******************
author: Tal Hindi
reviewer: 
status: 
*******************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ELEMENT_SIZE 6

typedef struct 
{
	void *data;
	void (*print)(const void *);
    void (*add)(void *);
    void (*cleanup)(void *);
} element_t;


/************************OPERATIONS ***************************************/
void PrintInt(const void *ptr_int)
{
	printf("%d\n", *(const int *)ptr_int);
}

void AddInt(void *ptr_int)
{
	*(int *)ptr_int += 10;
}

void PrintFloat(const void *ptr_float)
{
	printf("%f\n", *(const float *)ptr_float);
}

void AddFloat(void *ptr_float)
{
	*(float *)ptr_float += 10.0;
}

void PrintString(const void *ptr_string)
{
    printf("%s\n", *(const char * const *)ptr_string);
}


void AddString(void *ptr_string)
{
    char  **pp   = (char **)ptr_string;
    char   *str  = *pp;
    char    buf[12];
    size_t need;
    char *tmp;

    if (!str)
    {
    	printf("AddString: NULL");
    	return;
    }

    sprintf(buf, "%d", 10);

    need = strlen(str) + strlen(buf) + 1;
    tmp = realloc(str, need);
    
    if (!tmp) 
    {
    	printf("realloc failed");
    	return;
    }

    strcat(tmp, buf);
    *pp = tmp;  
}

void CleanUpString(void *ptr_string)
{
    char **pp = (char **)ptr_string;
    free(*pp);   
    free(pp);
}

void NoCleanup(void *ptr)
{
    (void)ptr; 
}


/*********************INIT Funcs ***********************************/

void InitInt(element_t *elem, int *value)
{
    elem->data = value;
    elem->print = PrintInt;
    elem->add = AddInt;
    elem->cleanup = NoCleanup;
}

void InitFloat(element_t *elem, float *value)
{
    elem->data = value;
    elem->print = PrintFloat;
    elem->add = AddFloat;
    elem->cleanup = NoCleanup;
}

void InitString(element_t *elem, const char *str)
{
	char *copy;
	char **holder;
	
    if (!str)
    {
        elem->data = NULL;
        elem->print = NULL;
        elem->add = NULL;
        elem->cleanup = NULL;
        return;
    }
	
	/* we can use strdup() */
    copy = (char *)malloc(strlen(str) + 1);
    
    if (!copy)
    {
        return;
    }

    strcpy(copy, str);
    
    holder = (char **)malloc(sizeof(char *));
    
    if (!holder)
    {
    	free(copy);
    	return;
    }
    
    *holder = copy;
    
	elem->data = holder;
    elem->print = PrintString;
    elem->add = AddString;   
    elem->cleanup = CleanUpString;
}


/************************UTIL ***************************************/

static void PrintAll(const element_t *arr, size_t size)
{
    size_t i;
    for (i = 0; i < size; ++i)
    {
        arr[i].print(arr[i].data);
    }
}

static void AddAll(element_t *arr, size_t size)
{
    size_t i;
    for (i = 0; i < size; ++i)
    {
        arr[i].add(arr[i].data);
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
    
    InitInt(&elements[0], &int_val_a);
    InitFloat(&elements[1], &float_val_a);
    InitString(&elements[2], string_val_a);
    InitInt(&elements[3], &int_val_b);
    InitFloat(&elements[4], &float_val_b);
    InitString(&elements[5], string_val_b);
	
    
    
    
    

    printf("Before add:\n");
    PrintAll(elements, ELEMENT_SIZE);

    AddAll(elements, ELEMENT_SIZE);

    printf("\nAfter add:\n");
    PrintAll(elements, ELEMENT_SIZE);

    CleanUpAll(elements, ELEMENT_SIZE);
    


    return 0;
}
