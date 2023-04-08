#include <iostream>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void custom_sighandler(int sig_int)
{
	std::cout << "[Custom signal handling]\nSIG_INT:\t" << sig_int << "\nSignal: \t" << strsignal(sig_int) << std::endl;
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Wrong number of arguments were given!" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	if (strlen(argv[2]) != 1)
		argv[2][0] = '0';

	int SIG_INT = -1;
	if (isdigit(*argv[1]))
		sscanf(argv[1], "%d", &SIG_INT);

	std::cout << "\tPID: " << getpid() << std::endl;

	switch (argv[2][0])
	{
	case '1':
		if (signal(SIG_INT, SIG_DFL) == SIG_ERR)
		{
			perror("Default signal");
			exit(EXIT_FAILURE);
		}
		break;
	case '2':
		if (signal(SIG_INT, SIG_IGN) == SIG_ERR)
		{
			perror("Ignore signal");
			exit(EXIT_FAILURE);
		}
		break;
	case '3':
		if (signal(SIG_INT, custom_sighandler) == SIG_ERR)
		{
			perror("Custome signal");
			exit(EXIT_FAILURE);
		}
		break;
	default:
		std::cout << "Wrong argument were given!" << std::endl;
		return 0;
	}
	pause();
}
