#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>

#define SHM_SIZE 1024

int main()
{
    int shmid;
    char *shared_memory;

    sem_t *mutex;

    shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);

    if (shmid < 0)
    {
        printf("Shared memory creation failed.\n");
        return 1;
    }

    mutex = sem_open("/mySemaphore", O_CREAT, 0644, 1);

    pid_t pid = fork();

    if(pid == 0)
    {
        shared_memory = (char*)shmat(shmid, NULL, 0);

        sem_wait(mutex);

        strcpy(shared_memory, "Hello from Child Process!");

        printf("Child wrote: %s\n", shared_memory);

        sem_post(mutex);

        shmdt(shared_memory);
    }
    else
    {
        wait(NULL);

        shared_memory = (char*)shmat(shmid, NULL, 0);

        sem_wait(mutex);

        printf("Parent read: %s\n", shared_memory);

        sem_post(mutex);

        shmdt(shared_memory);

        shmctl(shmid, IPC_RMID, NULL);

        sem_close(mutex);
        sem_unlink("/mySemaphore");
    }

    return 0;
}