#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "strings.h"

void PrintResult(const char *desc, int condition)
{
    printf("%s: %s\n", desc, condition ? "PASS" : "FAIL");
}

void TestStrLen()
{
    printf("\n--- Testing StrLen ---\n");
    PrintResult("Length of 'Tal'", StrLen("Tal") == strlen("Tal"));
    PrintResult("Length of empty string", StrLen("") == strlen(""));
    PrintResult("Length of 'hello world\\n'", StrLen("hello world\n") == strlen("hello world\n"));
}

void TestStrCmp()
{
    printf("\n--- Testing StrCmp ---\n");
    PrintResult("Equal strings", StrCmp("abc", "abc") == 0 && strcmp("abc", "abc") == 0);
    PrintResult("abc < abd", StrCmp("abc", "abd") < 0 && strcmp("abc", "abd") < 0);
    PrintResult("abe > abc", StrCmp("abe", "abc") > 0 && strcmp("abe", "abc") > 0);
    PrintResult("Empty vs abc", StrCmp("", "abc") < 0 && strcmp("", "abc") < 0);
    PrintResult("abc vs empty", StrCmp("abc", "") > 0 && strcmp("abc", "") > 0);
}

void TestStrNCmp()
{
    printf("\n--- Testing StrNCmp ---\n");
    PrintResult("abc/abc, n=3", StrNCmp("abc", "abc", 3) == strncmp("abc", "abc", 3));
    PrintResult("abc/abd, n=3", StrNCmp("abc", "abd", 3) == strncmp("abc", "abd", 3));
    PrintResult("abc/ab, n=5", StrNCmp("abc", "ab", 5) == strncmp("abc", "ab", 5));
}

void TestStrCaseCmp()
{
    printf("\n--- Testing StrCaseCmp ---\n");
    PrintResult("Tal vs tal", StrCaseCmp("Tal", "tal") == 0);
    PrintResult("ABC vs abc", StrCaseCmp("ABC", "abc") == 0);
    PrintResult("abc < abd", StrCaseCmp("abc", "abd") < 0);
}

void TestStrCpy()
{
    char my_dest[50];
    char std_dest[50];
    const char *src = "copy this";

    printf("\n--- Testing StrCpy ---\n");
    StrCpy(my_dest, src);
    strcpy(std_dest, src);
    PrintResult("Copy result", strcmp(my_dest, std_dest) == 0);
}

void TestStrNCpy()
{
    char my_dest[50];
    char std_dest[50];
    const char *src = "abcdef";

    printf("\n--- Testing StrNCpy ---\n");
    StrNCpy(my_dest, src, 3);
    strncpy(std_dest, src, 3);
    my_dest[3] = '\0';
    std_dest[3] = '\0';
    PrintResult("First 3 chars", strcmp(my_dest, std_dest) == 0);
}

void TestStrDup()
{
    const char *src = "TalDup";
    char *my_copy = StrDup(src);

    printf("\n--- Testing StrDup ---\n");
    PrintResult("Duplicated string", my_copy && strcmp(my_copy, src) == 0);
    free(my_copy);
}

void TestStrChr()
{
    const char *s = "abcdef";

    printf("\n--- Testing StrChr ---\n");
    PrintResult("Find 'd'", StrChr(s, 'd') == strchr(s, 'd'));
    PrintResult("Find '\0'", StrChr(s, '\0') == strchr(s, '\0'));
    PrintResult("Find 'x'", StrChr(s, 'x') == strchr(s, 'x'));
}

void TestStrCat()
{
    char my_dest[100] = "Hello ";
    char std_dest[100] = "Hello ";

    printf("\n--- Testing StrCat ---\n");
    StrCat(my_dest, "World!");
    strcat(std_dest, "World!");
    PrintResult("Concatenate", strcmp(my_dest, std_dest) == 0);
}

void TestStrNCat()
{
    char my_dest[100] = "123";
    char std_dest[100] = "123";

    printf("\n--- Testing StrNCat ---\n");
    StrNCat(my_dest, "456789", 3);
    strncat(std_dest, "456789", 3);
    PrintResult("Concatenate 3 chars", strcmp(my_dest, std_dest) == 0);
}

void TestStrStr()
{
    const char *hay = "this is a haystack";
    const char *needle1 = "hay";
    const char *needle2 = "notfound";

    printf("\n--- Testing StrStr ---\n");
    PrintResult("Find 'hay'", StrStr(hay, needle1) == strstr(hay, needle1));
    PrintResult("Empty needle", StrStr(hay, "") == strstr(hay, ""));
    PrintResult("Not found", StrStr(hay, needle2) == strstr(hay, needle2));
}

void TestStrSpn()
{
    const char *s1 = "abcde123";
    const char *s2 = "123abc";
    const char *s3 = "xyz";

    printf("\n--- Testing StrSpn ---\n");
    PrintResult("abcde123 with 'abcde'", StrSpn(s1, "abcde") == strspn(s1, "abcde"));
    PrintResult("123abc with '123'", StrSpn(s2, "123") == strspn(s2, "123"));
    PrintResult("xyz with 'abc'", StrSpn(s3, "abc") == strspn(s3, "abc"));
}

void TestStrTok()
{
    const char input[] = "one,two,,three";
    char my_input[50];
    char std_input[50];
    char *token_my, *token_std;

    printf("\n--- Testing StrTok ---\n");
    strcpy(my_input, input);
    strcpy(std_input, input);

    token_my = StrTok(my_input, ",");
    token_std = strtok(std_input, ",");

    while (token_my || token_std)
    {
        int match = (token_my && token_std && strcmp(token_my, token_std) == 0) || (!token_my && !token_std);
        PrintResult("Token", match);
        token_my = StrTok(NULL, ",");
        token_std = strtok(NULL, ",");
    }
}

int main()
{
    TestStrLen();
    TestStrCmp();
    TestStrNCmp();
    TestStrCaseCmp();
    TestStrCpy();
    TestStrNCpy();
    TestStrDup();
    TestStrChr();
    TestStrCat();
    TestStrNCat();
    TestStrStr();
    TestStrSpn();
    TestStrTok();

    return 0;
}

