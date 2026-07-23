#include <connector.hpp>

#ifdef _WIN32
#include <Windows.h>
std::string ldpx::Module::Library::Extension()
{
    return ".dll";
}
#define DLOPEN LoadLibraryA
#define DLCLOSE FreeLibrary
#define DLSYM GetProcAddress
using LibraryHandle = HINSTANCE__;
#else
#include <dlfcn.h>
std::string ldpx::Module::Library::Extension()
{
    return ".so";
}
#define DLOPEN(path) dlopen(path, RTLD_LAZY)
#define DLCLOSE dlclose
#define DLSYM dlsym
using LibraryHandle = void;
#endif

class DynamicLibrary : public ldpx::Module::Library
{
public:
    DynamicLibrary() = default;
    bool load(const std::string &path) override
    {
        _handle.reset(DLOPEN(path.c_str()));
        return _handle != nullptr;
    }
    void *getSymbol(const std::string &name) override
    {
        if (_handle)
        {
            return DLSYM(_handle.get(), name.c_str());
        }
        return nullptr;
    }
private:
    std::unique_ptr<LibraryHandle, std::function<void(LibraryHandle*)>> _handle{nullptr, [](LibraryHandle* handle) { if (handle) DLCLOSE(handle); }};
};

ldpx::Module::Library *ldpx::Module::Library::Create()
{
    return new DynamicLibrary();
}

bool ldpx::Connector::setup(const LdpConfig &config, LdpLogger &logger)
{
    bool result(false);
    LdpConfig::Object configObject;
    _logger = &logger;
    if (config.getObject(configObject))
    {
        LdpConfig::ObjectReader reader(configObject);
        result = reader.getObject("resources", _resourceConfigs);
        if (!result)
        {
            _logger->error("Missing 'resources' object in config");
        }
    }
    else
    {
        _logger->error("Top-level config is not an object");
    }
    return result;
}

LdpFifo::Ptr ldpx::Connector::acquireFifo(const std::string &name)
{
    return acquireResource<LdpFifoFactory, LdpFifo>(name);
}


ldpx::Module *ldpx::Connector::findModule(const std::string &providerName)
{
    ldpx::Module *result(nullptr);
    auto it = _pluginModules.find(providerName);
    if (it != _pluginModules.end())
    {
        result = &(it->second);
    }
    return result;
}
