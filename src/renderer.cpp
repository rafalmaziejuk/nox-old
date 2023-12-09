#include "nox_assert.h"
#include "renderer_registry.h"

#include <nox/renderer.h>

namespace NOX {

RendererPtr Renderer::create(RendererBackend backend) {
    auto &registry = RendererRegistry::instance();
    NOX_ENSURE_RETURN_NULLPTR(registry.loadPlugin(backend));

    return registry.createRenderer(backend);
}

bool Renderer::release(RendererPtr &&renderer) {
    auto &registry = RendererRegistry::instance();
    NOX_ENSURE_RETURN_FALSE_MSG(registry.releaseRenderer(std::move(renderer)), "Couldn't release renderer");

    return true;
}

} // namespace NOX
