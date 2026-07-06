#ifndef LDP_PROVIDER_FIFO_HPP
#define LDP_PROVIDER_FIFO_HPP

#include <ldp/core/config.hpp>
#include <ldp/resource/fifo.hpp>

class LdpFifoProvider : public LdpFifo
{
public:
    SourcePtr createSource() override
    {
        return SourcePtr(new LdpFifoSource(), [this](LdpFifoSource *source) { destroySourcePtr(source); });
    }

    SinkPtr createSink() override
    {
        return SinkPtr(new LdpFifoSink(), [this](LdpFifoSink *sink) { destroySinkPtr(sink); });
    }
    
    virtual bool configure(LdpConfig* config) = 0;

protected:
    
    virtual LdpFifoSource* createSourcePtr() = 0;
    
    virtual LdpFifoSink* createSinkPtr() = 0;

    virtual void destroySourcePtr(LdpFifoSource* source)
    {
        delete source;
    }
    
    virtual void destroySinkPtr(LdpFifoSink* sink)
    {
        delete sink;
    }
};



#endif