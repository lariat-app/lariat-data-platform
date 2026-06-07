#ifndef LDPX_HPP
#define LDPX_HPP

#include <ldp_runtime.hpp>

#include <argparse/argparse.hpp>

#include <fstream>

namespace ldpx
{
    class argument_parser
    {
    public:
        argument_parser() = default;
        ~argument_parser() = default;
        bool parse(int argc, const char **argv);
        std::string& service_name() const;
        std::string& config_path() const;
        std::string& log_path() const;
        ldp::log_level log_level() const;
    private:
        std::string _service_name;
        std::string _config_path;
        std::string _log_path;
        ldp::log_level _log_level = ldp::log_level::info;
    };

    class process
    {
    public:
        process();
        ~process() = default;

        bool init(int argc, const char **argv);
        int run();
        void quit();

        static process *get()
        {
            return _process;
        }

    private:
        std::ofstream _log_file;
        std::ostream *_log_sink;
        ldp::runtime _runtime;
        static process *_process;
    };
}

#endif