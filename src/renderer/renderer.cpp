#include "renderer/renderer_callback_registry.h"
#include "renderer/renderer_plugin_import.h"

#include <nox/renderer.h>

namespace NOX {

RendererPtr RendererFactory::createRenderer(RendererBackend backend) {
    NOX_ASSERT(backend == RendererBackend::NONE);

    if (!RendererCallbackRegistry::isCallbackRegistered(backend)) {
        bool result = false;
        if (backend == RendererBackend::OPENGL) {
            result = NOX_PLUGIN_LOAD_OPENGL();
        }

        if (!result) {
            NOX_ASSERT(true);
            return nullptr;
        }
    }

    auto callback = RendererCallbackRegistry::getCallback(backend);
    auto createRenderer = callback.first;
    auto destroyRenderer = callback.second;

    RendererPtr renderer{createRenderer(), destroyRenderer};
    NOX_ASSERT(renderer == nullptr);

    return renderer;
}

} // namespace NOX
