#ifndef LDP_FIFO_HPP
#define LDP_FIFO_HPP

#include <ldp/core/object.hpp>

struct LdpFifoSource : Object<LdpFifoSource>
{
    virtual bool read(std::ostream& output) = 0;
};

struct LdpFifoSink : Object<LdpFifoSink>
{
    virtual bool write(std::istream& input) = 0;
};

struct LdpFifo : Object<LdpFifo>
{
    virtual LdpFifoSource::Ptr acquireSource() = 0;
    virtual LdpFifoSink::Ptr acquireSink() = 0;
};

#endif // LDP_FIFO_HPP