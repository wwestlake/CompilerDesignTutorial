#ifndef _MESSAGE_THREAD_HPP
#define _MESSAGE_THREAD_HPP

#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <vector>
#include <condition_variable>

template <typename Tmessage>
class Message {
private:
    Tmessage _message;

public:
    Message(Tmessage message) : _message(message) {}

    Message(const Message& message) : _message(message._message) {}
    Message(Message&& message) noexcept : _message(std::move(message._message)) {}
    Message& operator =(const Message& message)
    {
        message._message = message;
        return *this;
    }

    Tmessage getMessage() { return _message; }

};

enum class ThreadCommands {
    SHUTDOWN
};

class CommandMessage : public Message<ThreadCommands> {
public:
    CommandMessage(ThreadCommands msg) : Message<ThreadCommands>(msg) {}
};




template <typename Tmessage>
class MessageThread {
private:
    std::queue<Message<Tmessage>> _message_queue;
    std::queue<CommandMessage> _command_queue;
    std::mutex _mutex;
    std::mutex _waiting;
    std::thread _thread;
    std::condition_variable _waiting_for_message;
    std::vector<std::function<void(Message<Tmessage>)>> _subscribers;


public:
    MessageThread() : _thread(&MessageThread<Tmessage>::run, this) 
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    MessageThread(const MessageThread<Tmessage>&) = delete;
    MessageThread<Tmessage>& operator =(const MessageThread<Tmessage>&) = delete;

    void SendMessage(Message<Tmessage> message)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _message_queue.push(message);
        }
        _waiting_for_message.notify_one();
    }

    void SendMessage(CommandMessage message)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _command_queue.push(message);
        }
        _waiting_for_message.notify_one();
    }



    void Subscribe(std::function<void(Message<Tmessage>)> sub)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _subscribers.push_back(sub);
        }
    }

    void Join()
    {
        if (_thread.joinable()) _thread.join();
    }

protected:
    virtual void PreProcess(Message<Tmessage> msg) {}
    virtual void PostProcess(Message<Tmessage> msg) {}

    void run()
    {
        while (true)
        {
            std::unique_lock<std::mutex> lk(_waiting);
            _waiting_for_message.wait(lk);
            {
                std::lock_guard<std::mutex> lock(_mutex);

                while (! _command_queue.empty())
                {
                    auto msg = _command_queue.front();
                    _command_queue.pop();
                    switch (msg.getMessage())
                    {
                        case ThreadCommands::SHUTDOWN:
                            goto exit;
                    }
                }

                while (! _message_queue.empty())
                {
                    auto msg = _message_queue.front();
                    _message_queue.pop();
                    PreProcess(msg);
                    for (auto sub : _subscribers)
                    {
                        sub(msg);
                    }
                    PostProcess(msg);
                }
            }
        }
        exit: ;
    }
};





#endif // !_MESSAGE_THREAD_HPP







