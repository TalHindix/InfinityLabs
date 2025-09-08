/****************************
author: Tal Hindi
reviewer: Menny Markovich
status: 
****************************/

#include <stdio.h> /* printf() ... fgets() */
#include <string.h> /* strcmp() strncmp() */
#include <stdlib.h> /* exit() */

#define INPUT_BUF 100
#define TEMP_BUF 10000

typedef enum
{
	e_ACTION_OK = 0,
	e_ACTION_ERR_OPENFILE = 1,
	e_ACTION_ERR_REMOVE = 2,
	e_ACTION_OK_REMOVE = 3,
	e_ACTION_EXIT = 4,
	e_ACTION_ERR_CLOSEFILE = 5
} ActionStatus;


struct CommandHandler
{
	char *command;
	int (*cmp_func)(const char*, const char*);
	ActionStatus (*act_func)(const char*, const char*);
};

int CmpPrepand(const char *input, const char *command)
{
    return (strncmp(input, command, 1) == 0);
}

int CmpRemove(const char *input, const char *command)
{
    return (strcmp(input, command) == 0);
}

int CmpCount(const char *input, const char *command)
{
    return (strcmp(input, command) == 0);
}

int CmpExit(const char *input, const char *command)
{
    return (strcmp(input, command) == 0);
}
int CmpAppend(const char *input, const char *command)
{
	(void)input;
	(void)command;
	return 1;
}


ActionStatus RemoveAction(const char *filename,const char *str)
{
	(void)str;
	
	if (0 == remove(filename)) /* remove() -> return 0 if the file deleted successfuly */
	{	
    	return e_ACTION_OK_REMOVE; 
    }
    else
    {
    	return e_ACTION_ERR_REMOVE;
    }
} 

ActionStatus PrepandAction(const char *filename, const char *str)
{
	FILE* filep = fopen(filename, "r");
    char existing_content[TEMP_BUF] = "";
    char line[INPUT_BUF];
    
    if (!filep) 
    {
		printf("Error opening file for reading");
    	return e_ACTION_ERR_OPENFILE;   
    }
    
    while (fgets(line, sizeof(line), filep) != NULL) /* with '\n' include */
    {
    	size_t remaining = TEMP_BUF - strlen(existing_content) - 1;
    	
    	if (strlen(line) < remaining)
        {
            strncat(existing_content, line, remaining);
        }
        else
        {
            fprintf(stderr, "Buffer overflow risk. Stopping read.\n");
            fclose(filep);
            return e_ACTION_ERR_OPENFILE;
        }
    };
    
    if (fclose(filep) == EOF)
	{
		printf("Error close the file after writing");
		return e_ACTION_ERR_CLOSEFILE;
	}
    
    
    filep = fopen(filename, "w");
    
    if (!filep)
    {
        printf("Error opening file for writing");
        return e_ACTION_ERR_OPENFILE;
    }
    
    fprintf(filep, "%s\n", str + 1); 
    
    fprintf(filep, "%s", existing_content);
    
    if (fclose(filep) == EOF)
    {
        printf("Error closing file after writing");
        return e_ACTION_ERR_OPENFILE;
    }
    
    printf("Added prefix line: %s\n", str + 1);
    
    return e_ACTION_OK;

}

ActionStatus AppendAction(const char *filename, const char *str)
{
	FILE *fp = fopen(filename, "a");
	
    if (!fp)
    {
        printf("fopen append");
        return e_ACTION_ERR_OPENFILE;
    }
    
    fprintf(fp, "%s\n", str);
    
    if (fclose(fp) == EOF)
    {
    	printf("Error close the file after append");
    	return e_ACTION_ERR_CLOSEFILE;
    }
    
    return e_ACTION_OK;
}


ActionStatus CountAction(const char *filename, const char *str)
{

	FILE *fp = fopen(filename, "r");
    size_t lines = 0;
    int ch = 0;
    
    (void)str;

    if (!fp)
    {
        printf("fopen count");
        return e_ACTION_ERR_OPENFILE;
    }

    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '\n')
        {
            ++lines;
        }
    }

    if (fclose(fp) == EOF)
    {
        printf("Error close the file after counting");
        return e_ACTION_ERR_CLOSEFILE;
    }
    
    printf("The file contains %lu line(s).\n", lines);
    return e_ACTION_OK;
}

ActionStatus ExitAction(const char* filename,const char *str)
{
	(void)filename;
    (void)str;
 
    return e_ACTION_EXIT;
}


int main(int argc, char **argv)
{
	char input[INPUT_BUF];
    const char *filename;
    size_t i = 0;
    ActionStatus status;
        
    struct CommandHandler handlers[] = {
    	{"<",		CmpPrepand,	PrepandAction},
        {"-remove", CmpRemove,	RemoveAction},
        {"-count",  CmpCount,	CountAction},
        {"-exit",   CmpExit,	ExitAction},
        {NULL, 		CmpAppend, 	AppendAction}
    };
    
    (void)argc;

    filename = argv[1];
    
    while (1)
    {
		printf("Please enter a string:\n");
		fgets(input,INPUT_BUF,stdin);
		
		/*strcspn return the pos (int) for this first occurences of '\n' */
		input[strcspn(input, "\n")] = '\0'; /* The most elegant way in the internet to remove '\n' */
		
		for (i = 0; handlers[i].cmp_func != NULL; ++i)
		{
			if (handlers[i].cmp_func(input,handlers[i].command))
			{
				status = handlers[i].act_func(filename, input);
				break;
			}
		}

		
		switch (status)
		{
			case e_ACTION_OK:
				break;
			case e_ACTION_ERR_OPENFILE:
				printf("Error, Open FILE Not SUCCESS !\n");
				break;
			case e_ACTION_ERR_REMOVE:
				printf("Error in the remove file action func().\n");
				break;
			case e_ACTION_OK_REMOVE:
				printf("The File Is Removed Successfully.\n");
				break;
			case e_ACTION_ERR_CLOSEFILE:
                printf("Error: Failed to close file properly.\n");
                break;
			case e_ACTION_EXIT:
				printf("Exiting program. Goodbye!\n");
				exit(0);
		}
    }
    
   
	return 0;
}

