#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define num_threads 10
#define run_time 30
sem_t binSem;

void exitBridge(void* arg)
{
    printf("%d is exiting bridge.\n", *(int *)arg);
    sem_post(&binSem);
}

void* enterBridge(void* arg)
{
    sem_wait(&binSem);
    int s = rand() % 3 + 1;
    printf("%d is entering bridge, wait %d seconds.\n", *(int *)arg, s);
    sleep(s);
    exitBridge(arg);
}

int main(int argc, char **argv)
{
    int res, i;
    int number[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    /*sem_init(sem_t *sem, int pshared, unsigned int value)
    The pshared argument indicates whether this semaphore is to be shared between the threads of a process, or between processes.
    If pshared has the value 0, then the semaphore is shared between the threads of a process.
    The value argument specifies the initial value for the semaphore.
    sem_init() returns 0 on success; on error, -1 is returned*/
    res = sem_init(&binSem, 0, 1);
    if(res != 0)
    {
        printf("Semaphore initialization failed!!/n");
        exit(EXIT_FAILURE);
    }

    pthread_t farmers[num_threads];
    for(i = 0; i < num_threads; i++)
    {
        /*pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
        arg是傳遞給start_routine的參數*/
        res = pthread_create(&farmers[i], NULL, enterBridge, &number[i]);
        if (res != 0)
        {
            printf("Thread creation failed!!/n");
            exit(EXIT_FAILURE);
        }
    }

    srand(time(NULL));
    sleep(run_time);
    return 0;
}
