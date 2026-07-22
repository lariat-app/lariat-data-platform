#include <config.hpp>

bool ldpx::Config::parse(const std::string &path, LdpLogger &logger)
{
    bool result(false);
    yyjson_read_err err;
    std::unique_ptr<yyjson_doc, decltype(&yyjson_doc_free)> doc(yyjson_read_file(path.c_str(), 0, NULL, &err), &yyjson_doc_free);
    if (doc)
    {
        yyjson_val *root = yyjson_doc_get_root(doc.get());
        result = parseValue(root, logger);
    }
    else
    {
        logger.error("Failed to parse config file: %s, error: %s, position: %zu", path.c_str(), err.msg, err.pos);
    }
    return result;
}

LdpConfigType ldpx::Config::getType() const
{
    return _type;
}

bool ldpx::Config::getInt(int &value) const
{
    if (_type == LdpConfigType::Integer)
    {
        value = std::get<int>(_value);
        return true;
    }
    return false;
}

bool ldpx::Config::getFloat(float &value) const
{
    if (_type == LdpConfigType::Float)
    {
        value = std::get<float>(_value);
        return true;
    }
    return false;
}

bool ldpx::Config::getString(std::string_view &value) const
{
    if (_type == LdpConfigType::String)
    {
        value = std::get<std::string>(_value);
        return true;
    }
    return false;
}

bool ldpx::Config::getBool(bool &value) const
{
    if (_type == LdpConfigType::Boolean)
    {
        value = std::get<bool>(_value);
        return true;
    }
    return false;
}

bool ldpx::Config::getArrayItems(const ArrayItemCallback &callback) const
{
    if (_type == LdpConfigType::Array)
    {
        const ArrayValue &array = std::get<ArrayValue>(_value);
        for (const auto &item : array)
        {
            callback(item.get());
        }
        return true;
    }
    return false;
}

bool ldpx::Config::getObjectItems(const ObjectItemCallback &callback) const
{
    if (_type == LdpConfigType::Object)
    {
        const ObjectValue &object = std::get<ObjectValue>(_value);
        for (const auto &pair : object)
        {
            callback(pair.first, pair.second.get());
        }
        return true;
    }
    return false;
}

bool ldpx::Config::parseValue(yyjson_val *val, LdpLogger &logger)
{
    bool result(true);
    if (val)
    {
        yyjson_type type = yyjson_get_type(val);
        switch (type)
        {
        case YYJSON_TYPE_NULL:
            _type = LdpConfigType::Null;
            break;
        case YYJSON_TYPE_BOOL:
            _type = LdpConfigType::Boolean;
            _value = static_cast<bool>(yyjson_get_bool(val));
            break;
        case YYJSON_TYPE_NUM:
            if (yyjson_is_int(val))
            {
                _type = LdpConfigType::Integer;
                _value = static_cast<int>(yyjson_get_int(val));
            }
            else if (yyjson_is_real(val))
            {
                _type = LdpConfigType::Float;
                _value = static_cast<float>(yyjson_get_real(val));
            }
            else
            {
                logger.error("Unsupported number type in config");
                result = false;
            }
            break;
        case YYJSON_TYPE_STR:
            _type = LdpConfigType::String;
            _value = std::string(yyjson_get_str(val), yyjson_get_len(val));
            break;
        case YYJSON_TYPE_ARR:
            result = parseArray(val, logger);
            break;
        case YYJSON_TYPE_OBJ:
            result = parseObject(val, logger);
            break;
        }
    }
    return result;
}

bool ldpx::Config::parseArray(yyjson_val *val, LdpLogger &logger)
{
    _type = LdpConfigType::Array;
    ArrayValue array;
    yyjson_arr_iter iter;
    yyjson_val *item;
    yyjson_arr_iter_init(val, &iter);
    while ((item = yyjson_arr_iter_next(&iter)))
    {
        auto configItem = std::make_unique<Config>();
        if (configItem->parseValue(item, logger))
        {
            array.push_back(std::move(configItem));
        }
        else
        {
            logger.error("Failed to parse array item in config");
            return false;
        }
    }
    _value = std::move(array);
    return true;
}

bool ldpx::Config::parseObject(yyjson_val *val, LdpLogger &logger)
{
    _type = LdpConfigType::Object;
    ObjectValue object;
    yyjson_obj_iter iter;
    yyjson_val *key, *value;
    yyjson_obj_iter_init(val, &iter);
    while ((key = yyjson_obj_iter_next(&iter)))
    {
        value = yyjson_obj_iter_get_val(key);
        const char *keyStr = yyjson_get_str(key);
        auto configValue = std::make_unique<Config>();
        if (configValue->parseValue(value, logger))
        {
            object.emplace(keyStr, std::move(configValue));
        }
        else
        {
            logger.error("Failed to parse object item in config for key: %s", keyStr);
            return false;
        }
    }
    _value = std::move(object);
    return true;
}