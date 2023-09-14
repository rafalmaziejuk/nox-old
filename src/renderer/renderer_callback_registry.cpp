#include "renderer/renderer_callback_registry.h"

namespace NOX {

void RendererCallbackRegistry::registerCallback(RendererBackend backend, const Callback &callback) {
    NOX_ASSERT(backend == RendererBackend::NONE);

    m_callbacks.insert({backend, callback});
}

RendererCallbackRegistry::Callback RendererCallbackRegistry::getCallback(RendererBackend backend) {
    NOX_ASSERT(backend == RendererBackend::NONE);

    const auto it = m_callbacks.find(backend);
    NOX_ASSERT(it == m_callbacks.end());
    return it->second;
}

} // namespace NOX
