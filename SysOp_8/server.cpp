#include "lib/meq.h"

void end(void)
{
    meq::delete_s(MEQ_NAME);
}

void custom_signal_handler(int signal)
{
    std::cout << "Deleting queue" << std::endl;
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
    int client_PID;
    char msg[SIZE_MSG];
    char result_str[SIZE_MSG];

    int num1 = 0, num2 = 0;
    char oper;
    int result = 0;

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

    mqd_t m_queue;
    mqd_t r_queue;

    m_queue = meq::create(MEQ_NAME, O_RDONLY);

    {
        mq_attr atr = meq::getattr(m_queue);
        std::cout << "Queue created: " << MEQ_NAME << "\tDes: " << m_queue << std::endl;
        std::cout << "mq_flags: " << atr.mq_flags << std::endl;
        std::cout << "mq_maxmsg: " << atr.mq_maxmsg << std::endl;
        std::cout << "mq_msgsize: " << atr.mq_msgsize << std::endl;
        std::cout << "mq_curmsgs: " << atr.mq_curmsgs << std::endl << std::endl;
    }

    while (true)
    {
        meq::receive(m_queue, msg, NULL);

        sscanf(msg, "%d %d %c %d", &client_PID, &num1, &oper, &num2);
        std::cout << "Recived operation: " << num1 << " " << oper << " " << num2 << " = ";

        switch (oper)
        {
        case '+':
            result = num1 + num2;
            sprintf(result_str, "%d", result);
            break;
        case '-':
            result = num1 - num2;
            sprintf(result_str, "%d", result);
            break;
        case '*':
            result = num1 * num2;
            sprintf(result_str, "%d", result);
            break;
        case '/':
            if (num2 == 0)
            {
                std::cout << "Do not divide by zero!";
                sprintf(result_str, "%s", "Do not divide by zero!");
                break;
            }
            result = num1 / num2;
            sprintf(result_str, "%d", result);
            break;
        default:
            std::cout << "Error operator";
            sprintf(result_str, "%s", "Error operator");
            result = 0;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(fromToRandom(MIN_TIME, MAX_TIME)));

        std::cout << result << std::endl;

        sprintf(msg, "/%d", client_PID);
        r_queue = meq::open(msg, O_WRONLY);
        std::cout << "Sending results" << std::endl;
        meq::send(r_queue, result_str, 1);
        meq::close(r_queue);
        std::cout << std::endl;
    }
    

}