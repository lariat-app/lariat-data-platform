#ifndef LDPX_CONFIG_HPP
#define LDPX_CONFIG_HPP

#include <ldp/core/config.hpp>

#include <string>

namespace ldp
{
    class Config : public ldp::Config
    {
    public:
        Config() = default;
        ~Config() = default;
        bool parse(const std::string &input);
    };
}

#endif // LDPX_CONFIG_HPP