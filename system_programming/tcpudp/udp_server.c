/**************************************
Exercise:   Ex1 - UDP Ping pong
Date:       17/11/2025
Developer:  Tal Hindi
Reviewer:
Status:
**************************************/

#include <stdio.h>      /* printf */
#include <unistd.h>     /* close */
#include <sys/socket.h> /* recvfrom */
#include <string.h>     /* strcmp */
#include <stdlib.h>     /* atoi */

#include "lib_socket.h" /* UdpSocket */

int main(int argc, char** argv)
{
    int socket_fd = 0;
    int port = PORT;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr = {0};
    socklen_t addr_len = sizeof(client_addr);
    ssize_t n = 0;

    if (argc > 1)
    {
        port = atoi(argv[1]);
    }

    socket_fd = UdpSocket();
    if (socket_fd < 0 || BindToPort(socket_fd, port) < 0)
    {
        perror("Setup failed");
        return 1;
    }

    printf("UDP Server listening on port %d\n", port);

    while (1)
    {
        n = recvfrom(socket_fd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr*)&client_addr, &addr_len);
        if (n < 0)
        {
            continue;
        }

        buffer[n] = '\0';
        printf("Received: %s\n", buffer);

        if (0 == strcmp(buffer, "exit"))
        {
            break;
        }

        sendto(socket_fd, "pong", 4, 0, (struct sockaddr*)&client_addr, addr_len);
    }

    close(socket_fd);
    return 0;
}
