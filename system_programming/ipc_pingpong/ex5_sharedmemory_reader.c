#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main(void)
{
    key_t key = 0;
    int shmid = 0;
    char* data = NULL;

    sleep(1);

    key = ftok("/tmp", 'B');
    shmid = shmget(key, SHM_SIZE, 0666);
    data = (char*)shmat(shmid, NULL, 0);

    printf("Reader (PID %d): %s\n", getpid(), data);

    shmdt(data);
    return 0;
}
