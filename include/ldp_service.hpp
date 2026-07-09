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
    LdpService::Ptr createService() override
    {
        return LdpService::Ptr(new ServiceType(), [](LdpService* service) { delete service; });
    }
};

#define LDP_SERVICE(ServiceType) \
    static LdpBasicServiceFactory<ServiceType> g_serviceFactory; \
    extern "C" LdpServiceFactory* createServiceFactory() \
    { \
        return &g_serviceFactory; \
    }

#endif // LDP_SERVICE_HPP