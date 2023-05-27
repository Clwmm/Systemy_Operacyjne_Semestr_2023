#include <iostream>
#include <signal.h>
#include <sys/wait.h>
#include "lib/sem.h"
#include "lib/shm.h"

#define A_SEM_PRO argv[1]
#define A_SEM_CON argv[2]
#define A_SHM_NAME argv[3]
#define A_FILE_IN argv[4]

struct SegmentSHM
{
    char buffor[NOBUF][NOELE];
    int put, takeout;
};

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        std::cout << "Specified wrong number of arguments" << std::endl;
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    int file;
    int shm_m;
    SegmentSHM* sSHM;
    sem_t* sem_pro;
    sem_t* sem_con;
    int reading;

    if((file = open(A_FILE_IN, O_RDONLY, 0644)) == -1)
    {
        std::cout << "Cannot open file: " << A_FILE_IN << std::endl;
        perror("Error file open");
        _exit(EXIT_FAILURE);
    }

    sem_pro = sem::open(A_SEM_PRO);
    sem_con = sem::open(A_SEM_CON);
    shm_m = shm::open(A_SHM_NAME);

    sSHM = (SegmentSHM*)shm::map(shm_m, sizeof(SegmentSHM));
    sSHM->put = 0;

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(fromToRandom(MIN_TIME, MAX_TIME)));

        sem::decrement(sem_pro);
        reading = read(file, sSHM->buffor[sSHM->put], NOELE);
        
        
        if (reading == -1)
        {
            std::cout << "Error reading from file" << std::endl;
            _exit(EXIT_FAILURE);
        }
        if (reading != NOELE)
        {
            sSHM->buffor[sSHM->put][reading] = '\0';
            write(STDOUT_FILENO, "Producer => ", 12);
            write(STDOUT_FILENO, sSHM->buffor[sSHM->put], strlen(sSHM->buffor[sSHM->put]));
            std::cout << std::endl;
            //std::cout << "Producer => " << "(" << strlen(sSHM->buffor[sSHM->put]) << ")" << "\t" << sSHM->buffor[sSHM->put] << std::endl;
            sem::increment(sem_con);
            break;
        }
        else
        {
            write(STDOUT_FILENO, "Producer => ", 12);
            
            
            write(STDOUT_FILENO, sSHM->buffor[sSHM->put], NOELE);
            std::cout << std::endl;
            //std::cout << "Producer => (" << strlen(sSHM->buffor[sSHM->put]) << ") " << sSHM->buffor[sSHM->put] << std::endl;
        }
        sSHM->put = (sSHM->put + 1) % NOBUF;

        sem::increment(sem_con);
    }
    
    if (close(file) == -1)
    {
        perror("Error file closing");
        _exit(EXIT_FAILURE);
    }

    shm::unmap_s(sSHM, sizeof(SegmentSHM));
    shm::close_s(shm_m);
    sem::close(sem_pro);
    sem::close(sem_con);
}