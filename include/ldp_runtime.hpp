#ifndef LDP_RUNTIME_HPP
#define LDP_RUNTIME_HPP

#include <ldp/core/config.hpp>
#include <ldp/core/logger.hpp>
#include <ldp/runtime/plugin.hpp>
#include <ldp/runtime/service.hpp>

class LdpRuntime
{
public:
    LdpRuntime() = default;
    ~LdpRuntime() = default;
    bool initialize(
        std::ostream &logSink, 
        LdpLogLevel level, 
        std::istream &configSource,
        const std::string& serviceName);
    int dispatch();
    void shutdown(int signum);
private:
    std::unique_ptr<LdpLogger> _logger;
    std::unique_ptr<LdpConfig> _config;
    std::unique_ptr<LdpConnector> _connector;
    std::unique_ptr<LdpServiceFactory> _serviceFactory;
};

#endif // LDP_RUNTIME_HPP