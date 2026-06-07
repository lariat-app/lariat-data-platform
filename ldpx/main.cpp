#include <ldpx.hpp>

#include <signal.h>


void on_signal(int signal)
{
    ldpx::process::get()->quit();
}

int main(int argc, const char ** argv)
{
    int rc(1);
    ldpx::process process;
    if(process.argparse(argc, argv))
    {
        signal(SIGINT, on_signal);
        signal(SIGTERM, on_signal);
        rc = process.run();
    }
    return rc;
}