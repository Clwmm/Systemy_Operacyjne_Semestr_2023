#include <iostream>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <limits.h>
#include <sys/param.h>
#include <string.h>
#include <sys/wait.h>

/*
    argv[1] - file - in
    argv[2] - pipe
*/

int fromToRandom(int a, int b)
{
    return (rand() % (b-a+1)) + a;
}

int main(int argc, char* argv[])
{
    sleep(1);
    srand(time(NULL));

    char bufor[1024];

    int file;
    int reading;
    int myfifo;

    const size_t producerDataSize = 8;
    const int waitingPeriod[2] = {50, 450}; // in miliseconds

    bool whileLoop = true;

    if ((file = open(argv[1], O_RDONLY)) == -1)
    {
        perror("File open error");
        _exit(EXIT_FAILURE);
    }

    std::cout << "b.cpp pipe: " << argv[2] << std::endl;

    if ((myfifo = open(argv[2], O_WRONLY)) == -1)
    {
        perror("Pipe open error");
        _exit(EXIT_FAILURE);
    }
    std::cout << "Pipe opened b.cpp" << std::endl;

    
    while (whileLoop)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(fromToRandom(waitingPeriod[0], waitingPeriod[1])));
        reading = read(file, &bufor, producerDataSize);
        switch (reading)
        {
        case -1:
            perror("Reading from file error");
            exit(EXIT_FAILURE);
            break;
        case 0:
            whileLoop = false;
            break;
        default:
            if (write(myfifo, &bufor, reading) == -1)
            {
                perror("Write to pipe error");
                _exit(EXIT_FAILURE);
            }
            bufor[reading] = '\0';
            std::cout << "Producing:\t" << bufor << std::endl;
            break;
        }

    }


    if (close(file) == -1)
    {
        perror("File close error");
        _exit(EXIT_FAILURE);
    }

    if (close(myfifo) == -1)
    {
        perror("Pipe close error");
        _exit(EXIT_FAILURE);
    }

}