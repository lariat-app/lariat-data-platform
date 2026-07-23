#include <config.hpp>
#include <connector.hpp>
#include <logger.hpp>

#include <argparse/argparse.hpp>
#include <csignal>

class Runtime
{
public:
    Runtime() = default;
    ~Runtime() = default;
    LdpService::Ptr configure(int argc, const char **argv)
    {
        LdpService::Ptr result(nullptr);
        argparse::ArgumentParser program("ldpx");
        program.add_argument("-l", "--log")
            .help("Path to the log file")
            .default_value(std::string("stderr"));
        program.add_argument("-c", "--config")
            .help("Path to the configuration file");
        program.add_argument("-d", "--debug")
            .help("Enable debug mode")
            .default_value(false)
            .implicit_value(true);
        program.add_argument("-t", "--trace")
            .help("Enable trace mode")
            .default_value(false)
            .implicit_value(true);
        program.add_argument("-s", "--service")
            .help("Name of the service");
        try
        {
            program.parse_args(argc, argv);
            if (_logger.open(program.get<std::string>("--log"), program.get<bool>("--debug"), program.get<bool>("--trace")) && _config.parse(program.get<std::string>("--config"), _logger) && _connector.setup(_config, _logger))
            {
                const std::string serviceName = program.get<std::string>("--service");
                const std::string serviceLibrary("lib" + serviceName + ldpx::Module::Library::Extension());
                if (_serviceModule.load<LdpServiceFactory>(serviceLibrary))
                {
                    LdpServiceFactory *serviceFactory = _serviceModule.getFactory<LdpServiceFactory>();
                    if (serviceFactory)
                    {
                        result = std::unique_ptr<LdpService, std::function<void(LdpService*)>>(serviceFactory->createObject(_connector, _logger), [serviceFactory](LdpService* service)
                        {
                            serviceFactory->destroyObject(service);
                        });
                    }
                    else
                    {
                        _logger.error("Failed to get service factory from %s", serviceLibrary.c_str());
                    }
                }
                else
                {
                    _logger.error("Failed to load service module %s", serviceLibrary.c_str());
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
        return result;
    }

private:
    ldpx::Logger _logger;
    ldpx::Config _config;
    ldpx::Connector _connector;
    ldpx::Module _serviceModule;
};

static Runtime runtime;
static LdpService::Ptr service;

static void onSignal(int signum)
{
    service->shutdown();
}

int main(int argc, const char **argv)
{
    int rc(EXIT_FAILURE);
    service = runtime.configure(argc, argv);
    if (service)
    {
        signal(SIGINT, onSignal);
        signal(SIGTERM, onSignal);
        rc = service->launch();
    }
    return rc;
}