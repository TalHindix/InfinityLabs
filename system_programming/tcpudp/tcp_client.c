/**************************************
Exercise:   Ex2 - TCP Ping pong
Date:       17/11/2025
Developer:  Tal Hindi
Reviewer:   Yuval Hochman
Status:     Approved
**************************************/

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* atoi */
#include <unistd.h>     /* close */
#include <sys/socket.h> /* connect */
#include <string.h>     /* strlen */
#include <time.h>       /* srand */
#include "lib_socket.h" /* TcpSocket */

#define MIN (3)
#define MAX (10)

int main(int argc, char** argv)
{
    int socket_fd = 0;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    const char *ip = "127.0.0.1";
    int port = PORT;
    ssize_t n = 0;
    int rand_num = 0;


    srand(time(NULL));

    if (argc > 1)
    {
        ip = argv[1];
    }
    if (argc > 2)
    {
        port = atoi(argv[2]);
    }

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        perror("[TCP Client] -> Socket failed");
        return 1;
    }

    InitAddress(&server_addr, ip, port);

    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("[TCP Client] -> failed to connect");
        return 1;
    }


    rand_num = rand() % (MAX- MIN + 1) + MIN; /* generate a rand number between [3,10] */

    printf("[TCP Client] Connected! Starting ping-pong. %d [TIMES]\n",rand_num);
    while(rand_num > 0)
    {
        int delay = 3 + rand() % 8; /* generate a delay seconds between [3,10] */
        send(socket_fd, "ping", 4, 0);
        printf("[TCP Client] -> Sent: ping\n");

        n = recv(socket_fd, buffer, sizeof(buffer) - 1, 0);
        if (n > 0)
        {
            buffer[n] = '\0';
            printf("[TCP Client] <- Received: %s\n", buffer);
            memset(buffer,0,sizeof(buffer));
        }

        --rand_num;

        printf("[TCP Client] -> The delay between each ping pong is: %d\n",delay);
        sleep(delay);
    }

    printf("[TCP Client] -> Ping-pong completed!\n");

    close(socket_fd);
    return 0;
}
