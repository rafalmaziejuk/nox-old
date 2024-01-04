#include "nox_assert.h"
#include "renderer_factory_registry.h"
#include "plugins/plugin_interface.h"

#include <optional>

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

std::optional<RendererFactory> rendererFactory{};
std::unique_ptr<Plugin> rendererPlugin{nullptr};

} // namespace

RendererPtr RendererFactoryRegistry::createRenderer(RendererBackend backend) {
    if (!rendererFactory.has_value()) {
        auto plugin = createRendererPlugin(backend);
        NOX_ENSURE_RETURN_NULLPTR_MSG(plugin != nullptr, "Couldn't create renderer plugin");
        NOX_ENSURE_RETURN_NULLPTR_MSG(plugin->pluginRegister(), "Couldn't register renderer plugin");
        NOX_ENSURE_RETURN_NULLPTR_MSG(plugin->pluginVersion() == NOX_PLUGIN_API_VERSION, "Plugin API version mismatch");
        NOX_ENSURE_RETURN_NULLPTR_MSG(rendererFactory.has_value(), "Couldn't initialize renderer factory");
        rendererPlugin = std::move(plugin);
    }

    const auto &[createRenderer, destroyRenderer] = rendererFactory.value();
    return {createRenderer(), destroyRenderer};
}

void RendererFactoryRegistry::initializeFactory(const RendererFactory &factory) {
    rendererFactory = factory;
}

} // namespace nox
