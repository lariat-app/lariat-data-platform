#ifndef LDP_PLUGIN_HPP
#define LDP_PLUGIN_HPP

#include <ldp/config.hpp>
#include <ldp/logger.hpp>
#include <ldp/registry.hpp>
#include <ldp/resource.hpp>

#include <functional>
#include <unordered_map>

namespace ldp
{
    template <class ResourceType>
    using resource_factory = std::function<ResourceType *(const config_object *properties, logger *logger)>;

    template <class ResourceType>
    class resource_factory_map
    {
    public:
        resource_factory_map() = default;
        ~resource_factory_map() = default;

        void add_factory(const std::string &name, const resource_factory<ResourceType> &factory)
        {
            _factories.emplace(name, factory);
        }

        resource_factory<ResourceType> get_factory(const std::string &name) const
        {
            resource_factory<ResourceType> result(nullptr);
            auto it = _factories.find(name);
            if (it != _factories.end())
            {
                result = it->second;
            }
            return result;
        }

    private:
        std::unordered_map<std::string, resource_factory<ResourceType>> _factories;
    };

    using blob_storage_factory_map = resource_factory_map<blob_storage>;
    using message_queue_factory_map = resource_factory_map<message_queue>;

    class resource_factory_plugin
    {
    public:
        resource_factory_plugin(const std::string &name);
        ~resource_factory_plugin() = default;
        ldp::blob_storage *create_blob_storage(const std::string &factory_name, const config_object *properties, logger *logger) const;
        ldp::message_queue *create_message_queue(const std::string &factory_name, const config_object *properties, logger *logger) const;

    protected:
        std::string _name;
        blob_storage_factory_map _blob_storage_factories;
        message_queue_factory_map _message_queue_factories;
    };
};

#define LDP_PLUGIN_CREATE_RESOURCE(plugin_name, resource_type, factory_name, properties, logger)                                                  \
    static ldp::resource_type *create_##resource_type(const std::string &factory_name, const ldp::config_object *properties, ldp::logger *logger) \
    {                                                                                                                                             \
        ldp::resource_type *result(nullptr);                                                                                                      \
        result = _instance.create_##resource_type(factory_name, properties, logger);                                                              \
        if (!result)                                                                                                                              \
        {                                                                                                                                         \
            logger->error("%s: failed to create resource from factory %s", #plugin_name, factory_name.c_str());                                   \
        }                                                                                                                                         \
        return result;                                                                                                                            \
    }

#define LDP_PLUGIN_BEGIN(plugin_name)                                                                \
    class plugin_name##_plugin : public ldp::resource_factory_plugin                                 \
    {                                                                                                \
    public:                                                                                          \
        plugin_name##_plugin();                                                                      \
    };                                                                                               \
    static plugin_name##_plugin _instance;                                                           \
    LDP_PLUGIN_CREATE_RESOURCE(plugin_name, blob_storage, create_blob_storage, properties, logger)   \
    LDP_PLUGIN_CREATE_RESOURCE(plugin_name, message_queue, create_message_queue, properties, logger) \
    extern "C" LDP_EXPORT_API void ldp_load_plugin(ldp_plugin_api *api)                           \
    {                                                                                                \
        api->create_blob_storage = &create_blob_storage;                                             \
        api->create_message_queue = &create_message_queue;                                           \
    }                                                                                                \
    plugin_name##_plugin::plugin_name##_plugin() : resource_factory_plugin(#plugin_name)             \
    {

#define LDP_PLUGIN_END }

#endif