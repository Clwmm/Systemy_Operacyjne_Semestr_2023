// Mateusz Ka³wa

#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void displayID()
{
    std::cout << "UID: " << getuid() << " GID: " << getgid() << " PID: " << getpid() << " PPID: " << getppid() << " PGID: " << getpgid(0) << std::endl;
}

int main()
{
    displayID();
    _exit(0);
}