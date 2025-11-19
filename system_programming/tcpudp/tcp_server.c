/**************************************
Exercise:   Ex2 - TCP Ping pong(Server)
Date:       17/11/2025
Developer:  Tal Hindi
Reviewer:   Yuval Hochman
Status:     Approved
**************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <unistd.h> /* close */
#include <sys/socket.h> /* listen */
#include <string.h> /* strcmp */
#include <pthread.h> /* pthread_create */

#include "lib_socket.h" /* TcpSocket */

static void* HandleClient(void *arg);

int main(int argc, char** argv)
{
    int server_fd = 0;
    int port = PORT;
    int* client_fd = NULL;
    pthread_t thread = {0};

    if (argc > 1)
    {
        port = atoi(argv[1]);
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0 || BindToPort(server_fd, port) < 0)
    {
        perror("Setup failed");
        return 1;
    }

    if (listen(server_fd, 5) < 0)
    {
        perror("Listen failed");
        return 1;
    }

    printf("TCP Server listening on port %d\n", port);

    while (1)
    {
        client_fd = malloc(sizeof(int));
        if (!client_fd)
        {
            continue;
        }

        *client_fd = accept(server_fd, NULL, NULL);
        if (*client_fd < 0)
        {
            free(client_fd);
            continue;
        }

        printf("New client connected\n");

        pthread_create(&thread, NULL, HandleClient, client_fd);
        pthread_detach(thread);
    }

    close(server_fd);
    return 0;
}

static void* HandleClient(void *arg)
{
    int client_fd = *(int*)arg;
    char buffer[BUFFER_SIZE];
    ssize_t n = 0;
    free(arg);

    while (1)
    {
        n = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (n <= 0)
        {
            break;
        }

        buffer[n] = '\0';
        printf("Received: %s\n", buffer);

        if (0 == strcmp(buffer, "exit"))
        {
            break;
        }

        send(client_fd, "pong", 4, 0);
        printf("Sent: pong\n");
    }

    close(client_fd);
    printf("Client disconnected\n");

    return NULL;
}
