// Mateusz Ka³wa

#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t displayID()
{
    std::cout << "UID: " << getuid() << " GID: " << getgid() << " PID: " << getpid() << " PPID: " << getppid() << " PGID: " << getpgid(0) << std::endl;
    return getpid();
}

int main()
{
    std::cout << "Proces macierzysty: " << std::endl;
    pid_t parent_id = displayID();


    std::cout << "Procesy potomne: " << std::endl;
    for (int i = 0; i < 3; i++)
    {
        switch (fork())
        {
        case -1:
            perror("Fork error");
            break;
        case 0:
            if (setpgid(0, 0) == -1)
            {
                perror("Setpgid");
                exit(-1);
            }
            displayID();
            break;
        default:
            wait(NULL);
            break;
        }
    }

    if (parent_id == getpid())
        return 0;

    _exit(0);
}