#ifndef LDP_MESSAGE_HPP
#define LDP_MESSAGE_HPP

#include <functional>
#include <memory>


struct LdpMessageSource
{

};

struct LdpMessageSink
{

};

struct LdpMessageBroker
{
    using SourcePtr = std::unique_ptr<LdpMessageSource, std::function<void(LdpMessageSource*)>>;
    using SinkPtr = std::unique_ptr<LdpMessageSink, std::function<void(LdpMessageSink*)>>;
    virtual bool createSource(SourcePtr& source) = 0;
    virtual bool createSink(SinkPtr& sink) = 0;
};

struct LdpEventBroker
{

};

struct LdpStreamBroker
{

};

using LdpMessageBrokerPtr = std::unique_ptr<LdpMessageBroker, std::function<void(LdpMessageBroker*)>>;
using LdpEventBrokerPtr = std::unique_ptr<LdpEventBroker, std::function<void(LdpEventBroker*)>>;
using LdpStreamBrokerPtr = std::unique_ptr<LdpStreamBroker, std::function<void(LdpStreamBroker*)>>;

#endif