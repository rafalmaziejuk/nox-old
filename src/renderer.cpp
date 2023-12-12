#include "nox_assert.h"
#include "renderer_factory_registry.h"

#include <nox/renderer.h>

namespace nox {

RendererPtr Renderer::create(RendererBackend backend) {
    auto &registry = RendererFactoryRegistry::instance();
    NOX_ENSURE_RETURN_NULLPTR_MSG(registry.loadPlugin(backend), "Couldn't register renderer factory");

    return registry.createRenderer(backend);
}

} // namespace nox
