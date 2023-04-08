// Mateusz Ka�wa

#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    

    std::cout << "Procesy potomne: " << std::endl;
    for (int i = 0; i < 3; i++)
    {
        switch (fork())
        {
        case -1:
            perror("Fork error");
            break;
        case 0:
            execl(argv[1], argv[1], (char*)NULL);
            break;
        default:
            wait(NULL);
            break;
        }
    }

    std::cout << "Proces macierzysty: " << std::endl;
    execl(argv[1], argv[1], (char*)NULL);
    
}