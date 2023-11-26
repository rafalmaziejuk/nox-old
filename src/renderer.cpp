#include "renderer_factory_registry.h"

#include <nox/config.h>
#include <nox/renderer.h>

namespace NOX {

RendererPtr Renderer::create(RendererBackend backend) {
    NOX_ASSERT(backend < RendererBackend::MAX);

    auto &registry = RendererFactoryRegistry::instance();
    if (registry.registerFactory(backend)) {
        const auto &[createRenderer, destroyRenderer] = registry[backend];
        return {createRenderer(), destroyRenderer};
    }

    NOX_ASSERT_MSG(false, "Couldn't create Renderer instance");
    return nullptr;
}

} // namespace NOX
