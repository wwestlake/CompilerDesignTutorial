

#include <iostream>
#include <thread>
#include <mutex>
#include "MessageThread.hpp"

int state1 = 0;
int state2 = 0;
std::mutex mutex;

void update_state_thread()
{
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            state1++;
            state2++;
        }
    }
}

void read_state_thread()
{
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (state1 != state2)
            {
                std::cout << state1 << " " << state2 << std::endl;
            } else {
                std::cout << "ok" << std::endl;

            }
        }
    }
}





int _main(void)
{
    std::thread update_thread(update_state_thread);
    std::thread read_thread(read_state_thread);

    update_thread.join();
    read_thread.join();


    return 0;
}

int main(void)
{
    MessageThread<int> msgThr;

    msgThr.Subscribe([](Message<int> msg) {
        std::cout << "Got message " << msg.getMessage() << std::endl;
    });

    msgThr.Subscribe([](Message<int> msg) {
        std::cout << "Second Sub Got message " << msg.getMessage() << std::endl;
    });


    

    for (int i = 0; i < 100; i++)
    {
        Message<int> myMessage_1(i);
        msgThr.SendMessage(myMessage_1);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    msgThr.SendMessage(CommandMessage(ThreadCommands::SHUTDOWN));

    msgThr.Join();


    return 0;
}
