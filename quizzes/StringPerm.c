#include <stdio.h> /* printf */
#include <string.h> 

void Swap(char* a,  char* b)
{
    char temp = *a;
    *a = *b;
    *b = temp;
}

void Permtutation(char* str , size_t l, size_t h)
{
    size_t i = 0;

    if(l == h)
    {
        printf("%s\n",str);
    }
    else
    {
        for(i = l; i <= h; ++i)
        {
            Swap(&str[l],&str[i]);
            Permtutation(str,l + 1, h);
            Swap(&str[l],&str[i]);
        }
    }
}


int main(void)
{
    char str[] = "ABC";
    Permtutation(str ,0 , strlen((str) +1 ));
    
    return 0;
}

