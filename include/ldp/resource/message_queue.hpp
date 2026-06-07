#ifndef LDP_MESSAGE_QUEUE_HPP
#define LDP_MESSAGE_QUEUE_HPP

namespace ldp
{
    class message_queue
    {
    public:
        virtual void send(const char *message) = 0;
    };
}

#endif