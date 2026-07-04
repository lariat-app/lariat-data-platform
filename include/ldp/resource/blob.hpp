#ifndef LDP_BLOB_HPP
#define LDP_BLOB_HPP

#include <functional>
#include <iostream>
#include <memory>
#include <string>

const size_t LDP_BLOB_DEFAULT_BLOCK_SIZE = 0; // Let the underlying implementation decide the block size
const size_t LDP_BLOB_DEFAULT_EXPIRATION = 0; // Let the underlying implementation decide the expiration time

struct LdpBlobSource
{
    using Callback = std::function<void(const uint8_t*, size_t, bool)>;
    virtual bool read(const std::string& bucket, const std::string& key, uint8_t* buffer, size_t maxSize, size_t& actualSize) = 0;
    virtual bool read(const std::string& bucket, const std::string& key, Callback callback, size_t blockSize = LDP_BLOB_DEFAULT_BLOCK_SIZE) = 0;
    virtual bool read(const std::string& bucket, const std::string& key, std::ostream& output, size_t blockSize = LDP_BLOB_DEFAULT_BLOCK_SIZE) = 0;
};

struct LdpBlobSink
{
    using Callback = std::function<void(uint8_t*, size_t&, bool&)>;
    virtual bool write(const std::string& bucket, const std::string& key, const uint8_t* buffer, size_t size) = 0;
    virtual bool write(const std::string& bucket, const std::string& key, Callback callback, size_t blockSize = LDP_BLOB_DEFAULT_BLOCK_SIZE) = 0;
    virtual bool write(const std::string& bucket, const std::string& key, std::istream& input, size_t blockSize = LDP_BLOB_DEFAULT_BLOCK_SIZE) = 0;
};

struct LdpBlobClient
{
    using Callback = std::function<void(const std::string&)>;
    virtual bool presignDownload(const std::string& bucket, const std::string& key, Callback callback, size_t expiration = LDP_BLOB_DEFAULT_EXPIRATION) = 0;
    virtual bool presignUpload(const std::string& bucket, const std::string& key, Callback callback, size_t expiration = LDP_BLOB_DEFAULT_EXPIRATION) = 0;
};

struct LdpBlobStorage
{
    using SourcePtr = std::unique_ptr<LdpBlobSource, std::function<void(LdpBlobSource*)>>;
    using SinkPtr = std::unique_ptr<LdpBlobSink, std::function<void(LdpBlobSink*)>>;
    using ClientPtr = std::unique_ptr<LdpBlobClient, std::function<void(LdpBlobClient*)>>;
    virtual bool createSource(SourcePtr& source) = 0;
    virtual bool createSink(SinkPtr& sink) = 0;
    virtual bool createClient(ClientPtr& client) = 0;
};

using LdpBlobStoragePtr = std::unique_ptr<LdpBlobStorage, std::function<void(LdpBlobStorage*)>>;

#endif