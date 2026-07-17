#include <ldp_runtime.hpp>

#include <argparse/argparse.hpp>

#include <fstream>

#include <csignal>

static std::ofstream logFile;
static LdpRuntime runtime;

static bool configure(int argc, const char ** argv);
static void onSignal(int signum);

int main(int argc, const char** argv)
{
    int rc(EXIT_FAILURE);
    if(configure(argc, argv))
    {
        signal(SIGINT, onSignal);
        signal(SIGTERM, onSignal);
        rc = runtime.dispatch();
    }
    return rc;
}

bool configure(int argc, const char ** argv)
{
    bool result(false);
    std::ostream *logSink(nullptr);
    LdpLogLevel logLevel;
    std::ifstream configFile;

    argparse::ArgumentParser program("ldpx");
    program.add_argument("-l", "--log")
        .help("Path to the log file")
        .default_value(std::string("stderr"));
    program.add_argument("-c", "--config")
        .help("Path to the configuration file")
        .default_value(std::string("config.json"));
    program.add_argument("-d", "--debug")
        .help("Enable debug mode")
        .default_value(false)
        .implicit_value(true);
    program.add_argument("-t", "--trace")
        .help("Enable trace mode")
        .default_value(false)
        .implicit_value(true);
    program.add_argument("-s", "--service")
        .help("Name of the service")
        .default_value(std::string("default_service"));
    try 
    {
        program.parse_args(argc, argv);

        const std::string logPath = program.get<std::string>("--log");
        if(logPath == "stdout")
        {
            logSink = &std::cout;
        }
        else if(logPath == "stderr")
        {
            logSink = &std::cerr;
        }
        else
        {
            logFile.open(logPath, std::ios::out | std::ios::app);
            if(!logFile.is_open())
            {
                throw std::runtime_error("Failed to open log file.");
            }
            logSink = &logFile;
        }

        const std::string configPath = program.get<std::string>("--config");
        configFile.open(configPath, std::ios::in);
        if(!configFile.is_open())
        {
            throw std::runtime_error("Failed to open config file.");
        }

        if(program.get<bool>("--trace"))
        {
            logLevel = LdpLogLevel::Trace;
        }
        else if (program.get<bool>("--debug"))
        {
            logLevel = LdpLogLevel::Debug;
        }
        else
        {
            logLevel = LdpLogLevel::Info;
        }

        const std::string serviceName = program.get<std::string>("--service");

        result = runtime.initialize(*logSink, logLevel, configFile, serviceName);
    }
    catch(const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        std::cerr << program;
        result = false;
    }
    return result;
}

void onSignal(int signum)
{
    runtime.shutdown(signum);
}