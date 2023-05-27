#include "sem.h"

// Mateusz Kalwa

sem_t* sem::create(const char* name)
{
    sem_t *sem_ptr;
    sem_ptr = sem_open(name, O_CREAT | O_EXCL, 0664, 1);
    
    if (sem_ptr == SEM_FAILED)
    {
        perror("Error creating semaphore");
        exit(EXIT_FAILURE);
    }

    return sem_ptr;
}

sem_t* sem::open(const char* name)
{
    sem_t* sem_ptr;
    sem_ptr = sem_open(name, 0);

    if (sem_ptr == SEM_FAILED)
    {
        perror("Error opening semaphore");
        exit(EXIT_FAILURE);
    }

    return sem_ptr;
}

void sem::close(sem_t* sem)
{
    if (sem_close(sem) == -1)
    {
        perror("Error closing semaphore");
        exit(EXIT_FAILURE);
    }
}

int sem::getValue(sem_t* sem)
{
    int* sem_i = new int(0);
    if (sem_getvalue(sem, sem_i) == -1)
    {
        perror("Error getting value of semaphore");
        exit(EXIT_FAILURE);
    }
    return *sem_i;
}

void sem::increment(sem_t* sem)
{
    if (sem_post(sem) == -1)
    {
        perror("Error incrementing semaphore");
        exit(EXIT_FAILURE); 
    }
}

void sem::decrement(sem_t* sem)
{
    if (sem_wait(sem) == -1)
    {
        perror("Error blocking semaphore");
        exit(EXIT_FAILURE);
    }
}

void sem::delete_s(const char* name)
{
    if (sem_unlink(name) == -1)
    {
        perror("Error deleting semaphore");
        exit(EXIT_FAILURE);
    }
}

int fromToRandom(int a, int b)
{
    return (rand() % (b-a+1)) + a;
}