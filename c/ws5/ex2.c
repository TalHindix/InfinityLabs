/****************************
author: Tal Hindi
reviewer: Menny Markovich
status: 
****************************/

#include <stdio.h> /* printf() ... fgets() */
#include <string.h> /* strcmp() strncmp() */
#include <stdlib.h> /* exit() */

#define SIZE 100

typedef enum
{
	ACTION_OK = 0,
	ACTION_ERR = 1,
	ACTION_EXIT = 2
} ActionStatus;


struct CommandHandler
{
	char *command;
	int (*cmp_func)(const char*);
	ActionStatus (*act_func)(const char*, const char*);
};

int CmpPrepand(const char *input) { return strncmp(input, "<", 1) == 0; }
int CmpRemove(const char *input) { return strncmp(input, "-remove", 7) == 0; }
int CmpCount(const char *input)  { return strncmp(input, "-count", 6) == 0; }
int CmpExit(const char *input)   { return strncmp(input, "-exit", 5) == 0; }


ActionStatus RemoveAction(const char *filename,const char *str)
{
	(void)str;
	
	if (0 == remove(filename)) 
	{
		if(*str != 'p')
		{
    		printf("File deleted successfully.\n");
    		
    	}
    	else
    	{
    		printf("Prepend to the file successfuly!\n");
    		
    	}
    	return ACTION_OK;
    }
    else
    {
    	printf("Error: Unable to delete the file.\n");
    	return ACTION_ERR;
    }
} 

ActionStatus PrepandAction(const char *filename, const char *str)
{
    const char *p_start = str;
    FILE *src  = fopen(filename, "r");
    char tmpname[SIZE];
    FILE *dst;
    
    if ('<' == p_start[0])
    {
        ++p_start;
        
        if (' ' == *p_start)
        {
         	++p_start; 
        }
    }
	
	
	
	if (strlen(filename) + 4 >= SIZE)
	{
		fprintf(stderr, "tmpname buffer too small\n");
		return ACTION_ERR;
	}
	
	strcpy(tmpname, filename);
	strcat(tmpname, ".tmp");


    dst = fopen(tmpname, "w");
    if (!dst)
    {
        perror("fopen temp");
        if (src) fclose(src);
        return ACTION_ERR;
    }

    fputs(p_start, dst);
    fputc('\n',dst);

    if (src)
    {
        int ch;
        while ((ch = fgetc(src)) != EOF)
        {
            fputc(ch, dst);
        }
        fclose(src);
    }
    fclose(dst);

 
    RemoveAction(filename,"prepand");
    
    if (rename(tmpname, filename) != 0)
	{
		perror("rename");
		return ACTION_ERR;
	}

    
    return ACTION_OK;
    
}

ActionStatus AppendAction(const char *filename, const char *str)
{
	FILE *fp = fopen(filename, "a");
    if (!fp)
    {
        perror("fopen append");
        return ACTION_ERR;
    }
    fprintf(fp, "%s\n", str);
    fclose(fp);
    
    return ACTION_OK;
}


ActionStatus CountAction(const char *filename, const char *str)
{

	
	FILE *fp = fopen(filename, "r");
    char line[SIZE];
    int count = 0;
    
    (void)str;

    if (!fp)
    {
        perror("fopen count");
        return ACTION_ERR;
    }

    while (fgets(line, SIZE, fp))
    {
     	if (strcmp(line, "\n") != 0 && strcmp(line, "\r\n") != 0)
        {
            ++count;
        }
    }

    fclose(fp);
    printf("The file contains %d line(s).\n", count);
    return ACTION_OK;
}

ActionStatus ExitAction(const char* filename,const char *str)
{
	(void)filename;
    (void)str;

    printf("Exiting program. Goodbye!\n");
    return ACTION_EXIT;
}


int main(int argc, char **argv)
{
    char input[SIZE];
    const char *filename;
    size_t i = 0;
    ActionStatus status = ACTION_OK;
    
    struct CommandHandler handlers[] = {
    	{"<",CmpPrepand,PrepandAction},
        {"-remove", CmpRemove,RemoveAction},
        {"-count",  CmpCount,CountAction},
        {"-exit",   CmpExit,ExitAction},
        {NULL, NULL, NULL}
    }; 

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    filename = argv[1];
    
    printf("Please enter a string: \n");
    fgets(input,SIZE,stdin);
    
    input[strcspn(input, "\n")] = '\0';
    
    for (i = 0; handlers[i].command != NULL; ++i)
    {
        if (handlers[i].cmp_func(input))
        {
            status = handlers[i].act_func(filename, input);
            break;
        }
    }

    if (handlers[i].command == NULL)
    {
        status = AppendAction(filename,input);
    }
    
    switch (status)
    {
        case ACTION_OK:
            break;
        case ACTION_ERR:
            fprintf(stderr, "Action failed. Please try again.\n");
            break;
        case ACTION_EXIT:
            return 0;
    }
    
   
	return 0;
}

