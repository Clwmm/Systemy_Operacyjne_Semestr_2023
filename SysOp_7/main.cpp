#include <iostream>
#include <signal.h>
#include <sys/wait.h>
#include "lib/sem.h"
#include "lib/shm.h"

#define A_PRODUCER argv[1]
#define A_CONSUMER argv[2]
#define A_FILE_IN argv[3]
#define A_FILE_OUT argv[4]

#define SEM_PRO "/sem_p"
#define SEM_CON "/sem_c"
#define SHM_NAME "/shared_m"

struct SegmentSHM
{
    char buffor[NOBUF][NOELE];
    int put, takeout;
};

void end(void)
{
    shm::delete_s(SHM_NAME);
    sem::delete_s(SEM_PRO);
    sem::delete_s(SEM_CON);
}

void custom_signal_handler(int signal)
{
    std::cout << "Deleting semaphore and shared memory" << std::endl;
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        std::cout << "Specified wrong number of arguments" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (atexit(end) != 0)
    {
        perror("Error atexit");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGINT, custom_signal_handler) == SIG_ERR)
    {
        perror("Error signal");
        exit(EXIT_FAILURE);
    }

    int shm_m;
    int stat_lock;
    sem_t* sem_m;

    sem_m = sem::create(SEM_PRO);
    sem_m = sem::create(SEM_CON);
    sem::decrement(sem_m);

    shm_m = shm::create(SHM_NAME);
    shm::set_lenght(shm_m, sizeof(SegmentSHM));

    srand(time(NULL));

    for (int i = 0; i < 2; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        switch (fork())
        {
        case -1:
            perror("Error fork");
            exit(EXIT_FAILURE);
            break;

        case 0:
            switch (i)
            {
            case 0:
                if (execl(A_PRODUCER, A_PRODUCER, SEM_PRO, SEM_CON, SHM_NAME, A_FILE_IN,(char*)NULL) == -1)
                {
                    perror("Error execl");
                    exit(EXIT_FAILURE);
                }
                break;
            case 1:
                if (execl(A_CONSUMER, A_CONSUMER, SEM_PRO, SEM_CON, SHM_NAME, A_FILE_OUT,(char*)NULL) == -1)
                {
                    perror("Error execl");
                    exit(EXIT_FAILURE);
                }
                break;
            }
            break;
        default:
            break;
        }
    }
    
    for (int i = 0; i < 2; i++)
    {
        if (wait(&stat_lock) == -1)
        {
            perror("Error wait");
            _exit(EXIT_FAILURE);
        }
    }

    shm::close_s(shm_m);
    
}