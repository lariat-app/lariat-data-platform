#ifndef LDP_FRAMEWORK_LOGGER_HPP
#define LDP_FRAMEWORK_LOGGER_HPP

struct LdpLogger
{
    virtual void trace(const char* fmt, ...) = 0;
    virtual void debug(const char* fmt, ...) = 0;
    virtual void info(const char* fmt, ...) = 0;
    virtual void warning(const char* fmt, ...) = 0;
    virtual void error(const char* fmt, ...) = 0;
};

#endif