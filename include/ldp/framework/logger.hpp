#ifndef LDP_FRAMEWORK_LOGGER_HPP
#define LDP_FRAMEWORK_LOGGER_HPP

#include <cstdarg>

enum LdpLogLevel
{
    LDP_LOG_TRACE = 0,
    LDP_LOG_DEBUG,
    LDP_LOG_INFO,
    LDP_LOG_WARNING,
    LDP_LOG_ERROR
};

struct LdpLogger
{
    template <typename... Args>
    void trace(const char* fmt, Args... args)
    {
        log(LDP_LOG_TRACE, fmt, args...);
    }
    
    template <typename... Args>
    void debug(const char* fmt, Args... args)
    {
        log(LDP_LOG_DEBUG, fmt, args...);
    }
    
    template <typename... Args>
    void info(const char* fmt, Args... args)
    {
        log(LDP_LOG_INFO, fmt, args...);
    }
    
    template <typename... Args>
    void warning(const char* fmt, Args... args)
    {
        log(LDP_LOG_WARNING, fmt, args...);
    }
    
    template <typename... Args>
    void error(const char* fmt, Args... args)
    {
        log(LDP_LOG_ERROR, fmt, args...);
    }

    virtual void log(LdpLogLevel level, const char* fmt, ...) = 0;
};

#endif