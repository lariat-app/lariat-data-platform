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

void ldpx::Logger::trace(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _logger->trace(fmt, args);
    va_end(args);
}

void ldpx::Logger::debug(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _logger->debug(fmt, args);
    va_end(args);
}

void ldpx::Logger::info(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _logger->info(fmt, args);
    va_end(args);
}

void ldpx::Logger::warning(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _logger->warn(fmt, args);
    va_end(args);
}

void ldpx::Logger::error(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    _logger->error(fmt, args);
    va_end(args);
}