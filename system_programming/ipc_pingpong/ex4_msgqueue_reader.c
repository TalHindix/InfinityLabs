#include <stdio.h>      /* printf */
#include <stdlib.h>     /* exit */
#include <sys/msg.h>    /* msgget */
#include <sys/ipc.h>    /* ftok */
#include <unistd.h>     /* getpid */

#define PATH_FOR_KEY "/tmp"
#define PROJECT_ID 'A'

struct MsgBuffer
{
    long type;
    char text[100];
};

int main(void)
{
    key_t key = 0;
    int msgid = 0;
    struct MsgBuffer message = {0};
    pid_t pid = getpid();

    key = ftok(PATH_FOR_KEY, PROJECT_ID);
    if (-1 == key)
    {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }

    msgid = msgget(key, 0666);
    if (-1 == msgid)
    {
        perror("msgget failed - queue doesn't exist");
        exit(EXIT_FAILURE);
    }

    printf("Reader [PID %d]: Connected to message queue %d\n", pid, msgid);

    if (-1 == msgrcv(msgid, &message, sizeof(message.text), 1, 0))
    {
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
    }

    printf("Reader [PID %d]: Received - %s\n", pid, message.text);

    return 0;
}

