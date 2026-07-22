#ifndef LDP_TEMPLATES_FIFO_IPP
#define LDP_TEMPLATES_FIFO_IPP

#include <ldp/core/fifo.hpp>
#include <ldp/framework/config.hpp>
#include <ldp/framework/logger.hpp>
#include <ldp/resource/provider.hpp>

namespace ldp
{
    struct Fifo : LdpFifo, LdpResourceProvider
    {
        Fifo() = default;
        LdpFifoSource::Ptr acquireSource() override
        {
            return std::unique_ptr<LdpFifoSource, std::function<void(LdpFifoSource*)>>(createSource(), [](LdpFifoSource* source)
            {
                delete source;
            });
        }
        LdpFifoSink::Ptr acquireSink() override
        {
            return std::unique_ptr<LdpFifoSink, std::function<void(LdpFifoSink*)>>(createSink(), [](LdpFifoSink* sink)
            {
                delete sink;
            });
        }
        virtual LdpFifoSource* createSource() = 0;
        virtual LdpFifoSink* createSink() = 0;
    };
}

#define LDP_FIFO_FACTORY(FIFO_IMPL) LDP_RESOURCE_PROVIDER_FACTORY(LdpFifo, ldp::Fifo, FIFO_IMPL)

#endif