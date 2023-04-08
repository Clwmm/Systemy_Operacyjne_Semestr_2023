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
            displayID();
            break;
        default:
            std::cout << "Iteration: " << i << std::endl;
            sleep(5 - i);
            break;
        }
    }

    if (parent_id == getpid())
        return 0;

    _exit(0);
}

/*
PID: 597 PPID: 9 PGID: 597
{
    PID: 598 PPID: 597 PGID: 597
    {
        PID: 599 PPID: 598 PGID: 597
        {
            PID: 600 PPID: 599 PGID: 597
        }

        PID: 601 PPID: 598 PGID: 597
    }

    PID: 602 PPID: 597 PGID: 597
    {
        PID: 603 PPID: 602 PGID: 597
    }

    PID: 604 PPID: 597 PGID: 597
}
*/
