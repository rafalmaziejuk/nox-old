#include "asserts.h"
#include "renderer_factory_registry.h"

#include <nox/renderer.h>

namespace nox {

RendererPtr Renderer::create(RendererBackend backend) {
    auto &registry = RendererFactoryRegistry::instance();
    NOX_ENSURE_RETURN_NULLPTR(registry.initialize(backend));

    const auto &rendererFactory = registry.getFactory(backend);
    NOX_ENSURE_RETURN_NULLPTR(rendererFactory.has_value());

    const auto &[createRenderer, destroyRenderer] = rendererFactory.value();
    auto *renderer = createRenderer();
    NOX_ASSERT_MSG(renderer != nullptr, "Couldn't create renderer");

    return {renderer, destroyRenderer};
}

} // namespace nox
