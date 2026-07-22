#ifndef LDP_LOGGER_HPP
#define LDP_LOGGER_HPP

#include <ldp/framework/logger.hpp>

#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/spdlog.h>

#include <iostream>
#include <fstream>

namespace ldpx
{
    class Logger : public LdpLogger
    {
    public:
        Logger() = default;
        ~Logger() = default;
        bool open(const std::string& logPath, bool debug, bool trace);
        void trace(const char* fmt, ...) override;
        void debug(const char* fmt, ...) override;
        void info(const char* fmt, ...) override;
        void warning(const char* fmt, ...) override;
        void error(const char* fmt, ...) override;
    private:
        std::ofstream _file;
        std::shared_ptr<spdlog::sinks::ostream_sink_mt> _sink;
        std::shared_ptr<spdlog::logger> _logger;
    };
}

#endif // LDP_LOGGER_HPP