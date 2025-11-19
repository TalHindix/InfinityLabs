/**************************************
Exercise:   Ex1 - UDP
Date:       17/11/2025
Developer:  Tal Hindi
Reviewer:   Yuval Hochman
Status:     Approved
**************************************/

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* atoi */
#include <unistd.h>     /* close */
#include <sys/socket.h> /* sendto */
#include <string.h>     /* strlen */

#include "lib_socket.h" /* UdpSocket */

int main(int argc, char** argv)
{
    int socket_fd = 0;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr = {0};
    const char* ip = "127.0.0.1";
    const char* message = "ping";
    int port = PORT;
    ssize_t n = 0;

    if (argc > 1)
    {
        ip = argv[1];
    }
    if (argc > 2)
    {
        port = atoi(argv[2]);
    }

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0)
    {
        perror("[UDP Client] -> Socket failed");
        return 1;
    }

    InitAddress(&server_addr, ip, port);

    printf("[UDP Client] -> ready. Type exit to quit.\n");

    while(1)
    {
        if(sendto(socket_fd, message , strlen(message) + 1 , 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        {
            perror("[UDP Client] -> sendto failed (error during copying data)\n");
        }

        printf("[UDP Client] -> Sent: %s\n",message);

        n = recvfrom(socket_fd, buffer, BUFFER_SIZE - 1, 0, NULL, NULL);
        if (n > 0)
        {
            buffer[n] = '\0';
            printf("[UDP Client] -> Received: %s\n\n", buffer);
        }

        sleep(2);
    }

    printf("[UDP Client] Ping-pong completed!\n");

    close(socket_fd);
    return 0;
}


