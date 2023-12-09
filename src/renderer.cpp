#include "nox_assert.h"
#include "renderer_factory_registry.h"

#include <nox/renderer.h>

namespace NOX {

RendererPtr Renderer::create(RendererBackend backend) {
    auto &registry = RendererFactoryRegistry::instance();
    NOX_ENSURE_RETURN_NULLPTR_MSG(registry.registerFactory(backend), "Couldn't register renderer factory");

    const auto &[createRenderer, destroyRenderer] = registry[backend];
    return {createRenderer(), destroyRenderer};
}

} // namespace NOX
