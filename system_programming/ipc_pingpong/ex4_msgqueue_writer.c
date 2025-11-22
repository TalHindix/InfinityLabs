#include <stdio.h>      /* printf */
#include <stdlib.h>     /* exit */
#include <sys/msg.h>    /* msgget */
#include <sys/ipc.h>    /* ftok */
#include <string.h>     /* strcpy */

#define PATH_FOR_KEY "/tmp"
#define PROJECT_ID 'A'
#define PERMISSIONS 0666

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

    key = ftok(PATH_FOR_KEY, PROJECT_ID);
    if (-1 == key)
    {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }

    msgid = msgget(key, IPC_CREAT | PERMISSIONS);
    if (-1 == msgid)
    {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    printf("Writer: Message queue created with ID: %d\n", msgid);

    message.type = 1;
    strcpy(message.text, "Message 1 from P1");
    msgsnd(msgid, &message, sizeof(message.text), 0);
    printf("Writer: Sent - %s\n", message.text);

    strcpy(message.text, "Message 2 from P1");
    msgsnd(msgid, &message, sizeof(message.text), 0);
    printf("Writer: Sent - %s\n", message.text);

    strcpy(message.text, "Message 3 from P1");
    msgsnd(msgid, &message, sizeof(message.text), 0);
    printf("Writer: Sent - %s\n", message.text);

    strcpy(message.text, "Message 4 from P1");
    msgsnd(msgid, &message, sizeof(message.text), 0);
    printf("Writer: Sent - %s\n", message.text);

    printf("Writer: All messages sent. Process P1 will now exit.\n");

    return 0;
}
