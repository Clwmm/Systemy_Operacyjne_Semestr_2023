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
    argv[1] - file - out
    argv[2] - pipe
*/

int fromToRandom(int a, int b)
{
    return (rand() % (b-a+1)) + a;
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    sleep(1);

    char bufor[1024];

    int file;
    int reading;
    int pipe_;

    const size_t consumerDataSize = 5;
    const int waitingPeriod[2] = {50, 450}; // in miliseconds

    bool whileLoop = true;

    if ((file = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
    {
        perror("File open error");
        _exit(EXIT_FAILURE);
    }

    std::cout << "c.cpp pipe: " << argv[2] << std::endl;

    if ((pipe_ = open(argv[2], O_WRONLY, 0666)) == -1)
    {
        perror("Pipe open error");
        _exit(EXIT_FAILURE);
    }
    std::cout << "Pipe opened c.cpp" << std::endl;

    
    while (whileLoop)
    {
            
        std::this_thread::sleep_for(std::chrono::milliseconds(fromToRandom(waitingPeriod[0], waitingPeriod[1])));
            
        reading = read(pipe_, &bufor, consumerDataSize);

        switch (reading)
        {
        case -1:
            perror("Reading from stream error");
            _exit(EXIT_FAILURE);
            break;
        case 0:
            whileLoop = false;
            break;
        default:
            if (write(file, &bufor, reading) == -1)
            {
                perror("Write to file error");
                _exit(EXIT_FAILURE);
            }
            bufor[reading] = '\0';
            std::cout << "Consuming:\t" << bufor << std::endl;
            break;
        }

    }


    if (close(file) == -1)
    {
        perror("File close error");
        _exit(EXIT_FAILURE);
    }

    if (close(pipe_) == -1)
    {
        perror("Pipe close error");
        _exit(EXIT_FAILURE);
    }

}