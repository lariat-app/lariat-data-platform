#ifndef LDP_INTERFACE_HPP
#define LDP_INTERFACE_HPP

#include <functional>
#include <memory>

struct LdpInterface
{
    virtual ~LdpInterface() = default;
};

template <typename InterfaceType>
using LdpInterfacePtr = std::unique_ptr<InterfaceType, std::function<void(InterfaceType*)>>;

#endif