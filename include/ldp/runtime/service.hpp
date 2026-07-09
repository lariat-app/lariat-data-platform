#ifndef LDP_RUNTIME_SERVICE_HPP
#define LDP_RUNTIME_SERVICE_HPP

#include <ldp/resource/fifo.hpp>

struct LdpConnector
{
    using Ptr = std::unique_ptr<LdpConnector, std::function<void(LdpConnector*)>>;
    virtual LdpFifo::Ptr createFifo(const std::string& name) = 0; 
};

struct LdpService
{
    using Ptr = std::unique_ptr<LdpService, std::function<void(LdpService*)>>;
    virtual bool onInitialize(LdpConnector* connector) = 0;
    virtual bool onDispatch() = 0;
    virtual void onShutdown() = 0;
};

struct LdpServiceFactory
{
    using Ptr = std::unique_ptr<LdpServiceFactory, std::function<void(LdpServiceFactory*)>>;
    virtual LdpService::Ptr createService() = 0;
};

#endif