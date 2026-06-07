#include <ldp_runtime.hpp>
#include <yyjson.h>

class json_object : public ldp::config_object
{
public:
    json_object(yyjson_val *obj) : _obj(obj) {}
    ~json_object() = default;

    bool get_array(const std::string &key, std::unique_ptr<ldp::config_array> &array) const override;
    bool get_object(const std::string &key, std::unique_ptr<ldp::config_object> &object) const override;

    bool get_bool(const std::string &key, bool &value) const override
    {
        bool result(false);
        yyjson_val *val = yyjson_obj_get(_obj, key.c_str());
        if (val && yyjson_is_bool(val))
        {
            value = yyjson_get_bool(val);
            result = true;
        }
        return result;
    }
    
    bool get_double(const std::string &key, double &value) const override
    {
        bool result(false);
        yyjson_val *val = yyjson_obj_get(_obj, key.c_str());
        if (val && yyjson_is_num(val))
        {
            value = yyjson_get_num(val);
            result = true;
        }
        return result;
    }

    bool get_int(const std::string &key, int &value) const override
    {
        bool result(false);
        yyjson_val *val = yyjson_obj_get(_obj, key.c_str());
        if (val && yyjson_is_num(val))
        {
            value = static_cast<int>(yyjson_get_num(val));
            result = true;
        }
        return result;
    }
    
    bool get_string(const std::string &key, std::string &value) const override
    {
        bool result(false);
        yyjson_val *val = yyjson_obj_get(_obj, key.c_str());
        if (val && yyjson_is_str(val))
        {
            value = std::string(yyjson_get_str(val), yyjson_get_len(val));
            result = true;
        }
        return result;
    }
    
    bool is_null(const std::string &key) const override
    {
        bool result(false);
        yyjson_val *val = yyjson_obj_get(_obj, key.c_str());
        if (val && yyjson_is_null(val))
        {
            result = true;
        }
        return result;

    }
    
    bool has_key(const std::string &key) const override
    {
        bool result(false);
        yyjson_val *val = yyjson_obj_get(_obj, key.c_str());
        if (val)
        {
            result = true;
        }
        return result;
    }

private:
    yyjson_val *_obj;
};

class json_array : public ldp::config_array
{
public:
    json_array(yyjson_val *arr) : _arr(arr) {}
    ~json_array() = default;

    bool get_array(size_t index, std::unique_ptr<ldp::config_array> &array) const override;
    bool get_object(size_t index, std::unique_ptr<ldp::config_object> &object) const override;

    bool get_bool(size_t index, bool &value) const override
    {
        bool result(false);
        yyjson_val *val = yyjson_arr_get(_arr, index);
        if (val && yyjson_is_bool(val))
        {
            value = yyjson_get_bool(val);
            result = true;
        }
        return result;
    }

    bool get_double(size_t index, double &value) const override
    {
        bool result(false);
        yyjson_val *val = yyjson_arr_get(_arr, index);
        if (val && yyjson_is_num(val))
        {
            value = yyjson_get_num(val);
            result = true;
        }
        return result;
    }

    bool get_int(size_t index, int &value) const override
    {
        bool result(false);
        yyjson_val *val = yyjson_arr_get(_arr, index);
        if (val && yyjson_is_num(val))
        {
            value = static_cast<int>(yyjson_get_num(val));
            result = true;
        }
        return result;
    }

    bool get_string(size_t index, std::string &value) const override
    {
        bool result(false);
        yyjson_val *val = yyjson_arr_get(_arr, index);
        if (val && yyjson_is_str(val))
        {
            value = std::string(yyjson_get_str(val), yyjson_get_len(val));
            result = true;
        }
        return result;
    }

    bool is_null(size_t index) const override
    {
        bool result(false);
        yyjson_val *val = yyjson_arr_get(_arr, index);
        if (val && yyjson_is_null(val))
        {
            result = true;
        }
        return result;
    }

    size_t size() const override
    {
        return yyjson_arr_size(_arr);
    }

private:
    yyjson_val *_arr;
};

class json_config : public ldp::config
{
public:
    json_config() = default;
    ~json_config() = default;

    bool read(const char *data, size_t size)
    {
        bool result(false);
        yyjson_doc *doc = yyjson_read(data, size, 0);
        if (doc)
        {
            _doc.reset(doc);
            _root_object = std::make_unique<json_object>(yyjson_doc_get_root(doc));
            _root_object->get_array("plugins", _plugins);
            _root_object->get_object("services", _services);
            _root_object->get_object("resources", _resources);
            result = true;
        }
        return result;
    }

    ldp::config_array *get_plugins() const override
    {
        return _plugins.get();
    }

    ldp::config_object *get_services() const override
    {
        return _services.get();
    }

    ldp::config_object *get_resources() const override
    {
        return _resources.get();
    }

private:
    std::unique_ptr<yyjson_doc, decltype(&yyjson_doc_free)> _doc{nullptr, &yyjson_doc_free};
    std::unique_ptr<json_object> _root_object;
    std::unique_ptr<ldp::config_array> _plugins;
    std::unique_ptr<ldp::config_object> _services;
    std::unique_ptr<ldp::config_object> _resources;
};

bool json_object::get_array(const std::string &key, std::unique_ptr<ldp::config_array> &array) const
{
    bool result(false);
    yyjson_val *val = yyjson_obj_get(_obj, key.c_str());
    if (val && yyjson_is_arr(val))
    {
        array = std::make_unique<json_array>(val);
        result = true;
    }
    return result;
}

bool json_object::get_object(const std::string &key, std::unique_ptr<ldp::config_object> &object) const
{
    bool result(false);
    yyjson_val *val = yyjson_obj_get(_obj, key.c_str());
    if (val && yyjson_is_obj(val))
    {
        object = std::make_unique<json_object>(val);
        result = true;
    }
    return result;
}

bool json_array::get_array(size_t index, std::unique_ptr<ldp::config_array> &array) const
{
    bool result(false);
    yyjson_val *val = yyjson_arr_get(_arr, index);
    if (val && yyjson_is_arr(val))
    {
        array = std::make_unique<json_array>(val);
        result = true;
    }
    return result;
}

bool json_array::get_object(size_t index, std::unique_ptr<ldp::config_object> &object) const
{
    bool result(false);
    yyjson_val *val = yyjson_arr_get(_arr, index);
    if (val && yyjson_is_obj(val))
    {
        object = std::make_unique<json_object>(val);
        result = true;
    }
    return result;
}

bool ldp::runtime::read_config(std::istream &source, std::unique_ptr<ldp::config> &config)
{
    std::string buffer;
    buffer.reserve(1024);
    return read_config_preprocess(source, buffer) && read_config_parse(buffer, config);
}

bool ldp::runtime::read_config_preprocess(std::istream &source, std::string &config_data)
{
    bool result(false);
    if (source)
    {
        config_data.assign((std::istreambuf_iterator<char>(source)), std::istreambuf_iterator<char>());
        result = true;
    }
    return result;
}

bool ldp::runtime::read_config_parse(const std::string &config_data, std::unique_ptr<ldp::config> &config)
{
    bool result(false);
    std::unique_ptr<json_config> json;
    json.reset(new json_config());
    if (json->read(config_data.c_str(), config_data.size()))
    {
        config.reset(json.release());
        result = true;
    }
    return result;
}