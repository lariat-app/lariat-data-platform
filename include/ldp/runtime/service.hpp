#ifndef LDP_RUNTIME_SERVICE_HPP
#define LDP_RUNTIME_SERVICE_HPP

#include <ldp/resource/blob.hpp>
#include <ldp/resource/message.hpp>

struct LdpServiceRuntime
{
    virtual LdpBlobStoragePtr createBlobStorage(const std::string& name) = 0;
    virtual LdpMessageBrokerPtr createMessageBroker(const std::string& name) = 0;
    virtual LdpEventBrokerPtr createEventBroker(const std::string& name) = 0;
    virtual LdpStreamBrokerPtr createStreamBroker(const std::string& name) = 0;
};

struct LdpService
{
    virtual bool initialize(LdpServiceRuntime* runtime) = 0;
    virtual bool shutdown() = 0;
};

#endif