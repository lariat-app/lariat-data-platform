#include <logger.hpp>

#include <cstdarg>

bool ldpx::Logger::open(const std::string &logPath, bool enableDebug, bool enableTrace)
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
            if (enableTrace)
            {
                _level = spdlog::level::trace;
            }
            else if (enableDebug)
            {
                _level = spdlog::level::debug;
            }
            else
            {
                _level = spdlog::level::info;
            }
            _logger->set_level(_level);
            debug("debug enabled");
            trace("trace enabled");
            result = true;
        }
    }

    return result;
}

void ldpx::Logger::log(LdpLogLevel level, const char *fmt, ...)
{
    static const spdlog::level::level_enum levelMap[] = {
        spdlog::level::trace, // LDP_LOG_TRACE
        spdlog::level::debug, // LDP_LOG_DEBUG
        spdlog::level::info,  // LDP_LOG_INFO
        spdlog::level::warn,  // LDP_LOG_WARNING
        spdlog::level::err    // LDP_LOG_ERROR
    };
    const spdlog::level::level_enum logLevel = levelMap[level];
    if (logLevel >= _level)
    {
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
        _logger->log(logLevel, message);
    }
}
