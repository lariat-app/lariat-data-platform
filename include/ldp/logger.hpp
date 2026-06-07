#ifndef LDP_LOGGER_HPP
#define LDP_LOGGER_HPP

#include <functional>
#include <string>
#include <variant>

namespace ldp
{
    class logger
    {
    public:
        virtual ~logger() = default;
        virtual void error(const char *fmt, ...) = 0;
        virtual void warn(const char *fmt, ...) = 0;
        virtual void info(const char *fmt, ...) = 0;
        virtual void debug(const char *fmt, ...) = 0;
        virtual void trace(const char *fmt, ...) = 0;
    };
}

#endif