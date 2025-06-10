/****************************
author: Tal Hindi
reviewer: Menny Markovich
status: 
****************************/

#include <stdio.h> /* printf() ... fgets() */
#include <string.h> /* strcmp() strncmp() */
#include <stdlib.h> /* exit() */

#define SIZE 100
#define TMPSIZE 10000

typedef enum
{
	ACTION_OK = 0,
	ACTION_ERR = 1,
	ACTION_EXIT = 2
} ActionStatus;


struct CommandHandler
{
	char *command;
	int (*cmp_func)(const char*,const char*);
	ActionStatus (*act_func)(const char*, const char*);
};

int CmpPrepand(const char *input,const char *command) { return strncmp(input, command,1) == 0; }
int CmpRemove(const char *input,const char *command) { return strcmp(input, command) == 0; }
int CmpCount(const char *input,const char *command)  { return strcmp(input, command) == 0; }
int CmpExit(const char *input,const char *command)   { return strcmp(input, command) == 0; }


ActionStatus RemoveAction(const char *filename,const char *str)
{
	(void)str;
	
	if (0 == remove(filename)) 
	{	
    	return ACTION_OK;
    }
    else
    {
    	return ACTION_ERR;
    }
} 

ActionStatus PrepandAction(const char *filename, const char *str)
{

	/* Open To Read */
    FILE* f = fopen(filename, "r");
    char existing_content[TMPSIZE] = "";
    char line[SIZE];
    
    if (!f) 
    {
		perror("Error opening file for reading");
    	return ACTION_ERR;   
    }
    
    while (fgets(line, sizeof(line), f) != NULL) /* with '\n' include */
    {
    	strcat(existing_content, line);
    };
    
    fclose(f);
    
    
    
    /* Open To Write */
    f = fopen(filename, "w");
    
    if (!f)
    {
        perror("Error opening file for writing");
        return ACTION_ERR;
    }
    
    fprintf(f, "%s\n", str + 1); 
    
    fprintf(f, "%s", existing_content);
    
    fclose(f);
    
    printf("Added prefix line: %s\n", str + 1);
    
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
    
     (void)argc;

    filename = argv[1];
    
    while (1)
    {
		printf("Please enter a string: \n");
		fgets(input,SIZE,stdin);
		
		input[strcspn(input, "\n")] = '\0';
		
		for (i = 0; handlers[i].command != NULL; ++i)
		{
			if (handlers[i].cmp_func(input,handlers[i].command))
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
				printf("Exiting program. Goodbye!\n");
				exit(0);
				break;
		}
    }
    
   
	return 0;
}

