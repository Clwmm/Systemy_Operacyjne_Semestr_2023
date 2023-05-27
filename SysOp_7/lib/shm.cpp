#include "shm.h"

// Mateusz Kałwa

/*
    int create(const char* name);
    int open(const char* name);
    void close(const int& fd);
    void set_lenght(const int& fd, const off_t& lenght);
    void delete_s(const char* name);
    void map(const int& fd, const size_t& lenght);
    void unmap(void* addr, const size_t& lenght);
*/

int shm::create(const char* name)
{
    int descriptor;
    if ((descriptor = shm_open(name, O_EXCL | O_RDWR | O_CREAT, 0644)) == -1)
    {
        perror("Error creating shared memory");
        exit(EXIT_FAILURE);
    }
    return descriptor;
}

int shm::open(const char* name)
{
    int descriptor;
    if ((descriptor = shm_open(name, O_RDWR, 0644)) == -1)
    {
        perror("Error opening shared memory");
        exit(EXIT_FAILURE);
    }
    return descriptor;
}

void shm::close_s(const int& fd)
{
    if (close(fd) == -1)
    {
        perror("Error closing shared memory");
        exit(EXIT_FAILURE);
    }
}

void shm::set_lenght(const int& fd, const off_t& lenght)
{
    if (ftruncate(fd, lenght) == -1)
    {
        perror("Error can not set lenght");
        exit(EXIT_FAILURE);
    }
}

void shm::delete_s(const char* name)
{
    if (shm_unlink(name) == -1)
    {
        perror("Error deleting shared memory");
        exit(EXIT_FAILURE);
    }
}

void* shm::map(const int& fd, const size_t& lenght)
{
    void* addr = mmap(NULL, lenght, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("Error maping");
        exit(EXIT_FAILURE);
    }
    return addr;
}

void shm::unmap_s(void* addr, const size_t& lenght)
{
    if (munmap(addr, lenght) == -1)
    {
        perror("Error unmapping");
        exit(EXIT_FAILURE);
    }
}