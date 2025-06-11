/***************************
 Exercise:  WS3
 Date: 	    05/08/25
 Developer: Tal Hindi
 Reviewer:  Menachem Markovich
 Status: 	Approved
***************************/


#define _POSIX_C_SOURCE 200809L /* required for strdup */
#include <stdio.h>  /* printf() */
#include <stdlib.h> /* malloc() , free() */
#include <string.h> /* strdup() */
#include <ctype.h> /* tolower() */


static size_t CountEnvVariables(char **envp)
{
	size_t n = 0;
	while(NULL != envp[n])
	{
		++n;
	}
	printf("n: %lu\n",n);
	return n;
}


static char **DupEnvLower(char **envp)
{
	size_t count = CountEnvVariables(envp);
    size_t i = 0;
    char **env_copy = (char **)malloc((count + 1) * sizeof(char *)); /* (count + 1) for the null like the original envp */
    char *ch = NULL;

    if (NULL == env_copy)
    {
        return NULL;
    }

    for (i = 0; i < count; ++i)
    {
        env_copy[i] = strdup(envp[i]);
        if (NULL != env_copy[i])
        {
            ch = env_copy[i];
            while ('\0' != *ch)
            {
                *ch = tolower((unsigned char)*ch);
                ++ch;
            }
        }
    }

    env_copy[count] = NULL; /* add the NULL to the end */
    return env_copy;
}

static void Print_Env(char **env)
{
    char **p = env;
    while (NULL != *p)
    {
        printf("%s\n", *p);
        ++p;
    }
}

int main(int argc, char **argv, char **envp)
{
  	char **env_copy = DupEnvLower(envp);
  	char **p = env_copy;
  	
  	(void)argc;
  	(void)argv;
  	
  	Print_Env(env_copy);
	 

    while ( NULL != *p )
    {
        free(*p);
        ++p;
    }
    
    free(env_copy);
    
    return 0;

}
