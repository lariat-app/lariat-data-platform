#ifndef CONNECTOR_HPP
#define CONNECTOR_HPP

#include <ldp/resource/consumer.hpp>
#include <ldp/resource/provider.hpp>

#include <shared_mutex>
#include <unordered_map>
#include <variant>

namespace ldpx
{
    enum class ModuleType
    {
        ServiceFactory,
        FifoFactory
    };

    class Module
    {
    public:
        struct Library
        {
            static Library *Create();
            static std::string Extension();
            virtual ~Library() = default;
            virtual bool load(const std::string &path) = 0;
            virtual void *getSymbol(const std::string &name) = 0;
        };
        Module() = default;
        ~Module() = default;
        Module(const Module &) = delete;
        Module &operator=(const Module &) = delete;
        Module(Module &&) noexcept = default;
        Module &operator=(Module &&) noexcept = default;
        template <typename FactoryType>
        bool load(const std::string &path)
        {
            bool result(false);
            _library = std::unique_ptr<Library>(Library::Create());
            if (_library->load(path))
            {
                std::function<FactoryType *()> getFactoryFunc = reinterpret_cast<FactoryType *(*)()>(_library->getSymbol("getFactory"));
                _factory = reinterpret_cast<FactoryType *>(getFactoryFunc());
                result = std::holds_alternative<FactoryType *>(_factory);
            }
            return result;
        }
        template <typename FactoryType>
        FactoryType *getFactory() const
        {
            FactoryType *factory = nullptr;
            if (std::holds_alternative<FactoryType *>(_factory))
            {
                factory = std::get<FactoryType *>(_factory);
            }
            return factory;
        }
    private:
        std::variant<std::monostate, LdpFifoFactory *, LdpServiceFactory *> _factory;
        std::unique_ptr<Library> _library;
    };

    class Connector : public LdpConnector
    {
    public:
        Connector() = default;
        ~Connector() = default;
        bool setup(const LdpConfig &config, LdpLogger &logger);
        LdpFifo::Ptr acquireFifo(const std::string &name) override;
    private:
        template <typename Factory, typename Resource>
        std::unique_ptr<Resource, std::function<void(Resource*)>> acquireResource(const std::string &name)
        {
            _logger->debug("Acquiring resource: %s", name.c_str());
            std::unique_ptr<Resource, std::function<void(Resource*)>> result(nullptr, [](Resource *resource) {});
            LdpConfig::ObjectReader configReader(_resourceConfigs);
            LdpConfig::Object resourceConfig;
            if (configReader.getObject(name, resourceConfig))
            {
                _logger->debug("Found configuration for resource: %s", name.c_str());
                LdpConfig::ObjectReader resourceConfigReader(resourceConfig);
                std::string_view providerView;
                const LdpConfig* resourceProperties(nullptr);
                if (resourceConfigReader.getString("provider", providerView))
                {
                    std::string provider(providerView);
                    if (resourceConfigReader.getItem("properties", &resourceProperties))
                    {
                        _logger->debug("Found provider: %s for resource: %s", provider.c_str(), name.c_str());
                        Module *plugin = getModule<Factory>(provider);
                        if (plugin)
                        {
                            Factory *factory = plugin->getFactory<Factory>();
                            if (factory)
                            {
                                _logger->debug("Found factory for provider: %s", provider.c_str());
                                result = std::unique_ptr<Resource, std::function<void(Resource*)>>(
                                    factory->createObject(*resourceProperties, *_logger), [factory](Resource* resource)
                                {
                                    factory->destroyObject(resource);
                                });
                            }
                            else
                            {
                                _logger->error("Failed to get factory for provider: %s", provider.c_str());
                            }
                        }
                        else
                        {
                            _logger->error("Failed to find module for provider: %s", provider.c_str());
                        }
                    }
                    else
                    {
                        _logger->error("Missing 'properties' item for resource: %s", name.c_str());
                    }
                }
                else
                {
                    _logger->error("Missing 'provider' string for resource: %s", name.c_str());
                }
            }
            else
            {
                _logger->error("Missing configuration for resource: %s", name.c_str());
            }
            return result;
        }
        template<typename Factory>
        Module *getModule(const std::string &providerName)
        {
            Module *result = findModule(providerName);
            if (!result)
            {
                std::string libraryPath("libldp_" + providerName + Module::Library::Extension());
                result = loadModule<Factory>(providerName, libraryPath);
            }
            return result;
        }
        Module *findModule(const std::string &providerName);
        template <typename Factory>
        Module *loadModule(const std::string &providerName, const std::string &libraryPath)
        {
            Module *result(nullptr);
            Module plugin;
            if (plugin.load<Factory>(libraryPath))
            {
                _pluginModules.emplace(providerName, std::move(plugin));
                result = &_pluginModules.at(providerName);
            }
            else
            {
                _logger->error("Failed to load module for provider: %s from library: %s", providerName.c_str(), libraryPath.c_str());
            }
            return result;
        }
        LdpLogger *_logger;
        LdpConfig::Object _resourceConfigs;
        std::unordered_map<std::string, Module> _pluginModules;
    };
}

#endif