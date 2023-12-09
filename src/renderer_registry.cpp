#include "nox_assert.h"
#include "renderer_registry.h"
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

RendererRegistry &RendererRegistry::instance() {
    static RendererRegistry registry{};
    return registry;
}

bool RendererRegistry::loadPlugin(RendererBackend backend) {
    if (containsFactory(backend)) {
        return true;
    }

    auto plugin = createRendererPlugin(backend);
    NOX_ENSURE_RETURN_FALSE_MSG(plugin != nullptr, "Couldn't create renderer plugin");
    NOX_ENSURE_RETURN_FALSE_MSG(plugin->pluginRegister(), "Couldn't register renderer plugin");
    NOX_ENSURE_RETURN_FALSE_MSG(plugin->pluginVersion() == NOX_PLUGIN_API_VERSION, "Plugin API version mismatch");
    NOX_ENSURE_RETURN_FALSE_MSG(containsFactory(backend), "Couldn't register renderer factory");

    const auto index = static_cast<size_t>(backend);
    m_plugins.at(index) = std::move(plugin);

    return true;
}

void RendererRegistry::registerFactory(RendererBackend backend, const RendererFactory &factory) {
    const auto index = static_cast<size_t>(backend);
    m_factories.at(index) = factory;
}

RendererPtr RendererRegistry::createRenderer(RendererBackend backend) {
    NOX_ENSURE_RETURN_NULLPTR_MSG(containsFactory(backend), "Factory for given renderer backend isn't registered");

    const auto index = static_cast<size_t>(backend);
    const auto &[createRenderer, destroyRenderer] = m_factories.at(index);
    auto &renderersContainer = m_renderersContainers.at(index);
    renderersContainer.push_back({createRenderer(), destroyRenderer});

    return renderersContainer.back();
}

bool RendererRegistry::releaseRenderer(RendererPtr &&renderer) {
    const auto index = static_cast<size_t>(renderer->getRendererBackend());
    auto &renderersContainer = m_renderersContainers.at(index);
    auto it = std::find_if(renderersContainer.begin(), renderersContainer.end(), [&](const RendererPtr &other) {
        return (other == renderer);
    });
    NOX_ENSURE_RETURN_FALSE_MSG(it != renderersContainer.end(), "Couldn't find renderer in the registry");

    renderer.reset();
    renderersContainer.erase(it);

    return true;
}

bool RendererRegistry::containsFactory(RendererBackend backend) const {
    const auto index = static_cast<size_t>(backend);
    NOX_ENSURE_RETURN_FALSE(index < m_factories.size());
    const auto &factory = m_factories.at(index);

    return (factory.first != nullptr) &&
           (factory.second != nullptr);
}

} // namespace NOX
