#ifndef LDP_RUNTIME_SERVICE_HPP
#define LDP_RUNTIME_SERVICE_HPP

#include <ldp/resource/fifo.hpp>

struct LdpResourceConnector
{
    virtual LdpFifoPtr createFifo(const std::string& name) = 0; 
};

struct LdpService
{
    virtual bool initialize(LdpResourceConnector* connector) = 0;
    virtual bool shutdown() = 0;
};

using LdpResourceConnectorPtr = std::unique_ptr<LdpResourceConnector, std::function<void(LdpResourceConnector*)>>;
using LdpServicePtr = std::unique_ptr<LdpService, std::function<void(LdpService*)>>;

#endif