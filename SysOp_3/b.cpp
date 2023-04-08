#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		std::cout << "Wrong number of arguments were given!" << std::endl;
		exit(EXIT_FAILURE);
	}


	int SIG_ACTION = -1;
	if (strlen(argv[3]) == 1)
		sscanf(argv[3], "%d", &SIG_ACTION);
	std::string SIG_ACTION_STR = std::to_string(SIG_ACTION);

	int SIG_ID = -1;
	if (isdigit(*argv[2]))
		sscanf(argv[2], "%d", &SIG_ID);
	std::string SIG_ID_STR = std::to_string(SIG_ID);

	
	pid_t id_p;
	switch (id_p = fork())
	{
	case -1:
		perror("Fork error");
		exit(-1);
		break;
	case 0:
		if (execl(argv[1], argv[1], SIG_ID_STR.c_str(), SIG_ACTION_STR.c_str(), (char*)NULL) == -1)
		{
			perror("Execl error");
			exit(EXIT_FAILURE);
		}
		break;
	default:
		sleep(1);
		break;
	}
	
	if(kill(id_p, SIG_ID) == -1)
		perror("Kill error");

}
