#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main(void)
{
    key_t key = 0;
    int shmid = 0;
    char* data = NULL;

    key = ftok("/tmp", 'B');
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    data = (char*)shmat(shmid, NULL, 0);

    sprintf(data, "Hello from P1 (PID: %d)", getpid());
    printf("Writer: %s\n", data);

    shmdt(data);
    return 0;
}
