#ifndef LDP_FRAMEWORK_CONFIG_HPP
#define LDP_FRAMEWORK_CONFIG_HPP

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

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
    using Array = std::vector<const LdpConfig *>;
    using ArrayItemCallback = std::function<void(const LdpConfig *)>;
    using Object = std::map<std::string, const LdpConfig *>;
    using ObjectItemCallback = std::function<void(const std::string_view &, const LdpConfig *)>;
    virtual LdpConfigType getType() const = 0;
    virtual bool getInt(int &value) const = 0;
    virtual bool getFloat(float &value) const = 0;
    virtual bool getString(std::string_view &value) const = 0;
    virtual bool getBool(bool &value) const = 0;
    virtual bool getArrayItems(const ArrayItemCallback &callback) const = 0;
    virtual bool getObjectItems(const ObjectItemCallback &callback) const = 0;
    bool getArray(Array &out) const
    {
        return getArrayItems([&out](const LdpConfig *item)
                             { out.push_back(item); });
    }
    bool getObject(Object &out) const
    {
        return getObjectItems([&out](const std::string_view &key, const LdpConfig *value)
                              { out.emplace(key, value); });
    }
    class ObjectReader
    {
    public:
        ObjectReader(const Object &object) : _object(object) {}
        bool getInt(const std::string_view &key, int &value) const
        {
            const LdpConfig *config(nullptr);
            return getItem(key, &config) && config->getInt(value);
        }
        bool getFloat(const std::string_view &key, float &value) const
        {
            const LdpConfig *config(nullptr);
            return getItem(key, &config) && config->getFloat(value);
        }
        bool getString(const std::string_view &key, std::string_view &value) const
        {
            const LdpConfig *config(nullptr);
            return getItem(key, &config) && config->getString(value);
        }
        bool getBool(const std::string_view &key, bool &value) const
        {
            const LdpConfig *config(nullptr);
            return getItem(key, &config) && config->getBool(value);
        }
        bool getArray(const std::string_view &key, Array &value) const
        {
            const LdpConfig *config(nullptr);
            return getItem(key, &config) && config->getArray(value);
        }
        bool getObject(const std::string_view &key, Object &value) const
        {
            const LdpConfig *config(nullptr);
            return getItem(key, &config) && config->getObject(value);
        }
        bool hasKey(const std::string_view &key) const
        {
            const LdpConfig *config(nullptr);
            return getItem(key, &config);
        }
        bool getItem(const std::string_view &key, const LdpConfig **value) const
        {
            bool result(false);
            auto it = _object.find(std::string(key));
            if (it != _object.end())
            {
                *value = it->second;
                result = true;
            }
            return result;
        }

    private:
        const Object &_object;
    };

    class ArrayReader
    {
    public:
        ArrayReader(const Array &array) : _array(array) {}
        bool getInt(size_t index, int &value) const
        {
            const LdpConfig *config(nullptr);
            return getItem(index, &config) && config->getInt(value);
        }
        bool getFloat(size_t index, float &value) const
        {
            const LdpConfig *config(nullptr);
            return getItem(index, &config) && config->getFloat(value);
        }
        bool getString(size_t index, std::string_view &value) const
        {
            const LdpConfig *config(nullptr);
            return getItem(index, &config) && config->getString(value);
        }
        bool getBool(size_t index, bool &value) const
        {
            const LdpConfig *config(nullptr);
            return getItem(index, &config) && config->getBool(value);
        }
        bool getArray(size_t index, Array &value) const
        {
            const LdpConfig *config(nullptr);
            return getItem(index, &config) && config->getArray(value);
        }
        bool getObject(size_t index, Object &value) const
        {
            const LdpConfig *config(nullptr);
            return getItem(index, &config) && config->getObject(value);
        }
        size_t size() const
        {
            return _array.size();
        }

        bool getItem(size_t index, const LdpConfig **value) const
        {
            bool result(false);
            if (index < _array.size())
            {
                *value = _array[index];
                result = true;
            }
            return result;
        }

    private:
        const Array &_array;
    };
};

#endif