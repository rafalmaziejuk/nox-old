#include "plugins/plugin_interface.h"
#include "renderer/renderer_factory_registry.h"

#if defined(NOX_BUILD_RENDERER_OPENGL)
NOX_PLUGIN_IMPORT(OPENGL);
#endif

namespace NOX {

namespace {

std::unique_ptr<Plugin> createRendererPlugin(RendererBackend backend) {
#if defined(NOX_BUILD_RENDERER_OPENGL)
    if (backend == RendererBackend::OPENGL) {
        return NOX_PLUGIN_CREATE(OPENGL);
    }
#endif

    return nullptr;
}

} // namespace

RendererFactoryRegistry &RendererFactoryRegistry::instance() {
    static RendererFactoryRegistry registry{};
    return registry;
}

bool RendererFactoryRegistry::initializeFactory(RendererBackend backend) {
    if (isFactoryRegistered(backend)) {
        return true;
    }

    auto plugin = createRendererPlugin(backend);
    if (plugin != nullptr) {
        if (plugin->pluginVersion() == NOX_PLUGIN_API_VERSION) {
            if (plugin->pluginRegister()) {
                const auto index = static_cast<size_t>(backend);
                m_plugins.at(index) = std::move(plugin);
                return true;
            }
        }
    }

    return false;
}

bool RendererFactoryRegistry::isFactoryRegistered(RendererBackend backend) const {
    const auto index = static_cast<size_t>(backend);
    const auto &factory = m_factories.at(index);
    return ((factory.first != nullptr) && (factory.second != nullptr));
}

RendererFactory &RendererFactoryRegistry::operator[](RendererBackend backend) {
    const auto index = static_cast<size_t>(backend);
    auto &factory = m_factories.at(index);
    return factory;
}

RendererFactory RendererFactoryRegistry::operator[](RendererBackend backend) const {
    const auto index = static_cast<size_t>(backend);
    const auto &factory = m_factories.at(index);
    return factory;
}

} // namespace NOX
