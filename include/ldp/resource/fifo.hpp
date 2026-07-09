#ifndef LDP_FIFO_HPP
#define LDP_FIFO_HPP

#include <functional>
#include <iostream>
#include <memory>

struct LdpFifoSource
{
    using Ptr = std::unique_ptr<LdpFifoSource, std::function<void(LdpFifoSource*)>>;
    virtual bool read(std::ostream& output) = 0;
};

struct LdpFifoSink
{
    using Ptr = std::unique_ptr<LdpFifoSink, std::function<void(LdpFifoSink*)>>;
    virtual bool write(std::istream& input) = 0;
};

struct LdpFifo
{
    using Ptr = std::unique_ptr<LdpFifo, std::function<void(LdpFifo*)>>;
    virtual LdpFifoSource::Ptr createSource() = 0;
    virtual LdpFifoSink::Ptr createSink() = 0;
};

#endif // LDP_FIFO_HPP