#ifndef LDP_CORE_FIFO_HPP
#define LDP_CORE_FIFO_HPP

#include <ldp/common/object.hpp>

struct LdpFifoSource : LdpObject<LdpFifoSource>
{
    virtual bool read(std::ostream& output) = 0;
};

struct LdpFifoSink : LdpObject<LdpFifoSink>
{
    virtual bool write(std::istream& input) = 0;
};

struct LdpFifo : LdpObject<LdpFifo>
{
    virtual LdpFifoSource::Ptr acquireSource() = 0;
    virtual LdpFifoSink::Ptr acquireSink() = 0;
};


#endif