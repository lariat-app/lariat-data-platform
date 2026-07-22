#ifndef LDP_SERVICE_HPP
#define LDP_SERVICE_HPP

#include <ldp/common/system.hpp>
#include <ldp/resource/consumer.hpp>
#include <ldp/templates/service.ipp>

#define LDP_EXPORT_SERVICE(SERVICE) \
    struct SERVICE##FactoryImpl : public LdpServiceFactory \
    { \
        SERVICE##FactoryImpl() = default; \
        ~SERVICE##FactoryImpl() = default; \
        LdpService *createObject(LdpConnector &connector, LdpLogger &logger) const override \
        { \
            LdpService *result(nullptr); \
            std::unique_ptr<ldp::Service> object(new SERVICE()); \
            if (object) \
            { \
                if (object->connect(connector, logger)) \
                { \
                    result = object.release(); \
                } \
                else \
                { \
                    logger.error("Failed to connect %s", #SERVICE); \
                } \
            } \
            else \
            { \
                logger.error("Failed to create %s", #SERVICE); \
            } \
            return result; \
        } \
        void destroyObject(LdpService *object) const override \
        { \
            delete object; \
        } \
    };\
    extern "C" LDP_EXPORT_DLL LdpServiceFactory *getLdpServiceFactory() \
    { \
        static SERVICE##FactoryImpl factory; \
        return &factory; \
    }

#endif