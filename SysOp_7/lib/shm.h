#ifndef SHM_H
#define SHM_H

// Mateusz Kałwa

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define NOELE 8
#define NOBUF 5

#define MIN_TIME 50
#define MAX_TIME 150

namespace shm
{
    int create(const char* name);
    int open(const char* name);
    void close_s(const int& fd);
    void set_lenght(const int& fd, const off_t& lenght);
    void delete_s(const char* name);
    void* map(const int& fd, const size_t& lenght);
    void unmap_s(void* addr, const size_t& lenght);
}

#endif