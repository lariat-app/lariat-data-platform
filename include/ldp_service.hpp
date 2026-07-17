#ifndef LDP_SERVICE_HPP
#define LDP_SERVICE_HPP

#include <ldp/runtime/service.hpp>

template <typename ServiceType>
class LdpBasicServiceFactory : public LdpServiceFactory
{
public:
    static_assert(std::is_base_of<LdpService, ServiceType>::value, "ServiceType must derive from LdpService");
    LdpBasicServiceFactory() = default;
    ~LdpBasicServiceFactory() = default;
    LdpService* createService() const override
    {
        return new ServiceType();
    }

    void destroyService(LdpService* service) const override
    {
        delete service;
    }
};

#define LDP_SERVICE(ServiceType) \
    static LdpBasicServiceFactory<ServiceType> g_serviceFactory; \
    extern "C" LdpServiceFactory* createServiceFactory() \
    { \
        return &g_serviceFactory; \
    }

#endif // LDP_SERVICE_HPP