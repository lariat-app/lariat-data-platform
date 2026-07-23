#ifndef LDP_TEMPLATES_BLOB_IPP
#define LDP_TEMPLATES_BLOB_IPP

#include <ldp/core/blob.hpp>
#include <ldp/framework/config.hpp>
#include <ldp/framework/logger.hpp>
#include <ldp/resource/provider.hpp>

namespace ldp
{
    struct BlobStorage : LdpBlobStorage, LdpResourceProvider
    {
        BlobStorage() = default;
        LdpBlobSource::Ptr acquireSource() override
        {
            return std::unique_ptr<LdpBlobSource, std::function<void(LdpBlobSource *)>>(createSource(), [](LdpBlobSource *source)
                                                                                        { delete source; });
        }
        LdpBlobSink::Ptr acquireSink() override
        {
            return std::unique_ptr<LdpBlobSink, std::function<void(LdpBlobSink *)>>(createSink(), [](LdpBlobSink *sink)
                                                                                    { delete sink; });
        }
        LdpBlobClient::Ptr acquireClient() override
        {
            return std::unique_ptr<LdpBlobClient, std::function<void(LdpBlobClient *)>>(createClient(), [](LdpBlobClient *client)
                                                                                        { delete client; });
        }
        virtual LdpBlobSource *createSource() = 0;
        virtual LdpBlobSink *createSink() = 0;
        virtual LdpBlobClient *createClient() = 0;
    };
}

#endif