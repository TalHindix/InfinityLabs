/***************************
 Exercise:  WS3
 Date: 	    05/08/25
 Developer: Tal Hindi
 Reviewer:  Menachem Markovich
 Status: 	Approved
***************************/


#define _POSIX_C_SOURCE 200809L
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
	printf("n:%lu\n",n);
	return n;
}


static char **DupEnv(char **envp)
{
	size_t count = CountEnvVariables(envp);
	char **copy = (char**)malloc((count + 1) * sizeof(char *));
	char **dest = copy;
	char **p = envp;
	
	if ( NULL == copy )
	{
		return NULL;
	}
	
	while( NULL != *p )
	{
		*dest = strdup(*p);
		++dest;
		++p;
	}
	
	*dest = NULL;
	return copy;	
}

void ToLowerEnv(char **envp)
{
    char **p = envp;

    while (NULL != *p)
    {
        char *s = *p;
        while (*s)
        {
            *s = tolower((unsigned char)*s);
            ++s;
        }
        ++p;
    }
}

void Print_Env(char **env)
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
  	char **env_copy = DupEnv(envp);
  	char **p = env_copy;
  	
  	(void)argc;
  	(void)argv;
  	
  	ToLowerEnv(env_copy);
  	Print_Env(env_copy);
	 

    while ( NULL != *p )
    {
        free(*p);
        ++p;
    }
    
    free(env_copy);
    
    return 0;

}
