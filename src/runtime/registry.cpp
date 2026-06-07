#include <ldp_runtime.hpp>

#include <unordered_map>

#ifdef _WIN32
#define DLL_TYPE ".dll"
#else
#define DLL_TYPE ".so"
#endif

class plugin_map
{
public:
    plugin_map() = default;
    ~plugin_map() = default;
    ldp_plugin_api *get_plugin(const std::string &name)
    {
        ldp_plugin_api *result = find_plugin(name);
        if (!result)
        {
            result = load_plugin(name);
        }
        return result;
    }

private:
    ldp_plugin_api *find_plugin(const std::string &name)
    {
        ldp_plugin_api *result(nullptr);
        auto it = _plugins.find(name);
        if (it != _plugins.end())
        {
            result = it->second->plugin_api();
        }
        return result;
    }
    ldp_plugin_api *load_plugin(const std::string &name)
    {
        const std::string path = "libldp_" + name + DLL_TYPE;
        ldp_plugin_api *result(nullptr);
        ldp::module *dll = ldp::load_module(path, ldp::module_type::plugin);
        if (dll)
        {
            result = dll->plugin_api();
            _plugins.emplace(name, std::unique_ptr<ldp::module>(dll));
        }
        return result;
    }
    std::unordered_map<std::string, std::unique_ptr<ldp::module>> _plugins;
};

template <class ResourceType>
class resource_map
{
public:
    resource_map() = default;
    ~resource_map() = default;
    template <auto Constructor>
    ResourceType *get_resource(plugin_map &plugins, const std::string &name, const ldp::config_object *properties, ldp::logger *logger)
    {
        ResourceType *result = find_resource(name);
        if (!result)
        {
            const std::string plugin_name = name.substr(0, name.find('.'));
            const std::string resource_type = name.substr(name.find('.') + 1);
            ldp_plugin_api *plugin_api = plugins.get_plugin(plugin_name);
            if (plugin_api)
            {
                result = (plugin_api->*Constructor)(resource_type, properties, logger);
                if (result)
                {
                    _resources.emplace(name, std::unique_ptr<ResourceType>(result));
                }
            }
        }
        return result;
    }

private:
    ResourceType *find_resource(const std::string &name)
    {
        ResourceType *result(nullptr);
        auto it = _resources.find(name);
        if (it != _resources.end())
        {
            result = it->second.get();
        }
        return result;
    }
    std::unordered_map<std::string, std::unique_ptr<ResourceType>> _resources;
};

class mapped_registry : public ldp::registry
{
public:
    mapped_registry() = default;
    ~mapped_registry() = default;
    
    ldp::blob_storage *get_blob_storage(const std::string &name, const ldp::config_object *resources, ldp::logger *logger) override
    {
        return _blob_storages.get_resource<&ldp_plugin_api::create_blob_storage>(_plugins,name, resources, logger);
    }
    
    ldp::message_queue *get_message_queue(const std::string &name, const ldp::config_object *resources, ldp::logger *logger) override
    {
        return _message_queues.get_resource<&ldp_plugin_api::create_message_queue>(_plugins, name, resources, logger);
    }

private:
    plugin_map _plugins;
    resource_map<ldp::blob_storage> _blob_storages;
    resource_map<ldp::message_queue> _message_queues;
};