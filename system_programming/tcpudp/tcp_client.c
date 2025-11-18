/**************************************
Exercise:   Ex2 - TCP Ping pong
Date:       17/11/2025
Developer:  Tal Hindi
Reviewer:
Status:
**************************************/

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* atoi */
#include <unistd.h>     /* close */
#include <sys/socket.h> /* connect */
#include <string.h>     /* strlen */

#include "lib_socket.h" /* TcpSocket */

int main(int argc, char** argv)
{
    int socket_fd = 0;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    const char *ip = "127.0.0.1";
    int port = PORT;
    size_t i = 0;
    ssize_t n = 0;

    if (argc > 1)
    {
        ip = argv[1];
    }
    if (argc > 2)
    {
        port = atoi(argv[2]);
    }

    socket_fd = TcpSocket();
    if (socket_fd < 0)
    {
        perror("Socket failed");
        return 1;
    }

    InitAddress(&server_addr, ip, port);

    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connect failed");
        return 1;
    }

    printf("TCP Client: Connected! Starting ping-pong.\n");

    for (i = 0; i < 5; ++i)
    {
        send(socket_fd, "ping", 4, 0);
        printf("Sent: ping\n");

        n = recv(socket_fd, buffer, BUFFER_SIZE - 1, 0);
        if (n > 0)
        {
            buffer[n] = '\0';
            printf("Received: %s\n\n", buffer);
        }

        sleep(1);
    }

    printf("Ping-pong completed!\n");

    close(socket_fd);
    return 0;
}
