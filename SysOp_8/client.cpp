#include "lib/meq.h"

mqd_t server;

void end(void)
{
    meq::close(server);
}

void custom_signal_handler(int signal)
{
    std::cout << "Closing queue" << std::endl;
    exit(EXIT_SUCCESS);
}

int main()
{
    char queue_msg_name[10];
    mqd_t queue_msg;
    char read[SIZE_MSG];
    char str[SIZE_MSG + 10];

    sprintf(queue_msg_name, "/%d", getpid());

    if (atexit(end) != 0)
    {
        perror("Error atexit");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGINT, custom_signal_handler) == SIG_ERR)
    {
        perror("Error signal");
        exit(EXIT_FAILURE);
    }

    server = meq::open(MEQ_NAME, O_WRONLY);
    
    {
        mq_attr atr = meq::getattr(server);
        std::cout << "Queue created: " << MEQ_NAME << "\tDes: " << server << std::endl;
        std::cout << "mq_flags: " << atr.mq_flags << std::endl;
        std::cout << "mq_maxmsg: " << atr.mq_maxmsg << std::endl;
        std::cout << "mq_msgsize: " << atr.mq_msgsize << std::endl;
        std::cout << "mq_curmsgs: " << atr.mq_curmsgs << std::endl << std::endl;
    }

    while (true)
    {
        std::cout << "Enter a operation: " << std::endl;
        if (scanf("%s", read) == EOF)
        {
            std::cout << "End client proccess" << std::endl;
            exit(EXIT_SUCCESS);
        }

        sprintf(str, "%d %s", getpid(), read);

        meq::send(server, str, 1);
        std::cout << "Send to server" << std::endl;

        queue_msg = meq::create(queue_msg_name, O_RDONLY);

        std::cout << "Waiting for a responde from the server..." << std::endl;
        meq::receive(queue_msg, str, NULL);
        std::cout << "Recived:\t" << str << std::endl;

        meq::close(queue_msg);
        meq::delete_s(queue_msg_name);

        std::cout << std::endl;


        std::this_thread::sleep_for(std::chrono::milliseconds(fromToRandom(MIN_TIME, MAX_TIME)));
    }
    
}