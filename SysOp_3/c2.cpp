#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    if (argc != 4)
	{
		std::cout << "Wrong number of arguments were given!" << std::endl;
		exit(EXIT_FAILURE);
	}
	int wait_status = 0;

	for (int i = 0; i < 3; i++)
	{
		switch (fork())
		{
		case -1:
			perror("Fork error");
			exit(EXIT_FAILURE);
			break;
		case 0:
			if (execl(argv[1], argv[1], argv[2], argv[3]) == -1)
			{
				perror("Execl error");
				exit(EXIT_FAILURE);
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
		std::cout << "The process waited for all child processes to finish" << std::endl;
	
}