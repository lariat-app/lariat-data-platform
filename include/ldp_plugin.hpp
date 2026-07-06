#ifndef LDP_PLUGIN_HPP
#define LDP_PLUGIN_HPP

#include <ldp/core/config.hpp>
#include <ldp/resource/fifo.hpp>
#include <ldp/provider/fifo.hpp>
#include <ldp/runtime/plugin.hpp>

struct LdpDefaultPlugin : public LdpResourcePlugin
{
    LdpDefaultPlugin() = default;
    ~LdpDefaultPlugin() = default;
    LdpFifo *createFifo(LdpConfig* config) override
    {
        return nullptr;
    }

    void destroyFifo(LdpFifo *fifo) override
    {
        // no-op
    }
};


#define LDP_EXPORT_PLUGIN(INSTANCE) \
    extern "C" LdpResourcePlugin* ldp_get_plugin() \
    { \
        return &INSTANCE; \
    }

template <typename FifoProvider>
struct LdpFifoPlugin : public LdpDefaultPlugin
{
    static_assert<std::is_base_of_v<LdpFifoProvider, FifoProvider>, "FifoProvider must derive from LdpFifoProvider">();
    LdpFifo *createFifo(LdpConfig* config) override
    {
        std::unique_ptr<FifoProvider, std::function<void(FifoProvider*)>> provider(new FifoProvider(), [this](FifoProvider *provider) { destroyFifo(provider); });
        if(!provider->configure(config))
        {
            provider.reset();
        }
        return provider.release();
    }

    void destroyFifo(LdpFifo *fifo) override
    {
        delete fifo;
    }
};

#define LDP_FIFO_PLUGIN(PROVIDER) \
    struct FIFO_PLUGIN : public LdpFifoPlugin<PROVIDER> \
    { \
        FIFO_PLUGIN() = default; \
        ~FIFO_PLUGIN() = default; \
    }; \
    static FIFO_PLUGIN instance; \
    LDP_EXPORT_PLUGIN(instance)


#endif