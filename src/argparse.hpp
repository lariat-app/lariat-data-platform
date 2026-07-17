#ifndef LDP_ARGPARSE_HPP
#define LDP_ARGPARSE_HPP

#include <ldp/core/logger.hpp>
#include <argparse/argparse.hpp>

namespace ldp
{
    class Arguments
    {
    public:
        Arguments() = default;
        ~Arguments() = default;
        std::string getConfigPath() const;
        std::string getLogPath() const;
        LdpLogLevel getLogLevel() const;
        std::string getServiceName() const;
    };

    class ArgumentParser
    {
    public:
        ArgumentParser(int argc, const char **argv);
        ~ArgumentParser() = default;
        bool parse(Arguments &args);
    private:
        int _argCount;
        const char **_argValues;
        argparse::ArgumentParser parser;
    };
}

#endif // LDP_ARGPARSE_HPP