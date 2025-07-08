/**************************************
Exercise: 	DS - DLL
Date:		8/7/2025
Developer:	Tal Hindi
Reviewer: 	Menny Markovich
Status:		
**************************************/

#include "dll.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h> /* malloc */




int main(void)
{
	dll_t* dll = DLLCreate();
	DLLDestroy(dll);
	return 0;
}
