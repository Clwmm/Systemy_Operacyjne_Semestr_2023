#ifndef MEQ_H
#define MEQ_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#include <time.h>
#include <chrono>
#include <thread>

#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <sys/wait.h>

#define MEQ_NAME "/my_queue_name"
#define MAX_MSG 10
#define SIZE_MSG 30

#define MIN_TIME 500
#define MAX_TIME 900

namespace meq
{
    mqd_t create(const char* name, int oflag);
    mqd_t open(const char* name, int oflag);
    void send(mqd_t mq_des, const char* msg, unsigned int msg_p);
    void receive(mqd_t mq_des, char* msg, unsigned int* msg_p);
    void close(mqd_t mq_des);
    void delete_s(const char* name);
    struct mq_attr getattr(mqd_t mq_des);
}

int fromToRandom(int a, int b);

#endif