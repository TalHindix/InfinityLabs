#include <stdio.h>
#include <string.h>


int CheckParen(const char* str)
{
    char stack[1024];
    int top = -1;
    size_t i = 0;
    char open;

    for(i = 0; i < strlen(str); ++i)
    {
        char c = str[i];

        if(c == '(' || c == '{' || c == '[')
        {
            stack[++top] = c;
        }
        else if (c == ')' || c == ']' || c == '}')
        {
            if (top < 0)
            {
                return 0;
            }

            open = stack[top--];  /* pop */

            if ((c == ')' && open != '(') ||
                (c == ']' && open != '[') ||
                (c == '}' && open != '{'))
            {
                return 0;
            }
        }
    }

    return (top == -1); /* true if no unclosed parentheses left */
}


int main()
{
    char str[] = "[()](){[()]()}";
    int y = CheckParen(str);

    printf("Y:%d\n",y);
    return 0;
}
