#ifndef LDP_FIFO_HPP
#define LDP_FIFO_HPP

#include <functional>
#include <iostream>
#include <memory>

struct LdpFifoSource
{
    bool read(std::ostream& output);
};

struct LdpFifoSink
{
    bool write(std::ostream& input);
};

struct LdpFifo
{
    using SourcePtr = std::unique_ptr<LdpFifoSource, std::function<void(LdpFifoSource*)>>;
    using SinkPtr = std::unique_ptr<LdpFifoSink, std::function<void(LdpFifoSink*)>>;
    virtual SourcePtr createSource() = 0;
    virtual SinkPtr createSink() = 0;
};

using LdpFifoPtr = std::unique_ptr<LdpFifo, std::function<void(LdpFifo*)>>;

#endif // LDP_FIFO_HPP