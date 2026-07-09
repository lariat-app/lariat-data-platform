#include <ldp_runtime.hpp>

#include <csignal>

class Process
{
private:
    LdpRuntime::Ptr runtime;
    LdpLogger::Ptr logger;
    LdpConfig::Ptr config;
    LdpConnector::Ptr connector;
    LdpService::Ptr service;
public:
    Process() = default;
    ~Process() = default;
    bool argparse(int argc, const char* argv[]);
    int dispatch();
    void terminate(int signum);
};

static Process process;
static void sighandle(int signum)
{
    process.terminate(signum);
}

int main(int argc, const char* argv[])
{
    int rc = EXIT_FAILURE;
    if (process.argparse(argc, argv))
    {
        signal(SIGINT, sighandle);
        signal(SIGTERM, sighandle);
        rc = process.dispatch();
    }
    return rc;
}