/**************************************
Exercise: 	System Programming - Sem_Manipulation
Date:		12/09/2025
Developer:	Tal Hindi
Reviewer: 	Guy Argaman
Status:		In Progress
**************************************/

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* exit */
#include <string.h>     /* strcmp */
#include <semaphore.h>  /* sem_wait */
#include <fcntl.h>      /* O_CREAT */
#include <errno.h>      /* errno */
#include <signal.h>		/* signal */

static sem_t* g_semaphore = NULL;
static const char* g_semaphore_name = NULL;
static int g_undo_count = 0;

static void CleanUp(void);
static void SignalHandler(int sig);


int main(int argc, char* argv[])
{
    char line[256] = {0};
    char command = 0;
    char undo_param[16] = {0};
    unsigned int number = 0;
    int args_read = 0;
    int value = 0;
    size_t i = 0;
    
    /* Check command line arguments */
    if (2 != argc)
    {
        printf("Usage: %s <semaphore_name>\n", argv[0]);
        return 1;
    }
    
    g_semaphore_name = argv[1];
    
    /* Register CleanUp() function */
    atexit(CleanUp);
    
	/* Register signal handlers */
    signal(SIGINT, SignalHandler);   /* Ctrl-C */
    signal(SIGTERM, SignalHandler);  /* kill -15 */

    /* Create a new named semaphore with value 0 */
    g_semaphore = sem_open(g_semaphore_name, O_CREAT | O_EXCL, 0644, 0);
    if (SEM_FAILED == g_semaphore)
    {
        if (EEXIST == errno)
        {
            printf("Error: Semaphore '%s' already exists\n", g_semaphore_name);
        }
        else
        {
            perror("sem_open failed");
        }
        return 1;
    }
    
    printf("Semaphore '%s' created\n", g_semaphore_name);
    printf("Commands: D number [undo], I number [undo], V, X\n\n");
    
    while (1)
    {
        printf("> ");
        
        if (NULL == fgets(line, sizeof(line), stdin))
        {
			perror("FAIL_TO_READ_INPUT");
            break;
        }
        
		/* Clear undo_param BEFORE parsing */
		undo_param[0] = '\0';

        args_read = sscanf(line, "%c %u %s", &command, &number, undo_param);
        
        switch (command)
        {
            case 'D':
            case 'd':
                if (args_read >= 2)
                {
                    for (i = 0; i < number; ++i)
                    {
                        sem_wait(g_semaphore);
                    }
                    
                    /* Check if undo was requested */
                    if (3 == args_read && 0 == strcmp(undo_param, "undo"))
                    {
                        g_undo_count += (int)number;  /* Will need to increment on exit */
                        printf("Decremented by %u (will undo on exit)\n", number);
                    }
                    else
                    {
                        printf("Decremented by %u\n", number);
                    }
                }
                else
                {
                    printf("Usage: D number [undo]\n");
                }
                break;
                
            case 'I':
            case 'i':
                if (args_read >= 2)
                {
                    for (i = 0; i < number; ++i)
                    {
                        sem_post(g_semaphore);
                    }
                    
                    if (3 == args_read && 0 == strcmp(undo_param, "undo"))
                    {
                        g_undo_count -= (int)number;
                        printf("Incremented by %u (will undo on exit)\n", number);
                    }
                    else
                    {
                        printf("Incremented by %u\n", number);
                    }
                }
                else
                {
                    printf("Usage: I number [undo]\n");
                }
                break;
                
            case 'V':
            case 'v':
                if (0 == sem_getvalue(g_semaphore, &value))
                {
                    printf("Current value: %d\n", value);
                }
                else
                {
                    perror("sem_getvalue failed");
                }
                break;
                
            case 'X':
            case 'x':
                printf("Exiting...\n");
                return 0;
                
            default:
                printf("Invalid command\n");
                break;
        }
        
    }
    
    return 0;
}

static void CleanUp(void)
{
    size_t i = 0;
    
    if (NULL != g_semaphore)
    {
        /* Apply undo operations if needed */
        if (0 != g_undo_count)
        {
            printf("\nApplying undo operations...\n");
            
            if (g_undo_count > 0)
            {
                /* Need to increment */
                for (i = 0; i < (size_t)g_undo_count; ++i)
                {
                    sem_post(g_semaphore);
                }
                printf("Undone: incremented by %d\n", g_undo_count);
            }
            else
            {
                /* Note: This might block if semaphore value is too low */
                for (i = 0; i < (size_t)(-g_undo_count); ++i)
                {
                    sem_wait(g_semaphore);
                }
                printf("Undone: decremented by %d\n", -g_undo_count);
            }
        }
        
        sem_close(g_semaphore);
        sem_unlink(g_semaphore_name);
        printf("Semaphore '%s' cleaned up\n", g_semaphore_name);
    }
}

static void SignalHandler(int sig)
{
    printf("\nReceived signal %d, cleaning up...\n", sig);
    exit(0);  /* This will trigger atexit(CleanUp) */
}