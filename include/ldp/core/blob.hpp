#ifndef LDP_CORE_BLOB_HPP
#define LDP_CORE_BLOB_HPP

#include <ldp/common/object.hpp>

const size_t LDP_BLOB_DEFAULT_BLOCK_SIZE = 0; // Let the underlying implementation decide the block size
const size_t LDP_BLOB_DEFAULT_EXPIRATION = 0; // Let the underlying implementation decide the expiration time

struct LdpBlobSource : LdpObject<LdpBlobSource>
{
    using Callback = std::function<void(const uint8_t*, size_t, bool)>;
    virtual bool read(const std::string_view& bucket, const std::string_view& key, uint8_t* buffer, size_t maxSize, size_t& actualSize) = 0;
    virtual bool read(const std::string_view& bucket, const std::string_view& key, Callback callback, size_t blockSize = LDP_BLOB_DEFAULT_BLOCK_SIZE) = 0;
    virtual bool read(const std::string_view& bucket, const std::string_view& key, std::ostream& output, size_t blockSize = LDP_BLOB_DEFAULT_BLOCK_SIZE) = 0;
};

struct LdpBlobSink : LdpObject<LdpBlobSink>
{
    using Callback = std::function<void(uint8_t*, size_t&, bool&)>;
    virtual bool write(const std::string_view& bucket, const std::string_view& key, const uint8_t* buffer, size_t size) = 0;
    virtual bool write(const std::string_view& bucket, const std::string_view& key, Callback callback, size_t blockSize = LDP_BLOB_DEFAULT_BLOCK_SIZE) = 0;
    virtual bool write(const std::string_view& bucket, const std::string_view& key, std::istream& input, size_t blockSize = LDP_BLOB_DEFAULT_BLOCK_SIZE) = 0;
};

struct LdpBlobClient : LdpObject<LdpBlobClient>
{
    using Callback = std::function<void(const std::string_view&)>;
    virtual bool presignDownload(const std::string_view& bucket, const std::string_view& key, Callback callback, size_t expiration = LDP_BLOB_DEFAULT_EXPIRATION) = 0;
    virtual bool presignUpload(const std::string_view& bucket, const std::string_view& key, Callback callback, size_t expiration = LDP_BLOB_DEFAULT_EXPIRATION) = 0;
};

struct LdpBlobStorage : LdpObject<LdpBlobStorage>
{
    virtual LdpBlobSource::Ptr acquireSource() = 0;
    virtual LdpBlobSink::Ptr acquireSink() = 0;
    virtual LdpBlobClient::Ptr acquireClient() = 0;
};

#endif