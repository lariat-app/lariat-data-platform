#include <ldp_plugin.hpp>

template <class ResourceType>
static ResourceType *create_resource(const ldp::resource_factory_map<ResourceType> &factories, const std::string &name, const ldp::config_object *properties, ldp::logger *logger)
{
    ResourceType *result(nullptr);
    
    auto factory = factories.get_factory(name);
    if (factory)
    {
        result = factory(properties, logger);
        if (!result)
        {
            logger->error("Failed to create resource from factory '%s'", name.c_str());
        }
    }
    else
    {
        logger->error("Factory '%s' not found", name.c_str());
    }
    return result;
}

ldp::resource_factory_plugin::resource_factory_plugin(const std::string &name) : _name(name)
{
}

ldp::blob_storage *ldp::resource_factory_plugin::create_blob_storage(const std::string &factory_name, const config_object *properties, logger *logger) const
{
    return create_resource(_blob_storage_factories, factory_name, properties, logger);
}

ldp::message_queue *ldp::resource_factory_plugin::create_message_queue(const std::string &factory_name, const config_object *properties, logger *logger) const
{
    return create_resource(_message_queue_factories, factory_name, properties, logger);
}

