#include "renderer/renderer_factory_registry.h"

#include <nox/buffer.h>
#include <nox/command_list.h>
#include <nox/pipeline_state.h>
#include <nox/renderer.h>
#include <nox/render_pass.h>
#include <nox/render_target.h>
#include <nox/resource.h>
#include <nox/shader.h>
#include <nox/swap_chain.h>
#include <nox/texture.h>

namespace NOX {

RendererPtr Renderer::create(RendererBackend backend) {
    auto &registry = RendererFactoryRegistry::instance();
    if (registry.initializeFactory(backend)) {
        const auto &[createRenderer, destroyRenderer] = registry[backend];
        return {createRenderer(), destroyRenderer};
    }

    return nullptr;
}

} // namespace NOX
