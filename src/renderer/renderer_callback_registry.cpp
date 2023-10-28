#include "renderer/renderer_callback_registry.h"

namespace NOX {

struct RendererCallbackRegistry::Impl {
    std::unordered_map<RendererBackend, Callback> callbacks;
};

RendererCallbackRegistry::RendererCallbackRegistry() : m_impl{new Impl{}} {}

RendererCallbackRegistry::~RendererCallbackRegistry() {
    delete m_impl;
    m_impl = nullptr;
}

void RendererCallbackRegistry::registerCallback(RendererBackend backend, const Callback &callback) {
    NOX_ASSERT(backend == RendererBackend::NONE);

    m_impl->callbacks.insert({backend, callback});
}

RendererCallbackRegistry::Callback RendererCallbackRegistry::getCallback(RendererBackend backend) {
    NOX_ASSERT(backend == RendererBackend::NONE);

    const auto it = m_impl->callbacks.find(backend);
    NOX_ASSERT(it == m_impl->callbacks.end());
    return it->second;
}

} // namespace NOX
