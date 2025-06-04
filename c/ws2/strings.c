#include <assert.h>
#include <stdlib.h>
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

    while ('\0' != (*dst = *src))
    {
        dst++;
        src++;
    }
    return start;
}

char *StrNCpy(char *dst, const char *src, size_t size)
{
    size_t i = 0;
    assert(dst && src);

    while ((i < size) && ('\0' != src[i]))
    {
        dst[i] = src[i];
        i++;
    }
    while (i < size)
    {
        dst[i] = '\0';
        i++;
    }
    return dst;
}

int StrNCmp(const char *s1, const char *s2, size_t n)
{
    size_t i = 0;
    assert(s1 && s2);

    while ((i < n) && s1[i] && s2[i])
    {
        if (s1[i] != s2[i])
        {
            return ((unsigned char)s1[i] - (unsigned char)s2[i]);
        }
        i++;
    }

    if (i < n)
    {
        return ((unsigned char)s1[i] - (unsigned char)s2[i]);
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

    return ((unsigned char)(*s1 ? *s1 : 0) - (unsigned char)(*s2 ? *s2 : 0));
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

    return StrCpy(copy, s);
}

char *StrCat(char *dst, const char *src)
{
    char *start = dst;
    assert(dst && src);

    while ('\0' != *dst)
    {
        dst++;
    }

    while ('\0' != (*dst = *src))
    {
        dst++;
        src++;
    }

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

    while ((0 != n) && ('\0' != *src))
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
    size_t i = 0;
    if ('\0' == *needle)
    {
        return (char *)haystack;
    }

    while ('\0' != *haystack)
    {
        i = 0;
        while (needle[i] && haystack[i] && (haystack[i] == needle[i]))
        {
            i++;
        }

        if ('\0' == needle[i])
        {
            return (char *)haystack;
        }

        haystack++;
    }

    return NULL;
}

size_t StrSpn(const char *s, const char *accept)
{
    size_t count = 0;
    const char *a;

    assert(s && accept);

    while ('\0' != *s)
    {
        a = accept;
        while ('\0' != *a && *a != *s)
        {
            a++;
        }

        if ('\0' == *a)
        {
            break;
        }

        count++;
        s++;
    }

    return count;
}

char *StrTok(char *input_str, const char *delimiters)
{
    static char *next_token = NULL;
    char *token_start;
    const char *delim_char;
    int is_delim;

    if (NULL != input_str)
    {
        next_token = input_str;
    }

    if (NULL == next_token)
    {
        return NULL;
    }

    while ('\0' != *next_token)
    {
        delim_char = delimiters;
        is_delim = 0;
        while ('\0' != *delim_char)
        {
            if (*delim_char == *next_token)
            {
                is_delim = 1;
                break;
            }
            delim_char++;
        }
        if (!is_delim)
        {
            break;
        }
        next_token++;
    }

    if ('\0' == *next_token)
    {
        next_token = NULL;
        return NULL;
    }

    token_start = next_token;

    while ('\0' != *next_token)
    {
        delim_char = delimiters;
        while ('\0' != *delim_char)
        {
            if (*delim_char == *next_token)
            {
                *next_token = '\0';
                next_token++;
                return token_start;
            }
            delim_char++;
        }
        next_token++;
    }

    next_token = NULL;
    return token_start;
}
