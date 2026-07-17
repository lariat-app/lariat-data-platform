#ifndef RUNTIME_HPP
#define RUNTIME_HPP

#include <ldp_runtime.hpp>

namespace ldp
{
    bool initializeLogger(std::unique_ptr<LdpLogger> &logger, std::ostream &logSink, LdpLogLevel level);
    bool initializeConfig(std::unique_ptr<LdpConfig> &config, std::istream &configSource);
    bool initializeConnector(std::unique_ptr<LdpConnector> &connector, const LdpConfig &config, LdpLogger &logger);
    bool initializeService(std::unique_ptr<LdpService> &service, const LdpConfig &config, LdpLogger &logger, const std::string& serviceName);
}

#endif // RUNTIME_HPP
