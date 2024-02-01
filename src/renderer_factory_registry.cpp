#include "asserts.h"
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
RendererBackend rendererBackend{RendererBackend::NONE};

} // namespace

bool RendererFactoryRegistry::loadRendererPlugin(RendererBackend backend) {
    if (rendererFactory.has_value()) {
        NOX_ENSURE_RETURN_FALSE_MSG(backend == rendererBackend, "Creation of different API renderers isn't allowed");
        return true;
    }

    auto plugin = createRendererPlugin(backend);
    NOX_ENSURE_RETURN_FALSE_MSG(plugin != nullptr, "Couldn't create renderer plugin");
    NOX_ENSURE_RETURN_FALSE_MSG(plugin->pluginRegister(), "Couldn't register renderer plugin");
    NOX_ENSURE_RETURN_FALSE_MSG(plugin->pluginVersion() == NOX_PLUGIN_API_VERSION, "Plugin API version mismatch");
    NOX_ENSURE_RETURN_FALSE_MSG(rendererFactory.has_value(), "Couldn't initialize renderer factory");
    rendererPlugin = std::move(plugin);

    return true;
}

RendererPtr RendererFactoryRegistry::createRenderer() {
    NOX_ENSURE_RETURN_NULLPTR_MSG(rendererFactory.has_value(), "Renderer factory hasn't been properly initialized");

    const auto &[createRenderer, destroyRenderer] = rendererFactory.value();
    return {createRenderer(), destroyRenderer};
}

void RendererFactoryRegistry::initializeFactory(RendererBackend backend, const RendererFactory &factory) {
    rendererBackend = backend;
    rendererFactory = factory;
}

} // namespace nox
