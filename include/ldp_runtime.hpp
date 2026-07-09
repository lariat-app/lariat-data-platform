#ifndef LDP_RUNTIME_HPP
#define LDP_RUNTIME_HPP

#include <ldp/core/config.hpp>
#include <ldp/core/logger.hpp>
#include <ldp/runtime/plugin.hpp>
#include <ldp/runtime/service.hpp>

struct LdpRuntime
{
    using Ptr = std::unique_ptr<LdpRuntime, std::function<void(LdpRuntime*)>>;
    virtual LdpLogger::Ptr openLogger(std::ostream& output, LdpLogLevel level) const = 0;
    virtual LdpConfig::Ptr readConfig(std::istream& input, LdpLogger* logger) const = 0;
    virtual LdpConnector::Ptr createConnector(const LdpConfig* config, LdpLogger* logger) const = 0;
};

LdpRuntime::Ptr LdpInitialize();

#endif // LDP_RUNTIME_HPP