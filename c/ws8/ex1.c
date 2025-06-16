#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STRING_MAX_SIZE 250

typedef struct 
{
	void *data;
	void (*print)(const void *);
    void (*add)(void *);
    void (*cleanup)(void *);
} element_t;

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
    printf("%s\n", (const char *)ptr_string);
}

void AddString(void *ptr_string)
{
    char *str = (char *)ptr_string;
    char num_str[12];
    size_t current_len;

    sprintf(num_str, "%d", 10);
    current_len = strlen(str);
    
    if (current_len + strlen(num_str) + 1 > STRING_MAX_SIZE)
    {
    	printf("error, not enough space in string");
    	return;
    }
   
    
    strcat(str, num_str);
}

void CleanUpString(void *ptr_string)
{
    free(ptr_string);
}


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

void NoCleanup(void *ptr)
{
    (void)ptr; 
}

int main()
{
	element_t elements[6];
	int data1 = 4;
    float data2 = 4.5f;
    char *data3 = (char *)malloc(STRING_MAX_SIZE);   
    int data4 = 8;
    float data5 = 8.5f;
    char *data6 = (char *)malloc(STRING_MAX_SIZE);
    if (!data6)
    {
        printf("malloc error\n");
        return 1;
    }
    if (!data3)
    {
        printf("malloc error\n");
        return 1;
    }
    strcpy(data3, "chapter");
    strcpy(data6, "hindi");
    
    
    elements[0].data    = &data1;
	elements[0].print   = PrintInt;
	elements[0].add     = AddInt;
	elements[0].cleanup = NoCleanup;

	elements[1].data    = &data2;
	elements[1].print   = PrintFloat;
	elements[1].add     = AddFloat;
	elements[1].cleanup = NoCleanup;

	elements[2].data    = data3;
	elements[2].print   = PrintString;
	elements[2].add     = AddString;
	elements[2].cleanup = CleanUpString;
	
	elements[3].data    = &data4;
	elements[3].print   = PrintInt;
	elements[3].add     = AddInt;
	elements[3].cleanup = NoCleanup;

	elements[4].data    = &data5;
	elements[4].print   = PrintFloat;
	elements[4].add     = AddFloat;
	elements[4].cleanup = NoCleanup;

	elements[5].data    = data6;
	elements[5].print   = PrintString;
	elements[5].add     = AddString;
	elements[5].cleanup = CleanUpString;


    printf("Before add:\n");
    PrintAll(elements, 6);

    AddAll(elements, 6);

    printf("\nAfter add:\n");
    PrintAll(elements, 6);

    CleanUpAll(elements, 6);

    return 0;
}
