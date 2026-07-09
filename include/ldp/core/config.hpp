#ifndef LDP_CORE_CONFIG_HPP
#define LDP_CORE_CONFIG_HPP

#include <memory>
#include <functional>
#include <map>
#include <string_view>

enum class LdpConfigType
{
    Null,
    Boolean,
    Integer,
    Float,
    String,
    Array,
    Object
};

struct LdpConfig
{
    using Ptr = std::unique_ptr<LdpConfig, std::function<void(LdpConfig*)>>;
    using ArrayItemCallback = std::function<void(const LdpConfig*)>;
    using ObjectItemCallback = std::function<void(const std::string_view&, const LdpConfig*)>;
    virtual LdpConfigType getType() const = 0;
    virtual bool getInt(int& value) const = 0;
    virtual bool getFloat(float& value) const = 0;
    virtual bool getString(std::string_view& value) const = 0;
    virtual bool getBool(bool& value) const = 0;
    virtual bool getArrayItems(const ArrayItemCallback& callback) const = 0;
    virtual bool getObjectItems(const ObjectItemCallback& callback) const = 0;
    bool getArray(std::vector<const LdpConfig*>& out) const
    {
        return getArrayItems([&out](const LdpConfig* item) {
            out.push_back(item);
        });
    }
    bool getObject(std::map<std::string, const LdpConfig*>& out) const
    {
        return getObjectItems([&out](const std::string_view& key, const LdpConfig* value) {
            out.emplace(key, value);
        });
    }};

#endif