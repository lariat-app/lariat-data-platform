#include <ldp_service.hpp>

ldp::basic_service::basic_service(const std::string &service_name) : _service_name(service_name)
{
}

bool ldp::basic_service::initialize(ldp::registry *registry, const ldp::config_object *resources, const ldp::config_object *service_properties, ldp::logger *logger)
{
    bool result;

    result = setup(registry, resources, logger);
    if(result)
    {
        result = configure(service_properties, logger);
        if (!result)
        {
            logger->error("%s: configuration failed", _service_name.c_str());
        }
    }
    else
    {
        logger->error("%s: setup failed", _service_name.c_str());
    }
    return result;
}

bool ldp::basic_service::launch(ldp::logger *logger)
{
    bool ok(true);
    _active = start(logger);
    if (!_active)
    {
        logger->error("%s: failed to start", _service_name.c_str());
    }

    while(_active && ok)
    {
        ok = run(logger);
        if (!ok)
        {
            logger->error("%s: failed during execution", _service_name.c_str());
        }
    }
    return ok;
}

void ldp::basic_service::shutdown(ldp::logger *logger)
{
    _active = false;
    stop(logger);
}