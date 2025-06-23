#ifndef EX_H
#define EX_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>   /* For isspace, isdigit, isalpha, toupper */
#include <limits.h>  /* For LONG_MAX and LONG_MIN */

void Reverse(char buf[], size_t len);

char* Itoa10(int number, char buf[]);

int Atoi10(const char *s);

long AtoiBase(const char *s, int base);
#endif 

