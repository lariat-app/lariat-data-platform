#include <logger.hpp>

#include <cstdarg>
    
bool ldpx::Logger::open(const std::string &logPath, bool debug, bool trace)
{
    bool result(false);
    if (logPath == "stdout")
    {
        _sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(std::cout);
    }
    else if (logPath == "stderr")
    {
        _sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(std::cerr);
    }
    else
    {
        _file.open(logPath, std::ios::out | std::ios::app);
        if (_file.is_open())
        {
            _sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(_file);
        }
    }

    if (_sink)
    {
        _logger = std::make_shared<spdlog::logger>("ldpx", _sink);
        if (_logger)
        {
            if (debug)
            {
                _logger->set_level(spdlog::level::debug);
            }
            else if (trace)
            {
                _logger->set_level(spdlog::level::trace);
            }
            else
            {
                _logger->set_level(spdlog::level::info);
            }
            result = true;
        }
    }

    return result;
}

void ldpx::Logger::log(LdpLogLevel level, const char *fmt, ...)
{
    static const spdlog::level::level_enum levelMap[] = {
        spdlog::level::trace,    // LDP_LOG_TRACE
        spdlog::level::debug,    // LDP_LOG_DEBUG
        spdlog::level::info,     // LDP_LOG_INFO
        spdlog::level::warn,     // LDP_LOG_WARNING
        spdlog::level::err       // LDP_LOG_ERROR
    };
    std::unique_ptr<char[]> dynamicBuffer;
    char buffer[256];
    std::string_view message;
    size_t size;
    va_list args;
    va_start(args, fmt);
    size = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    if (size >= sizeof(buffer))
    {
        dynamicBuffer = std::make_unique<char[]>(size + 1);
        va_start(args, fmt);
        vsnprintf(dynamicBuffer.get(), size + 1, fmt, args);
        va_end(args);
        message = std::string_view(dynamicBuffer.get(), size);
    }
    else
    {
        message = std::string_view(buffer, size);
    }
    _logger->log(levelMap[level], message); 
}
