#ifndef LDP_RUNTIME_PLUGIN_HPP
#define LDP_RUNTIME_PLUGIN_HPP

#include <ldp/core/config.hpp>
#include <ldp/resource/fifo.hpp>

struct LdpPlugin
{
    using Ptr = std::unique_ptr<LdpPlugin, std::function<void(LdpPlugin*)>>;
    virtual LdpFifo *createFifo(LdpConfig* config) const = 0;
    virtual void destroyFifo(LdpFifo *fifo) const = 0;
};



#endif