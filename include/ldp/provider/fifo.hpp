#ifndef LDP_PROVIDER_FIFO_HPP
#define LDP_PROVIDER_FIFO_HPP

#include <ldp/core/config.hpp>
#include <ldp/resource/fifo.hpp>

class LdpFifoProvider : public LdpFifo
{
public:
    LdpFifoSource::Ptr createSource() override final
    {
        return LdpFifoSource::Ptr(createSourcePtr(), [this](LdpFifoSource *source) { destroySourcePtr(source); });
    }

    LdpFifoSink::Ptr createSink() override final
    {
        return LdpFifoSink::Ptr(createSinkPtr(), [this](LdpFifoSink *sink) { destroySinkPtr(sink); });
    }
    
    virtual bool configure(LdpConfig* config) = 0;
    
    virtual LdpFifoSource* createSourcePtr() = 0;
    
    virtual LdpFifoSink* createSinkPtr() = 0;

protected:

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