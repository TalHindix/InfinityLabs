#include <stdio.h>
#include <pthread.h>

struct thread_data
{
    int thread_id;
    int sum;
    char* message;
};


void* PrintMessageFunction(void* thread_data);

int main(void)
{
    int iret1 = 0;
    int iret2 = 0;

    pthread_t thread1 = 0;
    pthread_t thread2 = 0;

        struct thread_data my_struct_data;
        my_struct_data.message = malloc(sizeof(char) * 255);
        my_struct_data.thread_id = 10;
        my_struct_data.sum = 35;

        pthread_create(&thread1, NULL, PrintMessageFunction, (void*)&my_struct_data);
    pthread_join(thread1,NULL);
/*     pthread_exit(NULL);
 */
    
    return 0;
}


void* PrintMessageFunction(void* thread_arg)
{
    
}