#ifndef SEM_H
#define SEM_H

#include <iostream>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <time.h>
#include <chrono>
#include <thread>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

namespace sem
{
    sem_t* create(const char* name);
    sem_t* open(const char* name);
    void close(sem_t* sem);
    int getValue(sem_t* sem);
    void increment(sem_t* sem);
    void decrement(sem_t* sem);
    void delete_s(const char* name);
}

int fromToRandom(int a, int b);

#endif