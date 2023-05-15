#include "sem.h"
#include <signal.h>
#include <sys/wait.h>

/*
    argv[1] - incement program name
    argv[2] - number of increment programs
    argv[3] - number of sections
    argv[4] - file name
    argv[5] - semaphore name
    argv[6] - synchronization
*/

#define A_IPROGRAM_NAME argv[1]
#define A_NOPROGRAMS argv[2]
#define A_NOSECTIONS argv[3]
#define A_FILENAME argv[4]
#define A_SEMAPHORE_NAME argv[5]
#define A_SYNCHRONIZATION argv[6]

bool synchro = false;
char semaphore_name[15];

void end(void)
{
    if (synchro)
    {
        sem::delete_s(semaphore_name);
        std::cout << "Deleting semaphore" << std::endl;
    }
}

void custom_handler(int signal)
{
    if (synchro)
    {
        std::cout << "Deleting semaphore" << std::endl;
        sem::delete_s(semaphore_name);
    }
    _exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
    if (argc != 7)
    {
        std::cout << "Specified wrong number of arguments" << std::endl;
        _exit(EXIT_FAILURE);
    }

    if (atexit(end) != 0)
    {
        perror("Error atexit");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGINT, custom_handler) == SIG_ERR)
    {
        perror("Error signal");
        exit(EXIT_FAILURE);
    }

    int nOPrograms = atoi(A_NOPROGRAMS);
    strcpy(semaphore_name, A_SEMAPHORE_NAME);

    int file;
    int stat_lock;

    if (atoi(A_SYNCHRONIZATION) == 1)
        synchro = true;

    if ((file = open(A_FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
    {
        std::cout << "Cannot open/create file: " << A_FILENAME << std::endl;
        perror("Error file");
        exit(EXIT_FAILURE);
    }

    if (write(file, "0", 2) == -1)
    {
        perror("Error writing to file");
        exit(EXIT_FAILURE);
    }

    if (close(file) == -1)
    {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    if (synchro)
    {
        sem_t* sem_ptr = sem::create(A_SEMAPHORE_NAME);
        std::cout << "Created semaphore: " << sem_ptr << "\tValue: " << sem::getValue(sem_ptr) << std::endl;
    }

    for (int i = 0; i < nOPrograms; ++i)
    {
        switch (fork())
        {
        case -1:
            perror("Error fork");
            exit(EXIT_FAILURE);
            break;
        
        case 0:
            execl(A_IPROGRAM_NAME, A_IPROGRAM_NAME, A_NOSECTIONS, A_FILENAME, A_SEMAPHORE_NAME, A_SYNCHRONIZATION);
            perror("Error execl");
            exit(EXIT_FAILURE);
            break;

        default:
            break;
        }
    }
    
    for (int i = 0; i < nOPrograms; ++i)
    {
        if (wait(&stat_lock) == -1)
        {
            perror("Error wait");
            exit(EXIT_FAILURE);
        }
    }

    if ((file = open(A_FILENAME, O_RDONLY, 0644)) == -1)
    {
        std::cout << "Cannot open file: " << A_FILENAME << std::endl;
        perror("Error file");
        exit(EXIT_FAILURE);
    }

    int reading;
    char bufor[100];
    
    reading = read(file, &bufor, 90);
    switch (reading)
    {
    case -1:
        perror("Error reading from file");
        exit(EXIT_FAILURE);
    case 0:
        std::cout << "Error: File is empty" << std::endl;
        exit(EXIT_FAILURE);
    default:
        bufor[reading] = '\0';
        int expectation = atoi(A_NOPROGRAMS) * atoi(A_NOSECTIONS);
        int reality = atoi(bufor);

        if (reality == expectation)
            std::cout << "Everything worked correctly\n\tGot: " << reality << std::endl;
        else
            std::cout << "Something went wrong!\n\tExpected: " << expectation << "\tGot: " << reality << std::endl;
        break;
    }

    if (close(file) == -1)
    {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }
}