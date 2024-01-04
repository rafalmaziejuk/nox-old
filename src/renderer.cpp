#include "nox_assert.h"
#include "renderer_factory_registry.h"

#include <nox/renderer.h>

namespace nox {

RendererPtr Renderer::create(RendererBackend backend) {
    auto renderer = nox::RendererFactoryRegistry::createRenderer(backend);
    NOX_ASSERT_MSG(renderer != nullptr, "Couldn't create renderer");

    return renderer;
}

} // namespace nox
