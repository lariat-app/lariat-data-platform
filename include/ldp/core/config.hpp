#ifndef LDP_CORE_CONFIG_HPP
#define LDP_CORE_CONFIG_HPP

#include <memory>
#include <functional>

struct LdpConfig
{
    using Ptr = std::unique_ptr<LdpConfig, std::function<void(LdpConfig*)>>;
    int placeHolder;
};

using LdpConfigPtr = std::unique_ptr<LdpConfig, std::function<void(LdpConfig*)>>;

#endif