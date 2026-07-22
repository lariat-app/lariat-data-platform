#ifndef LDP_COMMON_OBJECT_HPP
#define LDP_COMMON_OBJECT_HPP

#include <functional>
#include <iostream>
#include <memory>

template <typename T>
struct LdpObject
{
    using Ptr = std::unique_ptr<T, std::function<void(T*)>>;
    virtual ~LdpObject() = default;
};

#endif