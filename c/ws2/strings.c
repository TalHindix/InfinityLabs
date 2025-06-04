#include <assert.h>       /* for assertion */
#include <stdlib.h>       /* for malloc() */
#include "string.h"


size_t StrLen(const char *str)
{
    size_t len = 0;
    assert(str);

    while ('\0' != str[len])
    {
        len++;
    }
    return len;
}

int StrCmp(const char *s1, const char *s2)
{
    assert(s1 && s2);

    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}

char *StrCpy(char *dst, const char *src)
{
    char *start = dst;
    assert(dst && src);

    while ('\0' != *src)
    {
    	*dst = *src;
        dst++;
        src++;
    }
    *dst = '\0';
    
    return start;
}

char *StrNCpy(char *dst, const char *src, size_t size)
{
    char *start = dst;
    assert(dst && src);

    while ((size > 0) && ('\0' != *src))
    {
    	*dst = *src;
        dst++;
        src++;
        size--;
    }
    while (size > 0)
    {
        *dst = '\0';
        dst++;
        size--;
    }
    
    return start;
}

int StrNCmp(const char *s1, const char *s2, size_t n)
{
    assert(s1 && s2);

    while ((n>0) && ('\0' != *s1) && ('\0' != *s2))
    {
        if (*s1 != *s2)
        {
            return ((unsigned char)(*s1) - (unsigned char)(*s2));
        }
        
        s1++;
        s2++;
        n--;
    }

    if (n > 0)
    {
        return ((unsigned char)(*s1) - (unsigned char)(*s2));
    }

    return 0;
}

int StrCaseCmp(const char *s1, const char *s2)
{
    unsigned char c1, c2;
    assert(s1 && s2);

    while (*s1 && *s2)
    {
        c1 = (*s1 >= 'A' && *s1 <= 'Z') ? (*s1 + 32) : *s1;
        c2 = (*s2 >= 'A' && *s2 <= 'Z') ? (*s2 + 32) : *s2;

        if (c1 != c2)
        {
            return (c1 - c2);
        }

        s1++;
        s2++;
    }
	
	c1 = (*s1 >= 'A' && *s1 <= 'Z') ? (*s1 + 32) : *s1;
    c2 = (*s2 >= 'A' && *s2 <= 'Z') ? (*s2 + 32) : *s2;
	
	return ((unsigned char)c1 - (unsigned char)c2);
}

char *StrChr(const char *s, int c)
{
    assert(s);

    while ('\0' != *s)
    {
        if ((char)c == *s)
        {
            return (char *)s;
        }
        s++;
    }

    if ('\0' == (char)c)
    {
        return (char *)s;
    }

    return NULL;
}

char *StrDup(const char *s)
{
    char *copy;
    size_t len;
    assert(s);

    len = StrLen(s);
    copy = (char *)malloc(len + 1);
    if (NULL == copy)
    {
        return NULL;
    }
    
    StrCpy(copy, s);
    
    return copy;
}

char *StrCat(char *dst, const char *src)
{
    char *start = dst;
    assert(dst && src);

    while ('\0' != *dst)
    {
        dst++;
    }

    while ('\0' != *src)
    {
  		*dst = *src;
        dst++;
        src++;
    }
    *dst = '\0';

    return start;
}

char *StrNCat(char *dest, const char *src, size_t n)
{
    char *start = dest;
    assert(dest && src);

    while ('\0' != *dest)
    {
        dest++;
    }

    while ((n > 0) && ('\0' != *src))
    {
        *dest = *src;
        dest++;
        src++;
        n--;
    }

    *dest = '\0';
    return start;
}

char *StrStr(const char *haystack, const char *needle)
{
   const char *h, *n;
   
   if ('\0' == *needle)
   {
   		return (char *)haystack;
   }
   
   while ('\0' != *haystack)
    {
        h = haystack;
        n = needle;

        while (*h && *n && (*h == *n))
        {
            h++;
            n++;
        }

        if (*n == '\0')
        {
            return (char *)haystack;
        }

        haystack++;
    }
    
    return NULL;
}

size_t StrSpn(const char *str, const char *accept)
{
    size_t count = 0;
    const char *curr_accept;

    assert(str && accept);

    while ('\0' != *str)
    {
        curr_accept = accept;

        while (*curr_accept && *curr_accept != *str)
        {
            curr_accept++;
        }

        if ('\0' == *curr_accept)
        {
            break;
        }

        count++;
        str++;
    }

    return count;
}



char *StrTok(char *str, const char *delimiters)
{
    static char *next = NULL;
    char *start;

    if (str != NULL)
    {
        next = str;
    }

    if (next == NULL)
    {
        return NULL;
    }

    while (*next)
    {
        const char *d = delimiters;
        while (*d && *d != *next)
        {
            d++;
        }

        if ('\0' == *d)
        {
            break;
        }

        next++;
    }

    if ('\0' == *next)
    {
        next = NULL;
        return NULL;
    }

    start = next;

  
    while (*next)
    {
        const char *d = delimiters;
        while (*d)
        {
            if (*d == *next)
            {
                *next = '\0';
                next++;
                return start;
            }
            d++;
        }
        next++;
    }

    next = NULL;
    return start;
}

