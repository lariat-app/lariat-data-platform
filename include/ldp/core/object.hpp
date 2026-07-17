#ifndef LDP_FACTORY_HPP
#define LDP_FACTORY_HPP

#include <functional>
#include <iostream>
#include <memory>

#ifdef _WIN32
    #define LDP_EXPORT_DLL __declspec(dllexport)
#else
    #define LDP_EXPORT_DLL __attribute__((visibility("default")))
#endif

template <typename T>
struct Object
{
    using Ptr = std::unique_ptr<T, std::function<void(T*)>>;
    virtual ~Object() = default;
};

#endif // LDP_FACTORY_HPP