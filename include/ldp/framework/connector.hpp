#ifndef LDP_FRAMEWORK_CONNECTOR_HPP
#define LDP_FRAMEWORK_CONNECTOR_HPP

#include <ldp/core/fifo.hpp>

struct LdpConnector
{
    virtual LdpFifo::Ptr acquireFifo(const std::string &name) = 0;
};

#endif