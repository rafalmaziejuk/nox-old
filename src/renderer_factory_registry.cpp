#include "assertion.h"
#include "renderer_factory_registry.h"
#include "plugins/plugin_interface.h"

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

bool RendererFactoryRegistry::registerFactory(RendererBackend backend) {
    NOX_ASSERT_RETURN_FALSE(backend < RendererBackend::MAX);

    if (contains(backend)) {
        return true;
    }

    auto plugin = createRendererPlugin(backend);
    NOX_ASSERT_RETURN_FALSE_MSG(plugin != nullptr, "Couldn't create renderer plugin");
    NOX_ASSERT_RETURN_FALSE_MSG(plugin->pluginVersion() == NOX_PLUGIN_API_VERSION, "Plugin API version mismatch");
    NOX_ASSERT_RETURN_FALSE_MSG(plugin->pluginRegister(), "Couldn't register renderer plugin");

    const auto index = static_cast<size_t>(backend);
    m_plugins.at(index) = std::move(plugin);

    return true;
}

bool RendererFactoryRegistry::contains(RendererBackend backend) const {
    const auto index = static_cast<size_t>(backend);
    if (index >= m_factories.size()) {
        return false;
    }

    const auto &factory = m_factories.at(index);
    return ((factory.first != nullptr) && (factory.second != nullptr));
}

RendererFactory &RendererFactoryRegistry::operator[](RendererBackend backend) {
    const auto index = static_cast<size_t>(backend);
    auto &factory = m_factories.at(index);
    return factory;
}

const RendererFactory &RendererFactoryRegistry::operator[](RendererBackend backend) const {
    const auto index = static_cast<size_t>(backend);
    const auto &factory = m_factories.at(index);
    return factory;
}

} // namespace NOX
