#include "renderer/renderer_callback_registry.h"

namespace NOX {

std::unordered_map<RendererBackend, RendererCallbackRegistry::Callback> RendererCallbackRegistry::m_callbacks;

void RendererCallbackRegistry::registerCallback(RendererBackend backend, const Callback &callback) {
    NOX_ASSERT(backend == RendererBackend::NONE);

    m_callbacks.insert({backend, callback});
}

RendererCallbackRegistry::Callback RendererCallbackRegistry::getCallback(RendererBackend backend) {
    NOX_ASSERT(backend == RendererBackend::NONE);

    const auto &callback = m_callbacks.at(backend);
    return callback;
}

bool RendererCallbackRegistry::isCallbackRegistered(RendererBackend backend) {
    NOX_ASSERT(backend == RendererBackend::NONE);

    const auto it = m_callbacks.find(backend);
    return (it != m_callbacks.end());
}

} // namespace NOX
