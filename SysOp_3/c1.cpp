#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <cstdlib>
#include <sys/wait.h>

/*
	argv[1] - c2.cpp
	argv[2] - a.cpp
	argv[3] - SIG_ID
	argv[4] - SIG_ACTION
*/

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		std::cout << "Wrong number of arguments were given!" << std::endl;
		exit(EXIT_FAILURE);
	}
	int child_PID = 0;
	int child_PGID = 0;
	int wait_status = 0;

	int SIG_ACTION = -1;
	if (strlen(argv[4]) == 1)
		sscanf(argv[4], "%d", &SIG_ACTION);
	std::string SIG_ACTION_STR = std::to_string(SIG_ACTION);

	int SIG_ID = -1;
	if (isdigit(*argv[3]))
		sscanf(argv[3], "%d", &SIG_ID);
	std::string SIG_ID_STR = std::to_string(SIG_ID);

	switch (child_PID = fork())
	{
	case -1:
		perror("Fork error");
		exit(EXIT_FAILURE);
	case 0:
		setpgid(child_PID, 0);
		signal(SIG_ID, SIG_IGN);
		if (execl(argv[1], argv[1], argv[2], SIG_ID_STR.c_str(), SIG_ACTION_STR.c_str(), (char*)NULL) == -1)
		{
			perror("Execl error");
			exit(EXIT_FAILURE);
		}

		break;
	default:
		sleep(1);
		child_PGID = getpgid(child_PID);
		if(kill(-child_PGID, 0) == -1)
		{
			perror("Child does not exist");
			exit(EXIT_FAILURE);
		}
		if(kill(-child_PGID, SIG_ID) == -1)
		{
			perror("Kill error");
			exit(EXIT_FAILURE);
		}
		break;
	}

	sleep(1);

	wait(&wait_status);
	if (WIFSIGNALED(wait_status))
	{
		auto wterm = WTERMSIG(wait_status);
		std::cout << "c2.cpp PID:\t" << child_PID << std::endl;
		std::cout << "Signal:\t" << strsignal(wterm) << std::endl; 
	}
	else
		std::cout << "The process waited for all child processes to finish" << std::endl;

}
