#include <ldp_runtime.hpp>

#ifdef _WIN32
    #include <windows.h>
    #define DLHANDLE HMODULE
    #define DLOPEN(path) LoadLibraryA(path)
    #define DLCLOSE(handle) FreeLibrary(handle)
    #define DLSYM(handle, name) GetProcAddress(handle, name)
#else
    #include <dlfcn.h>
    #define DLHANDLE void*
    #define DLOPEN(path) dlopen(path, RTLD_LAZY)
    #define DLCLOSE(handle) dlclose(handle)
    #define DLSYM(handle, name) dlsym(handle, name)
#endif

class dll_module : public ldp::module
{
public:
    dll_module() = default;
    ~dll_module() override
    {
        unload();
    }
    
    bool open(const std::string &path)
    {
        bool result(false);
        unload();
        _handle = DLOPEN(path.c_str());
        if (_handle)
        {
            result = true;
        }
        return result;
    }

    bool load(ldp::module_type type) override
    {
        bool result(false);
        if (type == ldp::module_type::plugin)
        {
            auto load_plugin_func = reinterpret_cast<ldp_plugin_api *(*)(void)>(get_symbol("ldp_load_plugin"));
            if (load_plugin_func)
            {
                _plugin_api = load_plugin_func();
                result = (_plugin_api != nullptr);
            }
        }
        else if (type == ldp::module_type::service)
        {
            auto load_service_func = reinterpret_cast<ldp_service_api *(*)(void)>(get_symbol("ldp_load_service"));
            if (load_service_func)
            {
                _service_api = load_service_func();
                result = (_service_api != nullptr);
            }
        }
        else
        {
            result = true;
        }
        return result;
    }

    void *get_symbol(const std::string &name) const override
    {
        void *result = nullptr;
        if (_handle)
        {
            result = DLSYM(_handle, name.c_str());
        }
        return result;
    }

    ldp_plugin_api *plugin_api() const override
    {
        return _plugin_api;
    }

    ldp_service_api *service_api() override
    {
        return _service_api;
    }

private:
    void unload()
    {
        if (_handle)
        {
            DLCLOSE(_handle);
            _handle = nullptr;
            _plugin_api = nullptr;
            _service_api = nullptr;
        }
    }
    DLHANDLE _handle = nullptr;
    ldp_plugin_api *_plugin_api = nullptr;
    ldp_service_api *_service_api = nullptr;
};

ldp::module *ldp::load_module(const std::string &name, ldp::module_type type)
{
    ldp::module *result = nullptr;
    std::unique_ptr<dll_module> module = std::make_unique<dll_module>();
    if (module->open(name) && module->load(type))
    {
        result = module.release();
    }
    return result;
}