#include "string.h"


int StrLen(const char *str)
{
	int len = 0;
	assert(str);
	while ('\0' != str[len])
	{
		len++;
	}
	return len;
}


int StrCmp(const char *s1, const char *s2)
{
    assert(s1);
    assert(s2);

    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }

    return (unsigned char)(*s1) - (unsigned char)(*s2);  /* To prevent any heb lang for example more than 127 limit */
}

