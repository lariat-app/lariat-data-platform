#include <ldp_plugin.hpp>

#include <miniocpp/client.h>

class MinioBlobStorage : public ldp::BlobStorage
{
private:
    std::unique_ptr<minio::creds::Provider> _credentials;
    std::unique_ptr<minio::s3::Client> _client;
public:
    bool configure(const LdpConfig &config, LdpLogger &logger) override
    {
        return true;
    }
};