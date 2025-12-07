#include <stdio.h>     
#include <stdlib.h>     
#include <ctype.h>      
#include <string.h>

int main()     
{         
    char *str1 = "hello";       
    char str2[strlen(str1)];         
    char *str3 = (char*)malloc(strlen(str1) + 1);          
    while (*str1)         
    {               
        *str3 = *str1;             
        ++str3;             
        ++str1;         
    }          
    strcpy(str2, str1);                  
    if(islower(*str1))         
    {             
        *str1 = toupper(*str1);         
    }          
    free(str3); 
    str3 = NULL;

}    