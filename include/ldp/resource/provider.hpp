#ifndef LDP_RESOURCE_PROVIDER_HPP
#define LDP_RESOURCE_PROVIDER_HPP

#include <ldp/framework/config.hpp>
#include <ldp/framework/logger.hpp>
#include <ldp/core/fifo.hpp>

struct LdpResourceProvider
{
    virtual bool configure(const LdpConfig &config, LdpLogger &logger) = 0;
};

struct LdpFifoFactory
{
    virtual LdpFifo *createObject(const LdpConfig &config, LdpLogger &logger) const = 0;
    virtual void destroyObject(LdpFifo *fifo) const = 0;
};


#endif