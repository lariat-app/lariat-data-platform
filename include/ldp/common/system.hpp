#ifndef LDP_COMMON_SYSTEM_HPP
#define LDP_COMMON_SYSTEM_HPP

#ifdef _WIN32
    #define LDP_EXPORT_DLL __declspec(dllexport)
#else
    #define LDP_EXPORT_DLL __attribute__((visibility("default")))
#endif

#define LDP_EXPORT_FACTORY(FACTORY_TYPE, OBJECT_TYPE) \
    extern "C" LDP_EXPORT_DLL FACTORY_TYPE *get##FACTORY_TYPE() \
    { \
        static ldp::FACTORY_TYPE<OBJECT_TYPE> factory; \
        return &factory; \
    }

#endif