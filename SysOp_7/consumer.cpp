#include <iostream>
#include <signal.h>
#include <sys/wait.h>
#include "lib/sem.h"
#include "lib/shm.h"

#define A_SEM_PRO argv[1]
#define A_SEM_CON argv[2]
#define A_SHM_NAME argv[3]
#define A_FILE_OUT argv[4]

struct SegmentSHM
{
    char buffor[NOBUF][NOELE];
    int put, takeout;
};

bool if_last_block(SegmentSHM * sSHM)
{
    for (int i = 0; i < NOELE; i++)
        if (sSHM->buffor[sSHM->takeout][i] == '\0')
            return true;
    return false;
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

    if((file = open(A_FILE_OUT, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
    {
        std::cout << "Cannot open file: " << A_FILE_OUT << std::endl;
        perror("Error file open");
        _exit(EXIT_FAILURE);
    }

    sem_pro = sem::open(A_SEM_PRO);
    sem_con = sem::open(A_SEM_CON);
    shm_m = shm::open(A_SHM_NAME);

    sSHM = (SegmentSHM*)shm::map(shm_m, sizeof(SegmentSHM));
    sSHM->takeout = 0;

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(fromToRandom(MIN_TIME, MAX_TIME)));
        sem::decrement(sem_con);
        if (!if_last_block(sSHM))
        {
            if (write (file, sSHM->buffor[sSHM->takeout], NOELE) == -1) {
				perror("Błąd write do potoku");
				_exit(EXIT_FAILURE);
			}
            write(STDOUT_FILENO, "\tConsuming <= ", 14);
            write(STDOUT_FILENO, sSHM->buffor[sSHM->takeout], NOELE);
            std::cout << std::endl;
            //std::cout << "\tConsuming <= (" << strlen(sSHM->buffor[sSHM->takeout]) << ") " << sSHM->buffor[sSHM->takeout] << std::endl;
        }
        else
        {
            if (write(file, sSHM->buffor[sSHM->takeout], strlen(sSHM->buffor[sSHM->takeout])) == -1)
            {
                perror("Error writing");
                _exit(EXIT_FAILURE);
            }
            sem::increment(sem_pro);
            write(STDOUT_FILENO, "\tConsuming <= ", 14);
            write(STDOUT_FILENO, sSHM->buffor[sSHM->takeout], strlen(sSHM->buffor[sSHM->takeout]));
            std::cout << std::endl;
            //std::cout << "\tConsuming <= (" << strlen(sSHM->buffor[sSHM->takeout]) << ") " << sSHM->buffor[sSHM->takeout] << std::endl;
            break; 
        }

        sSHM->takeout = (sSHM->takeout + 1) % NOBUF;
        sem::increment(sem_pro);
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