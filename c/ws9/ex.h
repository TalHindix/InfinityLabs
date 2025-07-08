#ifndef _EX_H_
#define _EX_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>   /* For isspace, isdigit, isalpha, toupper */
#include <limits.h>  /* For LONG_MAX and LONG_MIN */

char* Itoa10(int number, char *buf);

int Atoi10(const char *s);

long AtoiBase(const char *s, unsigned int base);

#endif 

