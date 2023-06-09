#include "meq.h"

/*
namespace mqg
{
    mqd_t create(const char* name, int oflag);
    mqd_t open(const char* name, int oflag);
    void send(mqd_t mq_des, const char* msg, unsigned int msg_p);
    void recive(mqd_t mq_des, char* msg, unsigned int msg_p);
    void close(mqd_t mq_des);
    void delete_s(const char* name);
    struct mq_attr getattr(mqd_t mq_des);
}
*/

mqd_t meq::create(const char* name, int oflag)
{
    struct mq_attr attr;
    attr.mq_maxmsg = MAX_MSG;
    attr.mq_msgsize = SIZE_MSG;
    mqd_t des = mq_open(name, O_CREAT | O_EXCL | oflag, 0664, &attr);
    if (des == -1)
    {
        perror("Error creating queue");
        exit(EXIT_FAILURE);
    }
    return des;
}


mqd_t meq::open(const char* name, int oflag)
{
    mqd_t des = mq_open(name, oflag);
    if (des == -1)
    {
        perror("Error openning queue");
        exit(EXIT_FAILURE);
    }
    return des;
}

void meq::send(mqd_t mq_des, const char* msg, unsigned int msg_p)
{
    if (mq_send(mq_des, msg, SIZE_MSG, msg_p) == -1)
    {
        perror("Error sending msg");
        exit(EXIT_FAILURE);
    }
}

void meq::receive(mqd_t mq_des, char* msg, unsigned int* msg_p)
{
    if (mq_receive(mq_des, msg, SIZE_MSG, msg_p) == -1)
    {
        perror("Error reciving msg");
        exit(EXIT_FAILURE);
    }
}

void meq::close(mqd_t mq_des)
{
    if (mq_close(mq_des) == -1)
    {
        perror("Error closing queue");
        exit(EXIT_FAILURE);
    }
}

void meq::delete_s(const char* name)
{
    if (mq_unlink(name) == -1)
    {
        perror("Error unlinking queue");
        exit(EXIT_SUCCESS);
    }
}

mq_attr meq::getattr(mqd_t mq_des)
{
    struct mq_attr attr;
    if (mq_getattr(mq_des, &attr) == -1)
    {
        perror("Error getting attr");
        exit(EXIT_FAILURE);
    }
    return attr;
}

int fromToRandom(int a, int b)
{
    return (rand() % (b-a+1)) + a;
}