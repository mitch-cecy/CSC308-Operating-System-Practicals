#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t semaphore;

void* worker(void* arg)
{
    int id = *(int*)arg;

    sem_wait(&semaphore);

    printf("Thread %d entered the critical section.\n", id);

    sleep(2);

    printf("Thread %d leaving the critical section.\n", id);

    sem_post(&semaphore);

    return NULL;
}

int main()
{
    pthread_t threads[5];
    int id[5];

    sem_init(&semaphore, 0, 3);

    for(int i = 0; i < 5; i++)
    {
        id[i] = i + 1;
        pthread_create(&threads[i], NULL, worker, &id[i]);
    }

    for(int i = 0; i < 5; i++)
        pthread_join(threads[i], NULL);

    sem_destroy(&semaphore);

    return 0;
}