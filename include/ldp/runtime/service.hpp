#ifndef LDP_RUNTIME_SERVICE_HPP
#define LDP_RUNTIME_SERVICE_HPP

#include <ldp/resource/fifo.hpp>

class LdpConnector
{
public:
    using Ptr = std::unique_ptr<LdpConnector, std::function<void(LdpConnector*)>>;
    LdpFifo::Ptr acquireFifo(const std::string& name);
};

struct LdpService
{
    using Ptr = std::unique_ptr<LdpService, std::function<void(LdpService*)>>;
    virtual bool onInitialize(LdpConnector& connector) = 0;
    virtual bool onDispatch() = 0;
    virtual void onShutdown() = 0;
};

struct LdpServiceFactory
{
    using Ptr = std::unique_ptr<LdpServiceFactory, std::function<void(LdpServiceFactory*)>>;
    virtual LdpService *createService() const = 0;
    virtual void destroyService(LdpService* service) const = 0;
};

#endif