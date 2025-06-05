#ifndef __STRINGS_H__
#define __STRINGS_H__

#include <assert.h> /* assert */
#include <stdlib.h>
#include <stddef.h>

/**********
 Exercise: 	WS2
 Date: 		05/06/25
 Developer: Tal Hindi
 Reviewer: 	Baruch Haimson
 Status: 	Approved
 **********/


/* Returns the length of the string 'str' (excluding the null terminator). */
size_t StrLen(const char *str);

/* Compares strings 's1' and 's2'. Returns 0 if equal, negative if s1 < s2, positive if s1 > s2. */
int StrCmp(const char *s1, const char *s2);

/* Copies string from 'src' to 'dst'. Returns pointer to 'dst'. */
char* StrCpy(char *dst, const char *src);

/* Copies up to 'size' characters from 'src' to 'dst'. Returns pointer to 'dst'. */
char* StrNCpy(char *dst, const char *src, size_t size);

/* Compares up to 'n' characters of 's1' and 's2'. Returns as in StrCmp. */
int StrNCmp(const char *s1, const char *s2, size_t n);

/* Compares strings 's1' and 's2', ignoring case. Returns as in StrCmp. */
int StrCaseCmp(const char *s1, const char *s2);

/* Returns pointer to first occurrence of character 'c' in string 's', or NULL if not found. */
char* StrChr(const char *s, int c);

/* Allocates memory and duplicates the string 's'. Returns a pointer to the new string, or NULL on failure. */
char* StrDup(const char *s);

/* Appends the string 'src' to the end of 'dst'. 'dst' must have enough space. Returns pointer to 'dst'. */
char *StrCat(char *dst, const char *src);

/* Appends up to 'n' characters from 'src' to the end of 'dest'. Returns pointer to 'dest'. */
char *StrNCat(char *dest, const char *src, size_t n);

/*function finds the first occurrence of the substring needle in the string haystack.  The terminating null bytes ('\0') are not compared */
char *StrStr(const char *haystack, const char *needle);

/*function calculates the length (in bytes) of the initial segment of s which consists entirely of bytes in accept.*/
size_t StrSpn(const char *s, const char *accept);

/* function is used for tokenizing strings. These strings are a set of tokens using delimiters/separators characters. In general, tokens are usually words, phrases, or individual characters within a string. */
char *StrTok(char *input_str, const char *delimiters);


#endif
