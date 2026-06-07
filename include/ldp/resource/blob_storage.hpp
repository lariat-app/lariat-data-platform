#ifndef LDP_BLOB_STORAGE_HPP
#define LDP_BLOB_STORAGE_HPP

namespace ldp
{
    class blob_storage
    {
    public:
        virtual void store(const char *key, const char *value) = 0;
    };
}

#endif