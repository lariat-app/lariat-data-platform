#ifndef LDP_TEMPLATES_SERVICE_IPP
#define LDP_TEMPLATES_SERVICE_IPP

#include <ldp/resource/consumer.hpp>

namespace ldp
{
    struct Service : LdpService, LdpResourceConsumer
    {
        Service() = default;
    };
}

#endif