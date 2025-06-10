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
	e_ACTION_OK = 0,
	e_ACTION_ERR_OPENFILE = 1,
	e_ACTION_ERR_REMOVE = 2,
	e_ACTION_OK_REMOVE = 3,
	e_ACTION_EXIT = 4
	
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

	/* Open To Read */
    FILE* f = fopen(filename, "r");
    char existing_content[TMPSIZE] = "";
    char line[SIZE];
    
    if (!f) 
    {
		perror("Error opening file for reading");
    	return e_ACTION_ERR_OPENFILE;   
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
        return e_ACTION_ERR_OPENFILE;
    }
    
    fprintf(f, "%s\n", str + 1); 
    
    fprintf(f, "%s", existing_content);
    
    fclose(f);
    
    printf("Added prefix line: %s\n", str + 1);
    
    return e_ACTION_OK;

}

ActionStatus AppendAction(const char *filename, const char *str)
{
	FILE *fp = fopen(filename, "a");
    if (!fp)
    {
        perror("fopen append");
        return e_ACTION_ERR_OPENFILE;
    }
    fprintf(fp, "%s\n", str);
    fclose(fp);
    
    return e_ACTION_OK;
}


ActionStatus CountAction(const char *filename, const char *str)
{

	FILE *fp = fopen(filename, "r");
    int lines = 0;
    int ch = 0;
    
    (void)str;

    if (!fp)
    {
        perror("fopen count");
        return e_ACTION_ERR_OPENFILE;
    }

    while(!feof(fp)) /* end of file flag */
	{
	  ch = fgetc(fp);
	  if(ch == '\n')
	  {
		lines++;
	  }
	}

    fclose(fp);
    printf("The file contains %d line(s).\n", lines);
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
    char input[SIZE];
    const char *filename;
    size_t i = 0;
    ActionStatus status;
        
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
		
		/*strcspn return the pos (int) for this first occurences of '\n' */
		input[strcspn(input, "\n")] = '\0'; /* The most elegant way in the internet to remove '\n' */
		
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
			case e_ACTION_EXIT:
				printf("Exiting program. Goodbye!\n");
				exit(0);
				break;
		}
    }
    
   
	return 0;
}

