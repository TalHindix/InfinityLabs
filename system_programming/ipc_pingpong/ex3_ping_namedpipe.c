#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PING_FIFO "/tmp/ping_fifo"
#define PONG_FIFO "/tmp/pong_fifo"
#define BUF_SIZE  (128)


int main(void)
{
    int pingFd = -1;
    int pongFd = -1;
    char buffer[BUF_SIZE] = {0};
    ssize_t bytesRead = 0;
    size_t i = 0;

    mkfifo(PING_FIFO, 0666);
    mkfifo(PONG_FIFO, 0666);

    pingFd = open(PING_FIFO, O_WRONLY);
    pongFd = open(PONG_FIFO, O_RDONLY);

    for (i = 1; i <= 5; ++i)
    {
        int n = snprintf(buffer, BUF_SIZE, "ping %d", i);
        write(pingFd, buffer, (size_t)n + 1);

        memset(buffer, 0, BUF_SIZE);
        bytesRead = read(pongFd, buffer, BUF_SIZE);
        if (bytesRead <= 0)
        {
            break;
        }

        printf("PING: %s\n", buffer);
    }

    close(pingFd);
    close(pongFd);
    return 0;
}

