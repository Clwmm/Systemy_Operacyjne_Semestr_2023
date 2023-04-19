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
    argv[1] - b.cpp - producer
    argv[2] - c.cpp - consumer
    argv[3] - in
    argv[4] - out
    argv[5] - pipe
*/

char pipe_[30];

void end(void)
{
    if (unlink(pipe_) == -1)
        perror("Unlink error");
    else
        std::cout << "myfifo deleted" << std::endl;
}

void exit_signal(int signal)
{
    std::cout << "Process closed" << std::endl;
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
    if (argc != 6)
	{
		std::cout << "Wrong number of arguments were given!" << std::endl;
		exit(EXIT_FAILURE);
	}
    strcpy(pipe_, argv[5]);

    int wait_status;

    if (mkfifo(argv[5], 0644) == -1)
    {
        perror("Make fifo error");
        _exit(EXIT_FAILURE);
    }

    if (atexit(end) != 0)
    {
        perror("Atexit error");
        _exit(EXIT_FAILURE);
    }

    if (signal(SIGINT, exit_signal) == SIG_ERR)
    {
        perror("Signal error");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 2; i++)
    {
        switch (fork())
        {
        case -1:
            perror("Fork error");
            exit(EXIT_FAILURE);
            break;
        case 0:
            switch (i)
            {
            case 0:
                // Producer
                if (execl(argv[1], argv[1], argv[3], argv[5], (char*)NULL) == -1)
		        {
			        perror("Execl error");
			        exit(EXIT_FAILURE);
		        }
                break;
            case 1:
                // Consumer
                if (execl(argv[2], argv[2], argv[4], argv[5], (char*)NULL) == -1)
		        {
			        perror("Execl error");
			        exit(EXIT_FAILURE);
		        }
                break;
            }
            break;
        default:
            break;
        }
    }
    


    wait(&wait_status);
	if (WIFSIGNALED(wait_status))
	{
		auto wterm = WTERMSIG(wait_status);
    	std::cout << "Signal:\t" << strsignal(wterm) << std::endl; 
	}
    else
	    std::cout << "The process waited for child processes to finish" << std::endl;

    wait(&wait_status);
	if (WIFSIGNALED(wait_status))
	{
		auto wterm = WTERMSIG(wait_status);
    	std::cout << "Signal:\t" << strsignal(wterm) << std::endl; 
	}
    else
	    std::cout << "The process waited for child processes to finish" << std::endl;
}