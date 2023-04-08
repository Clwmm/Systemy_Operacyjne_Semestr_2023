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

int fromToRandom(int a, int b)
{
    return (rand() % (b-a+1)) + a;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
	{
		std::cout << "Wrong number of arguments were given!" << std::endl;
		exit(EXIT_FAILURE);
	}

    int filedes[2];
    int file;

    int reading;
    char bufor[1024];

    const size_t producerDataSize = 8;
    const size_t consumerDataSize = 5;
    const int waitingPeriod[2] = {50, 450}; // in miliseconds

    bool whileLoop = true;
    int wait_status;

    if (pipe(filedes) == -1)
    {
        perror("Pipe error");
        exit(EXIT_FAILURE);
    }

    switch (fork())
    {
    case -1:
        perror("Fork error");
        exit(EXIT_FAILURE);
        break;
    case 0:
        /*CONSUMER*/
        if (close(filedes[1]) == -1)
        {
            perror("Filedes close error");
            exit(EXIT_FAILURE);
        }

        if ((file = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
        {
            perror("File open error");
            exit(EXIT_FAILURE);
        }

        srand(time(NULL));
        sleep(1);
        

        while (whileLoop)
        {
            
            std::this_thread::sleep_for(std::chrono::milliseconds(fromToRandom(waitingPeriod[0], waitingPeriod[1])));
            
            reading = read(filedes[0], &bufor, consumerDataSize);

            switch (reading)
            {
            case -1:
                perror("Reading from stream error");
                exit(EXIT_FAILURE);
                break;
            case 0:
                whileLoop = false;
                break;
            default:
                write(file, &bufor, reading);
                bufor[reading] = '\0';
                std::cout << "Consuming:\t" << bufor << std::endl;
                break;
            }

        }


        if (close(file) == -1)
        {
            perror("File close error");
            exit(EXIT_FAILURE);
        }

        if (close(filedes[0]) == -1)
        {
            perror("Filedes close error");
            exit(EXIT_FAILURE);
        }

        break;

    default:
        /*PRODUCER*/
        if (close(filedes[0]) == -1)
        {
            perror("Filedes close error");
            exit(EXIT_FAILURE);
        }

        if ((file = open(argv[1], O_RDONLY, 0644)) == -1)
        {
            perror("File open error");
            exit(EXIT_FAILURE);
        }

        sleep(1);
        srand(time(NULL));

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
                write(filedes[1], &bufor, reading);
                bufor[reading] = '\0';
                std::cout << "Producing:\t" << bufor << std::endl;
                break;
            }

        }


        if (close(file) == -1)
        {
            perror("File close error");
            exit(EXIT_FAILURE);
        }

        if (close(filedes[1]) == -1)
        {
            perror("Filedes close error");
            exit(EXIT_FAILURE);
        }

        wait(&wait_status);
	    if (WIFSIGNALED(wait_status))
	    {
	    	auto wterm = WTERMSIG(wait_status);
	    	std::cout << "Signal:\t" << strsignal(wterm) << std::endl; 
	    }
	    else
		    std::cout << "The process waited for all child processes to finish" << std::endl;

        break;
    }
}