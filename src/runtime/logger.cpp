#include <ldp_runtime.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/ostream_sink.h>

#define LOG_METHOD(method) \
    void method(const char *fmt, ...) override \
    { \
        va_list args; \
        va_start(args, fmt); \
        _logger->method(fmt, args); \
        va_end(args); \
    }

class basic_logger : public ldp::logger
{
public:
    basic_logger() = default;
    ~basic_logger() = default;

    bool create(std::ostream &sink, ldp::log_level level)
    {
        const static spdlog::level::level_enum spd_levels[] = {spdlog::level::err, spdlog::level::warn, spdlog::level::info, spdlog::level::debug, spdlog::level::trace};
        bool result(false);
        spdlog::sink_ptr spd_sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(sink);
        _logger = std::make_unique<spdlog::logger>("ldp_runtime", spd_sink);
        if (_logger)
        {
            result = true;
            _logger->set_level(spd_levels[static_cast<size_t>(level)]);
        }
        return result;
    }

    LOG_METHOD(error)
    LOG_METHOD(warn)
    LOG_METHOD(info)
    LOG_METHOD(debug)
    LOG_METHOD(trace)

private:
    std::unique_ptr<spdlog::logger> _logger;
};

ldp::logger *create_logger(std::ostream &sink, ldp::log_level level)
{
    ldp::logger *result(nullptr);
    std::unique_ptr<basic_logger> logger = std::make_unique<basic_logger>();
    if (logger->create(sink, level))
    {
        result = logger.release();
    }
    return result;
}