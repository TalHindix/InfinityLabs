#include <stdio.h> /* printf() */

void CleanSpaces(char *input)
{
    char *read_ptr = input;        
    char *write_ptr = input;       
    int in_whitespace = 0;   

    while (*read_ptr == ' ' || *read_ptr == '\t')
    {
        read_ptr++;
    }
    
    while ('\0' != *read_ptr)
    {
    	if (*read_ptr == ' ' || *read_ptr == '\t')
        {
            if (!in_whitespace)
            {
                *write_ptr = ' ';
                write_ptr++;
                in_whitespace = 1;
            }
        }
        else
        {
            *write_ptr = *read_ptr;
            write_ptr++;
            in_whitespace = 0;
        }
		read_ptr++; 
    }
    
    if (write_ptr > input && *(write_ptr - 1) == ' ')
    {
        write_ptr--;
    }

    *write_ptr = '\0';
}


int main()
{
	char text[] = "   Hello     my   name   is\ttal   \t";

    printf("Before: \"%s\"\n", text);

    CleanSpaces(text);

    printf("After: \"%s\"\n", text);

    return 0;	
}
