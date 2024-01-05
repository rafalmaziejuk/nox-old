#include "nox_assert.h"
#include "renderer_factory_registry.h"

#include <nox/renderer.h>

namespace nox {

RendererPtr Renderer::create(RendererBackend backend) {
    NOX_ENSURE_RETURN_NULLPTR_MSG(RendererFactoryRegistry::loadRendererPlugin(backend), "Couldn't load renderer plugin");

    auto renderer = RendererFactoryRegistry::createRenderer();
    NOX_ASSERT_MSG(renderer != nullptr, "Couldn't create renderer");

    return renderer;
}

} // namespace nox
