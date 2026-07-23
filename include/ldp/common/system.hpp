#ifndef LDP_COMMON_SYSTEM_HPP
#define LDP_COMMON_SYSTEM_HPP

#ifdef _WIN32
#define LDP_EXPORT_DLL __declspec(dllexport)
#else
#define LDP_EXPORT_DLL __attribute__((visibility("default")))
#endif

#define LDP_EXPORT_FACTORY(TYPE, IMPL)           \
    extern "C" LDP_EXPORT_DLL TYPE *getFactory() \
    {                                            \
        static IMPL factory;                     \
        return &factory;                         \
    }

#endif