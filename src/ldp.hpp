#include <ldp_runtime.hpp>

#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/spdlog.h>

#include <unordered_map>

namespace ldp
{
    class Logger : public LdpLogger
    {
    public:
        Logger() = default;
        ~Logger() = default;
        void trace(const char* fmt, ...) override;
        void debug(const char* fmt, ...) override;
        void info(const char* fmt, ...) override;
        void warning(const char* fmt, ...) override;
        void error(const char* fmt, ...) override;
    private:
        std::shared_ptr<spdlog::logger> _logger;
    };

    class Config : public LdpConfig
    {

    };

    class Module
    {
        LdpPlugin* getPlugin() const;
        LdpService* getService() const;
    };

    class Plugin : public LdpPlugin
    {

    };

    class Connector : public LdpConnector
    {
    public:
        Connector(const LdpConfig* config, LdpLogger* logger);
        ~Connector() = default;
        LdpFifo::Ptr createFifo(const std::string& name) override;
    private:
        const LdpConfig* _config;
        LdpLogger* _logger;
        std::unordered_map<std::string, std::unique_ptr<Module>> _plugins;
    };

    class Runtime : public LdpRuntime
    {

    };
}