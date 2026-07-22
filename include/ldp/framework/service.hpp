#ifndef LDP_FRAMEWORK_SERVICE_HPP
#define LDP_FRAMEWORK_SERVICE_HPP

#include <ldp/common/object.hpp>

struct LdpService : LdpObject<LdpService>
{
    virtual int launch() = 0;
    virtual void shutdown() = 0;
};

#endif