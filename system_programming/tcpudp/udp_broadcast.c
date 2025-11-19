/**************************************
Exercise:   Ex3 - UDP (Broadcast)
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
    int port = PORT;
    struct sockaddr_in broadcast_addr = {0};
    const char* message = "ping";
    ssize_t n = 0;
    char buffer[BUFFER_SIZE];

    if (argc > 1)
    {
        port = atoi(argv[1]);
    }

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0 || EnableBroadcast(socket_fd) < 0)
    {
        perror("Setup failed");
        return 1;
    }

    InitAddress(&broadcast_addr, "255.255.255.255", port);

    printf("[UDP Broadcasting] -> 'Ping' messages on port %d...\n", port);

    while(1)
    {
        if(sendto(socket_fd, message , strlen(message) + 1, 0, (struct sockaddr*)&broadcast_addr, sizeof(broadcast_addr)) < 0)
        {
            perror("[UDP Broadcast] -> sendto() failed ! \n");
        }

        printf("[UDP Broadcasted] Sent -> %s\n", message);

        n = recvfrom(socket_fd, buffer, BUFFER_SIZE - 1, 0, NULL, NULL);
        if (n > 0)
        {
            buffer[n] = '\0';
            printf("[UDP Broadcast] <- Received: %s\n\n", buffer);
        }

        sleep(2);
    }

    printf("[UDP Broadcast] -> completed!\n");

    close(socket_fd);
    return 0;
}
