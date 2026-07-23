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
        void log(LdpLogLevel level, const char* fmt, ...) override;
    private:
        std::ofstream _file;
        std::shared_ptr<spdlog::sinks::ostream_sink_mt> _sink;
        std::shared_ptr<spdlog::logger> _logger;
        spdlog::level::level_enum _level;
    };
}

#endif // LDP_LOGGER_HPP