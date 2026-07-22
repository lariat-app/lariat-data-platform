#ifndef LDP_RESOURCE_CONSUMER_HPP
#define LDP_RESOURCE_CONSUMER_HPP

#include <ldp/framework/connector.hpp>
#include <ldp/framework/logger.hpp>
#include <ldp/framework/service.hpp>

struct LdpResourceConsumer
{
    virtual bool connect(LdpConnector &connector, LdpLogger &logger) = 0;
};

struct LdpServiceFactory
{
    virtual LdpService *createObject(LdpConnector &connector, LdpLogger &logger) const = 0;
    virtual void destroyObject(LdpService *service) const = 0;
};

#endif