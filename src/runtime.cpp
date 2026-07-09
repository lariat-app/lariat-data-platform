#include <ldp.hpp>

LdpRuntime::Ptr LdpInitialize()
{
    return LdpRuntime::Ptr(new ldp::Runtime(), [](LdpRuntime* runtime) { delete runtime; });
}