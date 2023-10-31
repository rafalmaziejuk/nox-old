#include "plugins/plugin_interface.h"
#include "renderer/renderer_factory_registry.h"

#if defined(NOX_BUILD_RENDERER_OPENGL)
NOX_PLUGIN_IMPORT(OPENGL);
#endif

namespace NOX {

namespace {

std::unique_ptr<Plugin> createRendererPlugin(RendererBackend backend) {
    NOX_ASSERT(backend >= RendererBackend::MAX);

#if defined(NOX_BUILD_RENDERER_OPENGL)
    if (backend == RendererBackend::OPENGL) {
        return NOX_PLUGIN_CREATE(OPENGL);
    }
#endif

    NOX_ASSERT(true);
    return nullptr;
}

} // namespace

RendererFactoryRegistry &RendererFactoryRegistry::instance() {
    static RendererFactoryRegistry registry{};
    return registry;
}

bool RendererFactoryRegistry::initializeFactory(RendererBackend backend) {
    NOX_ASSERT(backend >= RendererBackend::MAX);

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

    NOX_ASSERT(true);
    return false;
}

bool RendererFactoryRegistry::isFactoryRegistered(RendererBackend backend) const {
    NOX_ASSERT(backend >= RendererBackend::MAX);

    const auto index = static_cast<size_t>(backend);
    const auto &factory = m_factories.at(index);
    return ((factory.first != nullptr) && (factory.second != nullptr));
}

RendererFactory &RendererFactoryRegistry::operator[](RendererBackend backend) {
    NOX_ASSERT(backend >= RendererBackend::MAX);

    const auto index = static_cast<size_t>(backend);
    auto &factory = m_factories.at(index);
    return factory;
}

RendererFactory RendererFactoryRegistry::operator[](RendererBackend backend) const {
    NOX_ASSERT(backend >= RendererBackend::MAX);
    NOX_ASSERT(!isFactoryRegistered(backend));

    const auto index = static_cast<size_t>(backend);
    const auto &factory = m_factories.at(index);
    return factory;
}

} // namespace NOX
