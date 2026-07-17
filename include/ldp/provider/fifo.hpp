#ifndef LDP_PROVIDER_FIFO_HPP
#define LDP_PROVIDER_FIFO_HPP

#include <ldp/core/config.hpp>
#include <ldp/core/logger.hpp>
#include <ldp/resource/fifo.hpp>

class LdpFifoBase : public LdpFifo
{
public:
    virtual bool configure(const LdpConfig &config, LdpLogger &logger) = 0;
protected:
    virtual LdpFifoSource *createSource() = 0;
    virtual LdpFifoSink *createSink() = 0;
};

struct LdpFifoProvider
{
    virtual LdpFifo* createFifo(const LdpConfig &config, LdpLogger &logger) const = 0;
    virtual void destroyFifo(LdpFifo* fifo) const = 0;
};


#endif