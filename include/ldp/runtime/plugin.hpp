#ifndef LDP_RUNTIME_PLUGIN_HPP
#define LDP_RUNTIME_PLUGIN_HPP

#include <ldp/core/config.hpp>
#include <ldp/resource/fifo.hpp>

struct LdpResourcePlugin
{
    virtual LdpFifo *createFifo(LdpConfig* config) = 0;
    virtual void destroyFifo(LdpFifo *fifo) = 0;
};



#endif