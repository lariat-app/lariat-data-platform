#ifndef LDP_CORE_LOGGER_HPP
#define LDP_CORE_LOGGER_HPP

#include <functional>
#include <memory>

enum class LdpLogLevel
{
    Trace,
    Debug,
    Info,
    Warning,
    Error
};

struct LdpLogger
{
    using Ptr = std::unique_ptr<LdpLogger, std::function<void(LdpLogger*)>>;
    virtual void trace(const char* fmt, ...) = 0;
    virtual void debug(const char* fmt, ...) = 0;
    virtual void info(const char* fmt, ...) = 0;
    virtual void warning(const char* fmt, ...) = 0;
    virtual void error(const char* fmt, ...) = 0;
};

#endif // LDP_CORE_LOGGER_HPP