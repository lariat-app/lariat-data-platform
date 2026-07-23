#ifndef LDP_PLUGIN_HPP
#define LDP_PLUGIN_HPP

#include <ldp/common/system.hpp>
#include <ldp/resource/provider.hpp>
#include <ldp/templates/fifo.ipp>
#include <ldp/templates/blob.ipp>

#define LDP_EXPORT_PLUGIN(INTERFACE, TEMPLATE, OBJECT) \
    struct INTERFACE##FactoryImpl : public INTERFACE##Factory \
    { \
        INTERFACE##FactoryImpl() = default; \
        ~INTERFACE##FactoryImpl() = default; \
        INTERFACE *createObject(const LdpConfig &config, LdpLogger &logger) const override \
        { \
            INTERFACE *result(nullptr); \
            std::unique_ptr<TEMPLATE> object(new OBJECT()); \
            if (object) \
            { \
                if (object->configure(config, logger)) \
                { \
                    result = object.release(); \
                } \
                else \
                { \
                    logger.error("Failed to configure %s", #OBJECT); \
                } \
            } \
            else \
            { \
                logger.error("Failed to create %s", #OBJECT); \
            } \
            return result; \
        } \
        void destroyObject(INTERFACE *object) const override \
        { \
            delete object; \
        } \
    };\
    LDP_EXPORT_FACTORY(INTERFACE##Factory, INTERFACE##FactoryImpl)

#define LDP_EXPORT_FIFO(FIFO_TYPE) LDP_EXPORT_PLUGIN(LdpFifo, ldp::Fifo, FIFO_TYPE)
#define LDP_EXPORT_BLOB_STORAGE(BLOB_STORAGE_TYPE) LDP_EXPORT_PLUGIN(LdpBlobStorage, ldp::BlobStorage, BLOB_STORAGE_TYPE)

#endif