#include <stdio.h> /* printf */
#include <unistd.h> /* sleep */
#include <sys/types.h> /* pid_t */
#include <string.h> /* strlen */
#define BUFFER_SIZE (32)

int main(void)
{
    pid_t pid = 0;
    int pipe_parent_to_child[2] = {0}; /* [0] - toRead  [1] - toWrite */
    int pipe_child_to_parent[2] = {0}; /* [0] - toRead  [1] - toWrite */

    if(-1 == pipe(pipe_parent_to_child))
    {
        return -1;
    }
    if(-1 == pipe(pipe_child_to_parent))
    {
        return -1;
    }

    pid = fork();
    if(pid < 0)
    {
        return -1;
    }

    if(0 == pid)
    {
        char buffer[BUFFER_SIZE] = {0};
        const char* msg = "Pong";
        size_t i = 0;
        size_t count = 10;

        close(pipe_parent_to_child[1]);
        close(pipe_child_to_parent[0]);

        for(i = 0; i < count; ++i)
        {
            read(pipe_parent_to_child[0],buffer,sizeof(buffer));

            printf("%s\n", buffer);

            write(pipe_child_to_parent[1],msg , strlen(msg) + 1);
        }

        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);

        /* child code */
    }
    else
    {
        char buffer[BUFFER_SIZE] = {0};
        const char* msg = "Ping";
        size_t i = 0;
        size_t count = 10;

        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);


        for(i = 0; i < count; ++i)
        {
            write(pipe_parent_to_child[1],msg, strlen(msg) + 1);
            read(pipe_child_to_parent[0],buffer, sizeof(buffer));

            printf("%s\n\n", buffer);
        }

        close(pipe_parent_to_child[1]);
        close(pipe_child_to_parent[0]);
        /* parent code */
    }


    return 0;
}
