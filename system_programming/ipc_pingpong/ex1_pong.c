#include <stdio.h> /* printf */
#include <semaphore.h> /* sem_init */
#include <fcntl.h> /* O_CREAT */
#include <time.h>
#include <unistd.h>

#define SEM_PING "/sem_ping"
#define SEM_PONG "/sem_pong"

int counter = 5;

int main(void)
{
    sem_t* sem_ping = NULL;
    sem_t* sem_pong = NULL;

    sem_ping = sem_open(SEM_PING,O_CREAT,0666,1);
    if(SEM_FAILED == sem_ping)
    {
        return -1;
    }
    sem_pong = sem_open(SEM_PONG,O_CREAT,0666, 0);
    if(SEM_FAILED == sem_pong)
    {
        return -1;
    }

    while(counter)
    {
        if(-1 == sem_wait(sem_pong))
        {
            return -1;
        }

        printf("Pong is sent\n\n");

        if(-1 == sem_post(sem_ping))
        {
            return -1;
        }

        --counter;
    }

    sem_close(sem_ping);
    sem_close(sem_pong);

    sem_unlink(SEM_PING);
    sem_unlink(SEM_PONG);

    return 0;
}
