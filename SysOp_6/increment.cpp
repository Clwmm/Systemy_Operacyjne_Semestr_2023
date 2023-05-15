#include "sem.h"

/*
    argv[1] - number of sections
    argv[2] - file name
    argv[3] - semaphore name
    argv[4] - synchronization
*/

#define A_NOSECTIONS argv[1]
#define A_FILENAME argv[2]
#define A_SEMAPHORE_NAME argv[3]
#define A_SYNCHRONIZATION argv[4]

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        std::cout << "Specified wrong number of arguments" << std::endl;
        _exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    int nOsections = 0;

    sem_t* sem_ptr;

    int file;
    int reading;
    char bufor[100];
    int number = 0;

    bool synchro = false;

    nOsections = atoi(A_NOSECTIONS);
    if (atoi(A_SYNCHRONIZATION) == 1)
        synchro = true;

    int waitingPeriod[2] = { 15, 200 };
    if (!synchro)
    {
        waitingPeriod[0] = 15;
        waitingPeriod[1] = 700;
    }

    if (synchro)
        sem_ptr = sem::open(A_SEMAPHORE_NAME);

    for (int i = 0; i < nOsections; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(fromToRandom(waitingPeriod[0], waitingPeriod[1])));

        if (synchro)
        {
            sem::decrement(sem_ptr);
            std::cout << "PID: " << getpid() << "\t Semaphore value: " << sem::getValue(sem_ptr) << std::endl;
        }

        if ((file = open(A_FILENAME, O_RDONLY, 0644)) == -1)
        {
            std::cout << "Cannot open file: " << A_FILENAME << std::endl;
            perror("File error");
            _exit(EXIT_FAILURE); 
        }

        reading = read(file, &bufor, 10);

        if (reading == -1)
        {
            perror("Error reading from file");
            _exit(EXIT_FAILURE);
        }
        else if (reading == 0)
            break;
        else
        {
            bufor[reading] = '\0';
            std::cout << "PID: " << getpid() << "\t Semaphore value: " << sem::getValue(sem_ptr);
            std::cout << "\tBufor: " << bufor << "\tIteration: " << i << std::endl;
        }

        if (close(file) == -1)
        {
            perror("Error closing file");
            _exit(EXIT_FAILURE);
        }

        number = atoi(bufor);
        number++;
        sprintf(bufor, "%d", number);

        std::this_thread::sleep_for(std::chrono::milliseconds(fromToRandom(waitingPeriod[0], waitingPeriod[1])));

        if ((file = open(A_FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
        {
            std::cout << "Cannot open file: " << A_FILENAME << std::endl;
            perror("File error");
            _exit(EXIT_FAILURE);
        }

        if (write(file, &bufor, strlen(bufor)) == -1)
        {
            perror("Error writing to file");
            _exit(EXIT_FAILURE);
        }

        if (close(file) == -1)
        {
            perror("Error closing file");
            _exit(EXIT_FAILURE);
        }

        if (synchro)
        {
            sem::increment(sem_ptr);
            std::cout << "PID: " << getpid() << "\t Semaphore value: " << sem::getValue(sem_ptr) << std::endl;
        }

    }

    // if (synchro)
    //     sem::delete_s(A_SEMAPHORE_NAME);
}