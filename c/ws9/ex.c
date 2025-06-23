/****************
author: Tal Hindi
reviewer: 
status:

****************/

#include "ex.h"



/*  Helper Function to Check Overflow  */
int WillOverflow(long result, int digit, int base, int sign)
{
    if (sign > 0)
    {
        return result > (LONG_MAX - digit) / base;
    }
    else
    {
        return result < (LONG_MIN + digit) / base;
    }
}

/*  Helper Function to Reverse  */
void Reverse(char buf[], size_t len)
{
    size_t i = 0;
    size_t j = len - 1;
    while (i < j) 
    {
        char tmp = buf[i];
        buf[i++] = buf[j];
        buf[j--] = tmp;
    }
}

/* Int To String */
char* Itoa10(int number, char *buf)
{
	size_t i = 0;
	int sign = number;
	
	
	if (0 == number)
	{
		buf[i] = '0';
		++i;
	}
	else
	{
		if (number < 0)
		{
			number = -number;
		}
		
		while (number > 0)
		{
			buf[i] = '0' + (number % 10); /* cat */
			++i;
			number /= 10;
		}
		
	}
	
	if (sign < 0)
	{
		buf[i] = '-';
		++i;
	}
	
	buf[i] = '\0';
	Reverse(buf,i);
	return buf;
}

/* String to INT - Base 10 */
int Atoi10(const char* s)
{

	int sign = 1;
	int result = 0;

	if (s == NULL)
	{
		return 0;
	}	
	
	/* Move forward until no space */
	while (' ' == *s)
	{
		++s;
	}
	
	if	('-' == *s)
	{
		sign = -1;
		++s;
	}
	else if ('+' == *s)
	{
		++s;
	}
	
	
	while (*s >= '0' && *s <= '9')
	{
		int digit = *s - '0';
		result = result * 10 + digit;
		++s; 
	}

	return result * sign;

}

/* String to long Base 2-36 */
long AtoiBase(const char *text, int base)
{
    long result = 0;
    int sign = 1;
    int digit = 0;
    int has_digits = 0;

    if (text == NULL || base < 2 || base > 36)
    {
        return 0;
    }

    /* Skip leading spaces */
    while (isspace((unsigned char)*text))
    {
        ++text;
    }

    /* Handle sign */
    if ('-' == *text)
    {
        sign = -1;
        ++text;
    }
    else if ('+' == *text)
    {
        ++text;
    }

    /* Read characters */
    while (*text)
    {
        char ch = *text;

        if (isdigit((unsigned char)ch))
        {
            digit = ch - '0';
        }
        else if (isalpha((unsigned char)ch))
        {
            digit = toupper((unsigned char)ch) - 'A' + 10;
        }
        else
        {
            break;
        }

        if (digit >= base)
        {
            break;
        }

        /* Check overflow */
        if (WillOverflow(result, digit, base, sign))
        {
            return (sign > 0) ? LONG_MAX : LONG_MIN;
        }

        result = result * base + (sign * digit);
        has_digits = 1;

        ++text;
    }

    if (!has_digits)
    {
        return 0;
    }

    return result;
}


