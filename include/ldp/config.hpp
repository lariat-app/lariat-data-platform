#ifndef LDP_CONFIG_HPP
#define LDP_CONFIG_HPP

#include <memory>
#include <string>

namespace ldp
{
    class config_object;

    class config_array
    {
    public:
        virtual ~config_array() = default;
        virtual bool get_array(size_t index, std::unique_ptr<config_array>& array) const = 0;
        virtual bool get_bool(size_t index, bool& value) const = 0;
        virtual bool get_double(size_t index, double& value) const = 0;
        virtual bool get_int(size_t index, int& value) const = 0;
        virtual bool get_object(size_t index, std::unique_ptr<config_object>& object) const = 0;
        virtual bool get_string(size_t index, std::string& value) const = 0;
        virtual bool is_null(size_t index) const = 0;
        virtual size_t size() const = 0;
    };

    class config_object
    {
    public:
        virtual ~config_object() = default;
        virtual bool get_array(const std::string& key, std::unique_ptr<config_array>& array) const = 0;
        virtual bool get_bool(const std::string& key, bool& value) const = 0;
        virtual bool get_double(const std::string& key, double& value) const = 0;
        virtual bool get_int(const std::string& key, int& value) const = 0;
        virtual bool get_object(const std::string& key, std::unique_ptr<config_object>& object) const = 0;
        virtual bool get_string(const std::string& key, std::string& value) const = 0;
        virtual bool is_null(const std::string& key) const = 0;
        virtual bool has_key(const std::string& key) const = 0;
    };

    class config
    {
    public:
        virtual ~config() = default;
        virtual config_array* get_plugins() const = 0;
        virtual config_object* get_services() const = 0;
        virtual config_object* get_resources() const = 0;
    };
}

#endif