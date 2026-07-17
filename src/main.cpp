#include <config.hpp>
#include <logger.hpp>

class Runtime
{
public:
    Runtime() = default;
    ~Runtime() = default;
    bool initialize(int argc, const char ** argv);
    int dispatch();
    void shutdown();
private:
    
};

static Runtime runtime;

static void onSignal(int signum);

int main(int argc, const char ** argv)
{
    return 0;
}