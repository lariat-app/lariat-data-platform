#ifndef LDP_REGISTRY_HPP
#define LDP_REGISTRY_HPP

#include <string>


#ifdef _WIN32
#define LDP_EXPORT_API __declspec(dllexport)
#else
#define LDP_EXPORT_API
#endif

struct ldp_plugin_api;
struct ldp_module_api;

namespace ldp
{
    class config_object;
    class logger;

    class message_queue;
    class blob_storage;
    class resource_manager;

    class service;

    class registry
    {
    public:
        ldp_plugin_api *get_plugin(const std::string &name) const;
        ldp_module_api *get_module(const std::string &name) const;
    };
}

struct ldp_plugin_api
{
    ldp::blob_storage *(*create_blob_storage)(const std::string & factory_name, const ldp::config_object *properties, ldp::logger *logger);
    ldp::message_queue *(*create_message_queue)(const std::string & factory_name, const ldp::config_object *properties, ldp::logger *logger);
};

struct ldp_module_api
{
    ldp::service *(*create_service)(ldp::resource_manager *resource_manager, const ldp::config_object *resources, const ldp::config_object *service_properties, ldp::logger *logger);
};

#endif