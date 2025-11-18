/**************************************
Exercise:   Ex3 - UDP Broadcast
Date:       17/11/2025
Developer:  Tal Hindi
Reviewer:
Status:
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
    int i = 0;
    struct sockaddr_in broadcast_addr = {0};
    const char* messages[] = {"Tal ", "Hindi ", "The King"};

    if (argc > 1)
    {
        port = atoi(argv[1]);
    }

    socket_fd = UdpSocket();
    if (socket_fd < 0 || EnableBroadcast(socket_fd) < 0)
    {
        perror("Setup failed");
        return 1;
    }

    InitAddress(&broadcast_addr, "255.255.255.255", port);

    printf("Broadcasting 3 messages on port %d...\n", port);

    for (i = 0; i < 3; ++i)
    {
        sendto(socket_fd, messages[i], strlen(messages[i]), 0, (struct sockaddr*)&broadcast_addr, sizeof(broadcast_addr));

        printf("Broadcasted: %s\n", messages[i]);

        sleep(2);
    }

    printf("Broadcast completed!\n");

    close(socket_fd);
    return 0;
}
