#ifndef LDPX_CONFIG_HPP
#define LDPX_CONFIG_HPP

#include <ldp/framework/config.hpp>
#include <ldp/framework/logger.hpp>

#include <yyjson.h>

#include <istream>
#include <string>
#include <variant>

namespace ldpx
{
    class Config : public LdpConfig
    {
    public:
        Config() = default;
        ~Config() = default;
        bool parse(const std::string &path, LdpLogger &logger);
        LdpConfigType getType() const override;
        bool getInt(int &value) const override;
        bool getFloat(float &value) const override;
        bool getString(std::string_view &value) const override;
        bool getBool(bool &value) const override;
        bool getArrayItems(const ArrayItemCallback &callback) const override;
        bool getObjectItems(const ObjectItemCallback &callback) const override;
    private:
        bool parseValue(yyjson_val *val, LdpLogger &logger);
        bool parseArray(yyjson_val *val, LdpLogger &logger);
        bool parseObject(yyjson_val *val, LdpLogger &logger);
        using ArrayValue = std::vector<std::unique_ptr<Config>>;
        using ObjectValue = std::map<std::string, std::unique_ptr<Config>>;
        using Value = std::variant<std::monostate, bool, int, float, std::string, ArrayValue, ObjectValue>;
        LdpConfigType _type{LdpConfigType::Null};
        Value _value;
    };
}

#endif // LDPX_CONFIG_HPP