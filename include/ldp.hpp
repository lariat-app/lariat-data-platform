#ifndef LDP_HPP
#define LDP_HPP

#include <ldp/resource/blob.hpp>
#include <ldp/resource/message.hpp>

struct LdpRuntime
{
    using BlobStoragePtr = std::unique_ptr<LdpBlobStorage, std::function<void(LdpBlobStorage*)>>;
    using MessageBrokerPtr = std::unique_ptr<LdpMessageBroker, std::function<void(LdpMessageBroker*)>>;
    using EventBrokerPtr = std::unique_ptr<LdpEventBroker, std::function<void(LdpEventBroker*)>>;
    using StreamBrokerPtr = std::unique_ptr<LdpStreamBroker, std::function<void(LdpStreamBroker*)>>;
    BlobStoragePtr createBlobStorage(const char* provider, void *config);
    MessageBrokerPtr createMessageBroker(const char* provider, void *config);
    EventBrokerPtr createEventBroker(const char* provider, void *config);
    StreamBrokerPtr createStreamBroker(const char* provider, void *config);
};

#endif