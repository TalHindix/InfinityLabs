#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ReverseWords(char *str)
{
    
}

int main(void)
{
    char *str = malloc(20);
    if (!str) return 1;

    strcpy(str, "Hello World");

    ReverseWords(str);

    free(str);
    return 0;
}
