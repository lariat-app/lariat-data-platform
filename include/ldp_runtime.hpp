#ifndef LDP_RUNTIME_HPP
#define LDP_RUNTIME_HPP

#include <ldp/core/config.hpp>
#include <ldp/runtime/service.hpp>

struct LdpRuntime
{
    LdpConfigPtr readConfig(std::istream& input);
    LdpResourceConnectorPtr createConnector(LdpConfig* config);
};

#endif // LDP_RUNTIME_HPP