#include "nox_assert.h"
#include "renderer_factory_registry.h"
#include "plugins/plugin_interface.h"

#if defined(NOX_BUILD_RENDERER_OPENGL)
NOX_PLUGIN_IMPORT(OPENGL);
#endif

namespace nox {

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

bool RendererFactoryRegistry::loadPlugin(RendererBackend backend) {
    if (contains(backend)) {
        return true;
    }

    auto plugin = createRendererPlugin(backend);
    NOX_ENSURE_RETURN_FALSE_MSG(plugin != nullptr, "Couldn't create renderer plugin");
    NOX_ENSURE_RETURN_FALSE_MSG(plugin->pluginRegister(), "Couldn't register renderer plugin");
    NOX_ENSURE_RETURN_FALSE_MSG(plugin->pluginVersion() == NOX_PLUGIN_API_VERSION, "Plugin API version mismatch");
    NOX_ENSURE_RETURN_FALSE_MSG(contains(backend), "Couldn't register renderer factory");

    const auto index = static_cast<size_t>(backend);
    m_plugins.at(index) = std::move(plugin);

    return true;
}

void RendererFactoryRegistry::registerFactory(RendererBackend backend, const RendererFactory &factory) {
    const auto index = static_cast<size_t>(backend);
    m_factories.at(index) = factory;
}

RendererPtr RendererFactoryRegistry::createRenderer(RendererBackend backend) {
    NOX_ENSURE_RETURN_NULLPTR_MSG(contains(backend), "Factory for given renderer backend isn't registered");

    const auto index = static_cast<size_t>(backend);
    const auto &[createRenderer, destroyRenderer] = m_factories.at(index);

    return {createRenderer(), destroyRenderer};
}

bool RendererFactoryRegistry::contains(RendererBackend backend) const {
    const auto index = static_cast<size_t>(backend);
    NOX_ENSURE_RETURN_FALSE(index < m_factories.size());
    const auto &factory = m_factories.at(index);

    return (factory.first != nullptr) &&
           (factory.second != nullptr);
}

} // namespace nox
