#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "buffer.h"

buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t full, empty;
int count, in, out;

int insert_item(buffer_item item)
{
    /* insert item into buffer
    return 0 if successful, otherwise
    return -1 indicating an error condition */
    int success;
    /*sem_wait會等待semaphore大於0之後將其減1,
    若semaphore為0就將該thread暫停,直到其大於0*/
    sem_wait(&empty);
    /*pthread_mutex_lock會將指定的mutex鎖住,
    若有其他的函數2對同樣的mutex呼叫此函數就將函數2暫停,
    直到該mutex解鎖為止*/
    pthread_mutex_lock(&mutex);

    if(count != BUFFER_SIZE)
    {
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        success = 0;
    }
    else
        success = -1;

    pthread_mutex_unlock(&mutex);
    //sem_post將semaphore加1
    sem_post(&full);
    return success;
}

int remove_item(buffer_item *item)
{
    /* remove an object from buffer
    placing it in item
    return 0 if successful, otherwise
    return -1 indicating an error condition */
    int success;
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    if(count != 0)
    {
        *item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        success = 0;
    }
    else
        success = -1;

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    return success;
}

void *producer(void *param)
{
    buffer_item item;
    while(1)
    {
        sleep(rand() % 5 + 1);

        item = rand();
        if(insert_item(item))
            printf("Error inserted\n");
        else
            printf("Producer produced %d\n", item);
    }
}


void *consumer(void *param)
{
    buffer_item item;
    while(1)
    {
        sleep(rand() % 5 + 1);

        if(remove_item(&item))
            printf("Error occured\n");
        else
            printf("Consumer consumed %d\n", item);
    }
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("INPUT ERROR: Please input delay, number of producers and number of consumers.\n");
        exit(1);
    }

    int i;
    //long int strtol(const char *str, char **endptr, int base)
    long int runtime = strtol(argv[1], NULL, 0);
    long int num_producer = strtol(argv[2], NULL, 0);
    long int num_consumer = strtol(argv[3], NULL, 0);
    /*int sem_init(sem_t *sem, int pshared, unsigned int value)
    pshared = zero表示此semaphore僅由此process底下的threads共享
    pshared = nonzero表示此semaphore由多processes共享*/
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    /*int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr)
    attr = NULL表示建立預設的mutex屬性*/
    pthread_mutex_init(&mutex, NULL);

    /*int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
    arg是傳遞給start_routine的參數*/
    pthread_t producers[num_producer];
    for(i = 0; i < num_producer; i++)
        pthread_create(&producers[i], NULL, producer, NULL);

    pthread_t consumers[num_consumer];
    for(i = 0; i < num_consumer; i++)
        pthread_create(&consumers[i], NULL, consumer, NULL);

    srand(time(NULL));
    count = in = out = 0;
    sleep(runtime);
    return 0;
}
